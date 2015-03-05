
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _H264MediaParser_h_
#define _H264MediaParser_h_

#include "MediaParser/Defines.h"
#include "MediaParser/H264Info.h"
#include "MediaParser/RBSPIter.h"
#include "XSDK/XBitsy.h"
#include "XSDK/XMemory.h"
#include "XSDK/XSharedLib.h"
#include <cmath>

class H264MediaParserTest;

namespace MEDIA_PARSER
{
    class H264Info;
    class MediaInfo;

    class H264MediaParser
    {
        friend class ::H264MediaParserTest;

        struct SPSInfo
        {
            SPSInfo()
                : frameWidth(0),
                  frameHeight(0),
                  frameRate(0),
                  bitRate(0),
                  chromaFormat(CHROMA_FORMAT_UNKNOWN),
                  levelIDC(0)
            {}

            unsigned int  frameWidth;
            unsigned int  frameHeight;

            double        frameRate;
            unsigned int  bitRate;

            CHROMA_FORMAT chromaFormat;
            unsigned int  levelIDC;
        };

    public:
        //Later we need to make a H264MediaParser base and create 2 types which
        //parse h264 byte stream formatted and non-byte stream formatted streams
        X_API H264MediaParser(const uint8_t* buffer,
                              const size_t bufferSize,
                              bool byteStreamFormat = true);
        X_API bool IsYourType() const;

        //parseFull indicates whether the entire frame should be parsed (as opposed to
        //just what needs to be parsed to get the data we want). Parsing the whole
        //thing makes sense when you want to verify the frame's correctness, but if
        //all you want is the data, it might make more sense to not parse it all in
        //case the latter portion of the frame is messed up.
        X_API bool Parse(MediaInfo& info, bool parseFull = false);
        X_API bool Parse(H264Info& info, bool parseFull = false);

        X_API static size_t FindH264NalUnit(const uint8_t* frame,
                                            const size_t length,
                                            size_t& nal_start,
                                            size_t& nal_end);
    private:
        H264MediaParser(const H264MediaParser&);
        H264MediaParser& operator=(const H264MediaParser&);

        bool _IsSPS(const uint8_t type) const;
        bool _IsPPS(const uint8_t type) const;
        bool _IsNalType(const uint8_t type) const;
        bool _Parse(const uint8_t* buffer, size_t length, H264Info& info, int nalIndex, bool parseFull);

        //These functions use the horrible underscore naming scheme, because
        //the portion after _Parse_ is actually the name from the H264 spec,
        //and apparently they have no taste when it comes to identifier names.
        XIRef<XSDK::XMemory> _Parse_sei_message(RBSPIter iter, RBSPIter end) const;
        XIRef<XSDK::XMemory> _Parse_sei_payload(RBSPIter iter, RBSPIter end, const size_t payloadSize) const;
        SPSInfo _Parse_seq_parameter_set_data(XSDK::XBitsy<RBSPIter>& bitsy, bool parseFull) const;
        void _Parse_vui_parameters(XSDK::XBitsy<RBSPIter>& bitsy, SPSInfo& info, bool parseFull) const;
        void _Parse_hrd_parameters(XSDK::XBitsy<RBSPIter>& bitsy, SPSInfo& info, bool setBitrate, bool parseFull) const;
        void _Parse_scaling_list(XSDK::XBitsy<RBSPIter>& bitsy, size_t sizeOfScalingList) const;

        // Templatized to simplify testing (otherwise, we'd have to stuff a bunch
        // emulation bytes in the values being tested to make it work with RBSPIter).
        template <typename Iter>
        X_API static uint32_t _ParseUEV(XSDK::XBitsy<Iter>& bitsy);

        template <typename Iter>
        X_API static int32_t _ParseSEV(XSDK::XBitsy<Iter>& bitsy);

        const uint8_t* _buffer;
        const size_t   _bufferSize;
        const bool     _byteStreamFormat;

        uint8_t* _sps;
        size_t _spsLength;
        uint8_t* _pps;
        size_t _ppsLength;
    };


    template<typename Iter>
    uint32_t H264MediaParser::_ParseUEV(XSDK::XBitsy<Iter>& bitsy)
    {
        int zeroes = 0;

        for(; !bitsy.GetExactBits(1); ++zeroes)
        {}

        if(zeroes > 32)
            X_THROW(("Invalid exp-golomb encoding. Too many 0's: %d", zeroes));

        const uint32_t num = zeroes == 0 ? 0 : bitsy.GetExactBits(zeroes);

        return std::pow(2.0L, zeroes) - 1 + num;
    }

    template<typename Iter>
    int32_t H264MediaParser::_ParseSEV(XSDK::XBitsy<Iter>& bitsy)
    {
        const uint32_t uev = _ParseUEV(bitsy);

        if(uev == 0)
            return 0;

        const int32_t retval = (int32_t)(uev + 1) / 2;

        if(!(uev & 1))
            return retval * -1;

        return retval;
    }

};

#endif
