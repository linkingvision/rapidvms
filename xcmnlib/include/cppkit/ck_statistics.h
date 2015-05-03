
#ifndef _cppkit_ck_statistics_h_
#define _cppkit_ck_statistics_h_

#include <list>
#include <chrono>
#include "cppkit/os/ck_exports.h"

namespace cppkit
{

template<class SAMPLE_TYPE>
struct _sample
{
    SAMPLE_TYPE value;
    std::chrono::steady_clock::time_point time;
    bool operator<(const struct _sample<SAMPLE_TYPE> &other)
    {
        return value < other.value;
    }
};

class average
{
public:
    template<class SAMPLE_TYPE>
    CK_API static SAMPLE_TYPE calculate(std::list<struct _sample<SAMPLE_TYPE> > &samples, const int count)
    {
        SAMPLE_TYPE total = 0;
        typename std::list<struct _sample<SAMPLE_TYPE> >::iterator it = samples.begin();
        while( it != samples.end() )
        {
            total += it->value;
            it++;
        }
        return total / count;
    }
};

class median
{
public:
    template<class SAMPLE_TYPE>
    CK_API static SAMPLE_TYPE calculate(std::list<struct _sample<SAMPLE_TYPE> > &samples, const int count)
    {
        samples.sort();
        typename std::list<struct _sample<SAMPLE_TYPE> >::iterator it = samples.begin();
        for (int ii=0; ii<count/2; ii++)
            it++;
        return it->value;
    }
};

template<class T,class SAMPLE_TYPE>
class ck_statistics
{
public:
    CK_API ck_statistics() :
    _currentSamples(0), _maxSamples(0), _sampleLifetime(std::chrono::milliseconds(0)) {}
    CK_API ck_statistics( int maxSamples ) :
    _currentSamples(0), _maxSamples(maxSamples), _sampleLifetime(std::chrono::milliseconds(0)) {}
    CK_API ck_statistics( int maxSamples, std::chrono::milliseconds sampleLifetime ) :
        _currentSamples(0), _maxSamples(maxSamples), _sampleLifetime(sampleLifetime) {}

    CK_API void set_max_samples( int maxSamples ) { _maxSamples = maxSamples; }
    CK_API int get_max_samples() const { return _maxSamples; }

    CK_API void set_sample_lifetime( std::chrono::milliseconds lifetime ) { _sampleLifetime = lifetime; }
    CK_API std::chrono::milliseconds get_sample_lifetime() const { return _sampleLifetime; }

    CK_API int get_num_samples() { _prune_expired(); return _currentSamples; }
    // Returns number of samples, or -1 on error.
    CK_API int add_sample( SAMPLE_TYPE sample )
    {
        struct _sample<SAMPLE_TYPE> s = { sample, std::chrono::steady_clock::now() };
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
    CK_API int get_result( SAMPLE_TYPE& result )
    {
        _prune_expired();
        result = 0;
        if( _currentSamples > 0 )
            result = T::calculate( _samples, _currentSamples );
        return _currentSamples;
    }

    CK_API void clear() { _samples.clear(); _currentSamples = 0; }

private:
    void _prune_expired()
    {
        if( std::chrono::duration_cast<std::chrono::milliseconds>( _sampleLifetime ).count() <= 0 )
            return;
        if ( _samples.empty() )
            return;
        // calculate the expired time and remove old samples.
        std::chrono::steady_clock::time_point expired = std::chrono::steady_clock::now() - _sampleLifetime;
        typename std::list<struct _sample<SAMPLE_TYPE> >::iterator it = _samples.begin();
        while (it != _samples.end())
        {
            // We are naturally sorted from oldest to newest.
            if( it->time > expired )
                break;
            it = _samples.erase(it);
            --_currentSamples;
        }
    }

    std::list<struct _sample<SAMPLE_TYPE> > _samples;
    int _currentSamples;
    int _maxSamples; // 0 means disabled
    std::chrono::milliseconds _sampleLifetime;
};

}

#endif
