
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/MJPEGMediaParser.h"
#include "MediaParser/MJPEGInfo.h"
#include "XSDK/XSocket.h" // for x_ntohs()

using namespace MEDIA_PARSER;

static const uint8_t kSOIMarker[] = { 0xFF, 0xD8 };
static const uint8_t kEOIMarker[] = { 0xFF, 0xD9 };

MJPEGMediaParser::MJPEGMediaParser(const uint8_t* buffer, const size_t bufferSize)
    : _buffer(buffer),
      _bufferSize(bufferSize)
{
}

bool MJPEGMediaParser::IsYourType(void) const
{
    // Verify Start-Of-Image and End-Of-Image marker codes.
    const size_t last = _bufferSize - 1;
    if (_buffer[0] == kSOIMarker[0] &&
        _buffer[1] == kSOIMarker[1] &&
        _buffer[last-1] == kEOIMarker[0] &&
        _buffer[last] == kEOIMarker[1])
        return true;
    return false;
}

bool MJPEGMediaParser::Parse(MediaInfo &info) const
{
    if (!IsYourType())
        return false;

    info.SetFrameType(FRAME_TYPE_I_FRAME);
    info.SetContainsKeyFrame(true);
    info.SetMediaType(MEDIA_TYPE_MJPEG);

    // Find Start-Of-Frame marker.
    // Valid: 0xFF followed by C0,C1,C2,C3,C9,C10,C11
    // C0 and C2 have known chroma format of 4:2:0

    size_t pos = 2; // Skip SOI marker
    while (_FindNextMarker(pos))
    {
        if (_buffer[pos] != 0xFF)
            return false;

        switch (_buffer[pos+1])
        {
        case 0xD9: // End-Of-Image
            return false;
        case 0xC0:
        case 0xC2:
            info.SetChromaFormat(CHROMA_FORMAT_YUV_420);
        case 0xC1:
        case 0xC3:
        case 0xC9:
        case 0xCA:
        case 0xCB:
            info.SetFrameHeight( x_ntohs( *(uint16_t*)&_buffer[pos+5] ) );
            info.SetFrameWidth ( x_ntohs( *(uint16_t*)&_buffer[pos+7] ) );
            return true;
        };
    }

    return false;
}

bool MJPEGMediaParser::Parse(MJPEGInfo &info) const
{
    return Parse((MediaInfo&)info);
}

bool MJPEGMediaParser::_FindNextMarker(size_t& position) const
{
    position += 2; // Skip past current marker to segment length

    if (position + 2 >= _bufferSize) // Make sure we have room for length
        return false;

    size_t len = x_ntohs( *(uint16_t*)&_buffer[position] ); // length is 16-bits
    if ((position + len) >= _bufferSize)
        return false;

    position += len;
    return true;
}
