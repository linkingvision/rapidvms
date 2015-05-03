
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _MPEG4MediaParser_h_
#define _MPEG4MediaParser_h_

#include "MediaParser/Defines.h"
#include "XSDK/XSharedLib.h"

namespace MEDIA_PARSER
{
    class MPEG4Info;
    class MediaInfo;

    class MPEG4MediaParser
    {
    public:
        X_API MPEG4MediaParser(const uint8_t* buffer, const size_t bufferSize);
        X_API bool IsYourType(void) const;
        X_API bool Parse(MediaInfo &) const;
        X_API bool Parse(MPEG4Info &) const;
    private:
        MPEG4MediaParser(const MPEG4MediaParser &);
        MPEG4MediaParser & operator=(const MPEG4MediaParser &);

        const uint8_t*  _buffer;
        const size_t _bufferSize;
    };
};

#endif
