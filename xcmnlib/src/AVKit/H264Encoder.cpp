
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/H264Encoder.h"

#include "XSDK/XException.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace AVKit;
using namespace XSDK;

static const size_t DEFAULT_PADDING = 16;
static const size_t DEFAULT_ENCODE_BUFFER_SIZE = (1024*1024);
static const size_t DEFAULT_EXTRADATA_BUFFER_SIZE = (1024*256);

H264Encoder::H264Encoder( const struct CodecOptions& options,
                          bool annexB,
                          int encodeAttempts ) :
    _codec( avcodec_find_encoder( AV_CODEC_ID_H264 ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _numTillKey( 0 ),
    _encodeAttempts( encodeAttempts ),
    _extraData( DEFAULT_EXTRADATA_BUFFER_SIZE ),
    _lastWasKey( false ),
    _annexB( annexB )
{
    if( !_codec )
        X_THROW(("Unable to locate H264 codec."));

    if( !_context )
        X_THROW(("Unable to allocate H264 codec context."));

    _context->codec_id = AV_CODEC_ID_H264;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;
    _context->pix_fmt = AV_PIX_FMT_YUV420P;

    if( !_options.gop_size.IsNull() )
        _context->gop_size = _options.gop_size.Value();

    if( !_options.bit_rate.IsNull() )
        _context->bit_rate = _options.bit_rate.Value();
    else X_THROW(("Required option missing: bit_rate"));

    if( !_options.width.IsNull() )
        _context->width = _options.width.Value();
    else X_THROW(("Required option missing: width"));

    if( !_options.height.IsNull() )
        _context->height = _options.height.Value();
    else X_THROW(("Required option missing: height"));

    if( !_options.time_base_num.IsNull() )
        _context->time_base.num = _options.time_base_num.Value();
    else X_THROW(("Required option missing: time_base.num"));

    if( !_options.time_base_den.IsNull() )
        _context->time_base.den = _options.time_base_den.Value();
    else X_THROW(("Required option missing: time_base.den"));

    if( !_options.me_method.IsNull() )
        _context->me_method = _options.me_method.Value();

    if( !_options.me_subpel_quality.IsNull() )
        _context->me_subpel_quality = _options.me_subpel_quality.Value();

    if( !_options.delay.IsNull() )
        _context->delay = _options.delay.Value();

    if( !_options.thread_count.IsNull() )
    {
        _context->thread_count = _options.thread_count.Value();
        _context->thread_type = FF_THREAD_FRAME;
    }

    if( !_options.refs.IsNull() )
        _context->refs = _options.refs.Value();

    if( !_options.qmin.IsNull() )
        _context->qmin = _options.qmin.Value();

    if( !_options.qmax.IsNull() )
        _context->qmax = _options.qmax.Value();

    if( !_options.max_qdiff.IsNull() )
        _context->max_qdiff = _options.max_qdiff.Value();

    if( !_options.rc_buffer_size.IsNull() )
        _context->rc_buffer_size = _options.rc_buffer_size.Value();

    if( !_options.profile.IsNull() )
    {
        if( _options.profile.Value().ToLower() == "baseline" )
            _context->profile = FF_PROFILE_H264_BASELINE;
        else if( _options.profile.Value().ToLower() == "main" )
            _context->profile = FF_PROFILE_H264_MAIN;
        else if( _options.profile.Value().ToLower() == "high" )
            _context->profile = FF_PROFILE_H264_HIGH;

        av_opt_set( _context->priv_data, "profile", _options.profile.Value().ToLower().c_str(), 0 );
    }

    if( !_options.preset.IsNull() )
        av_opt_set( _context->priv_data, "preset", _options.preset.Value().c_str(), 0 );

    if( !_options.tune.IsNull() )
        av_opt_set( _context->priv_data, "tune", _options.tune.Value().c_str(), 0 );

    if( !_options.x264opts.IsNull() )
        av_opt_set( _context->priv_data, "x264opts", _options.x264opts.Value().c_str(), 0 );

    // We set this here to force FFMPEG to populate the extradata field (necessary if the output stream
    // is going to be muxed into a format with a global header (for example, mp4)).
    _context->flags |= CODEC_FLAG_GLOBAL_HEADER;

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        X_THROW(("Failed to open encoding context."));

    if( _context->extradata == NULL || _context->extradata_size == 0 )
        X_THROW(("No codec extradata available!"));

    memcpy( &_extraData.Extend( _context->extradata_size ), _context->extradata, _context->extradata_size );
}

H264Encoder::~H264Encoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

size_t H264Encoder::EncodeYUV420P( uint8_t* pic, uint8_t* output, size_t outputSize,
                                   FrameType type )
{
    AVFrame frame;
	av_frame_unref(&frame);

    frame.data[0] = pic;
    pic += (_context->width * _context->height);
    frame.data[1] = pic;
    pic += ((_context->width/4) * _context->height);
    frame.data[2] = pic;

    frame.linesize[0] = _context->width;
    frame.linesize[1] = (_context->width/2);
    frame.linesize[2] = (_context->width/2);

    frame.format = _context->pix_fmt;
    frame.width = _context->width;
    frame.height = _context->height;

    if( type != FRAME_TYPE_AUTO_GOP )
    {
        if( type == FRAME_TYPE_KEY )
            frame.pict_type = AV_PICTURE_TYPE_I;
        else frame.pict_type = AV_PICTURE_TYPE_P;
    }

    int attempts = 0;
    int gotPacket = 0;
    AVPacket pkt;

    do
    {
        uint8_t* dest = output;

        if( _annexB )
        {
            memcpy( dest, _context->extradata, _context->extradata_size );
            dest += _context->extradata_size;
        }

        av_init_packet( &pkt );
        pkt.data = dest;
        pkt.size = (_annexB) ? outputSize - _context->extradata_size : outputSize;

        if( avcodec_encode_video2( _context,
                                   &pkt,
                                   &frame,
                                   &gotPacket ) < 0 )
            X_THROW(("Error while encoding."));

        attempts++;

    } while( gotPacket == 0 && (attempts < _encodeAttempts) );

    if( pkt.flags & AV_PKT_FLAG_KEY )
        _lastWasKey = true;
    else _lastWasKey = false;

    return (_annexB) ? pkt.size + _context->extradata_size : pkt.size;
}

XIRef<XMemory> H264Encoder::EncodeYUV420P( XIRef<XMemory> pic,
                                           FrameType type )
{
    XIRef<XMemory> frame = new XMemory( DEFAULT_ENCODE_BUFFER_SIZE + DEFAULT_PADDING );

    uint8_t* p = &frame->Extend( DEFAULT_ENCODE_BUFFER_SIZE );

    size_t outputSize = EncodeYUV420P( pic->Map(),
                                       frame->Map(),
                                       frame->GetDataSize(),
                                       type );
    frame->ResizeData( outputSize );
    return frame;
}

struct CodecOptions H264Encoder::GetOptions() const
{
    return _options;
}

XIRef<XMemory> H264Encoder::GetExtraData() const
{
    XIRef<XMemory> ed = new XMemory( DEFAULT_EXTRADATA_BUFFER_SIZE );

    memcpy( &ed->Extend( _extraData.GetDataSize() ), _extraData.Map(), _extraData.GetDataSize() );

    return ed;
}
