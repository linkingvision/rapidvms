
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _H264Info_h_
#define _H264Info_h_

#include "MediaParser/MediaInfo.h"
#include "XSDK/XSharedLib.h"

class H264MediaParserTest;

namespace MEDIA_PARSER
{
    // NAL unit types
    typedef enum _NAL_TYPE
    {
        NAL_UNSPECIFIED=0,
        NAL_SLICE,
        NAL_DPA,
        NAL_DPB,
        NAL_DPC,
        NAL_IDR_SLICE,
        NAL_SEI,
        NAL_SPS,
        NAL_PPS,
        NAL_AUD,
        NAL_END_SEQUENCE,
        NAL_END_STREAM,
        NAL_FILLER_DATA,
        NAL_SPS_EXT,
        NAL_PREFIX,
        NAL_SUB_SEQ_PS,
        NAL_RESERVED_16,
        NAL_RESERVED_17,
        NAL_RESERVED_18,
        NAL_AUXILIARY_SLICE,
        NAL_CODED_SLICE,
        NAL_RESERVED_21,
        NAL_RESERVED_22,
        NAL_RESERVED_23,
        NAL_UNSPECIFED_24,
        NAL_UNSPECIFED_25,
        NAL_UNSPECIFED_26,
        NAL_UNSPECIFED_27,
        NAL_UNSPECIFED_28,
        NAL_UNSPECIFED_29,
        NAL_UNSPECIFED_30,
        NAL_UNSPECIFED_31,
    } NAL_TYPE;

    class H264Info : public MediaInfo
    {
        friend class H264MediaParser;
        friend class ::H264MediaParserTest;
        friend class MediaParser;
    public:
        X_API H264Info()
            : MediaInfo(),
              _levelIDC(0)
        {}

        X_API virtual ~H264Info() {}

        unsigned int GetLevelIDC() { return _levelIDC; }
        void SetLevelIDC(unsigned int levelIDC) { _levelIDC = levelIDC; }

    private:

        unsigned int _levelIDC;
    };

}

#endif
