
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/H264MP4ToAnnexB.h"

#include "XSDK/XException.h"

using namespace AVKit;
using namespace XSDK;

H264MP4ToAnnexB::H264MP4ToAnnexB( AVDeMuxer& deMuxer ) :
    _bsfc( av_bitstream_filter_init( "h264_mp4toannexb" ) ),
    _codec( deMuxer._context->streams[deMuxer._videoStreamIndex]->codec ),
    _filteredPacket()
{
    if( !_bsfc )
        X_THROW(("Unable to initialize h264_mp4toannexb bitstream filter."));

    if( !_codec )
        X_THROW(("H264MP4ToAnnexB requires valid codec context (either from an encoder or a demuxer)."));
}

H264MP4ToAnnexB::~H264MP4ToAnnexB() throw()
{
    _FreeFilteredPacket();

    av_bitstream_filter_close( _bsfc );
}

void H264MP4ToAnnexB::Transform( uint8_t* src, size_t srcSize, bool keyFrame )
{
    AVPacket inputPacket;
    av_init_packet( &inputPacket );
    inputPacket.data = src;
    inputPacket.size = srcSize;

    _FreeFilteredPacket();

    // If the return value is 0, the output buffer is not allocated and should be considered identical to the
    // input buffer, or in case *poutbuf was set it points to the input buffer (not necessarily to its starting
    // address).

    av_bitstream_filter_filter( _bsfc, _codec, NULL,
                                &_filteredPacket.data, &_filteredPacket.size,
                                inputPacket.data, inputPacket.size,
                                (keyFrame) ? 1 : 0 );

    // Given the super tricky comment above (lifted right from the ffmpeg documentation!) our strategy is this:
    // If our output buffer overlaps our input, then allocate storage for it, copy the data into it and reset
    // our _filteredPacket.data and _filteredPacket.size to point to it. This way, we can always guarantee our
    // _filteredPacket is distinct from out inputPacket.

    if( _filteredPacket.data >= inputPacket.data && _filteredPacket.data < (inputPacket.data + inputPacket.size) )
    {
        size_t filteredSize = inputPacket.size - (_filteredPacket.data - inputPacket.data);
        uint8_t* data = (uint8_t*)av_malloc( filteredSize );
        if( !data )
            X_THROW(("Unable to allocate filter packet."));
        memcpy( data, _filteredPacket.data, filteredSize );
        _filteredPacket.data = data;
        _filteredPacket.size = filteredSize;
    }
}

void H264MP4ToAnnexB::Transform( XIRef<XMemory> src, bool keyFrame )
{
    Transform( src->Map(), src->GetDataSize(), keyFrame );
}

size_t H264MP4ToAnnexB::GetAnnexBSize() const
{
    return _filteredPacket.size;
}

void H264MP4ToAnnexB::GetAnnexB( uint8_t* dest ) const
{
    memcpy( dest, _filteredPacket.data, _filteredPacket.size );
}

XIRef<XMemory> H264MP4ToAnnexB::GetAnnexB() const
{
    XIRef<XMemory> buffer = new XMemory;
    uint8_t* dest = &buffer->Extend( _filteredPacket.size );
    GetAnnexB( dest );
    return buffer;
}

void H264MP4ToAnnexB::_FreeFilteredPacket()
{
    if( _filteredPacket.size > 0 )
    {
        av_free( _filteredPacket.data );
        _filteredPacket.data = NULL;
        _filteredPacket.size = 0;
    }
}
