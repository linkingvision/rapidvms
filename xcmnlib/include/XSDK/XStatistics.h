
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XStatistics_H_
#define _XStatistics_H_

#include <list>
#include "XSDK/XSharedLib.h"
#include "XSDK/XTime.h"

namespace XSDK
{
    template<class SampleType>
    struct _Sample
    {
        SampleType value;
        struct timeval time;
        bool operator<(const struct _Sample<SampleType> &other)
        {
            return value < other.value;
        }
    };

    // Average
    class Average
    {
    public:
        template<class SampleType>
        X_API static SampleType Calculate(std::list<struct _Sample<SampleType> > &samples, const int count)
        {
            SampleType total = 0;
            typename std::list<struct _Sample<SampleType> >::iterator it = samples.begin();
            while( it != samples.end() )
            {
                total += it->value;
                it++;
            }
            return total / count;
        }
    };

    // Median
    class Median
    {
    public:
        template<class SampleType>
        X_API static SampleType Calculate(std::list<struct _Sample<SampleType> > &samples, const int count)
        {
            samples.sort();
            typename std::list<struct _Sample<SampleType> >::iterator it = samples.begin();
            for (int ii=0; ii<count/2; ii++)
                it++;
            return it->value;
        }
    };

template<class T,class SampleType>
class XStatistics
{
public:
    X_API XStatistics() :
      _currentSamples(0), _maxSamples(0), _sampleLifetime(XSDK::XDuration::Zero()) {}
    X_API XStatistics( int maxSamples ) :
      _currentSamples(0), _maxSamples(maxSamples), _sampleLifetime(XSDK::XDuration::Zero()) {}
    X_API XStatistics( int maxSamples, XSDK::XDuration sampleLifetime ) :
      _currentSamples(0), _maxSamples(maxSamples), _sampleLifetime(sampleLifetime) {}

    X_API void SetMaxSamples( int maxSamples ) { _maxSamples = maxSamples; }
    X_API int GetMaxSamples() const { return _maxSamples; }

    X_API void SetSampleLifetime( XSDK::XDuration lifetime ) { _sampleLifetime = lifetime; }
    X_API XSDK::XDuration GetSampleLifetime() const { return _sampleLifetime; }

    X_API int GetNumSamples() { _PruneExpired(); return _currentSamples; }
    // Returns number of samples, or -1 on error.
    X_API int AddSample( SampleType sample )
    {
        struct _Sample<SampleType> s = { sample, XSDK::Clock::currTime().ToTimeVal() };
        if( _maxSamples > 0 && (_currentSamples + 1) > _maxSamples )
        {
            _samples.pop_front();
            _samples.push_back( s );
        }
        else
        {
            _samples.push_back( s );
            _currentSamples++;
        }
        return _currentSamples;
    }

    // Returns the number of items comprising the result.
    X_API int GetResult( SampleType& result )
    {
        _PruneExpired();
        result = 0;
        if( _currentSamples > 0 )
            result = T::Calculate( _samples, _currentSamples );
        return _currentSamples;
    }

    X_API void Clear() { _samples.clear(); _currentSamples = 0; }

private:
    void _PruneExpired()
    {
        if( _sampleLifetime.Total(XSDK::MSECS) <= 0 )
            return;
        if ( _samples.empty() )
            return;
        // Calculate the expired time and remove old samples.
        XSDK::XTime expired = XSDK::Clock::currTime() - _sampleLifetime;
        typename std::list<struct _Sample<SampleType> >::iterator it = _samples.begin();
        while (it != _samples.end())
        {
            // We are naturally sorted from oldest to newest.
            if (XSDK::XTime(it->time) > expired)
                break;
            it = _samples.erase(it);
            --_currentSamples;
        }
    }

    std::list<struct _Sample<SampleType> > _samples;
    int _currentSamples;
    int _maxSamples; // 0 means disabled
    XSDK::XDuration _sampleLifetime; // 0 means disabled
}; // class XStatistics

} // namespace XSDK

#endif // _XStatistics_H_
