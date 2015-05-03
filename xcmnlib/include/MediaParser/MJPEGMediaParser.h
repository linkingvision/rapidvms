
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _MJPEGMediaParser_h_
#define _MJPEGMediaParser_h_

#include "MediaParser/Defines.h"
#include "XSDK/XSharedLib.h"

namespace MEDIA_PARSER
{
    class MediaInfo;
    class MJPEGInfo;

    class MJPEGMediaParser
    {
    public:
        X_API MJPEGMediaParser(const uint8_t* buffer, const size_t bufferSize);
        X_API bool IsYourType() const;
        X_API bool Parse(MJPEGInfo& info) const;
        X_API bool Parse(MediaInfo& info) const;
    private:
        MJPEGMediaParser(const MJPEGMediaParser&);
        MJPEGMediaParser& operator=(const MJPEGMediaParser&);

        // Assumes 'position' points to the current marker (0xFF, 0x??)
        bool _FindNextMarker(size_t& position) const;

        const uint8_t*  _buffer;
        const size_t _bufferSize;
    };
};

#endif
