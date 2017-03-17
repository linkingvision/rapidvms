
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/AVMuxer.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace AVKit;
using namespace XSDK;

AVMuxer::AVMuxer( const struct CodecOptions& options,
                  const XString& fileName,
                  OUTPUT_LOCATION location ) :
    _options( options ),
    _fileName( fileName ),
    _context( NULL ),
    _stream( NULL ),
    _location( location ),
    _ts( 0 ),
    _oweTrailer( false ),
    _numVideoFramesWritten( 0 ),
    _isTS( _fileName.ToLower().EndsWith( ".ts" ) ),
    _fileNum( 0 )
{
    _context = avformat_alloc_context();
    if( !_context )
        X_THROW(("Unable to allocate output format context."));

    _context->oformat = av_guess_format( NULL, _fileName.c_str(), NULL );

    if( !_context->oformat )
        X_THROW(("Unable to guess output format."));

    _context->oformat->video_codec = AV_CODEC_ID_H264;

    _stream = avformat_new_stream( _context, NULL );
    if( !_stream )
        X_THROW(("Unable to allocate output stream."));

    avcodec_get_context_defaults3( _stream->codec, NULL );

	_stream->codec->codec_id = AV_CODEC_ID_H264;
    _stream->codec->codec_type = AVMEDIA_TYPE_VIDEO;

    ApplyCodecOptions( options );

    if( !_options.gop_size.IsNull() )
        _stream->codec->gop_size = _options.gop_size.Value();
    else X_THROW(("Required option missing: gop_size"));

    _stream->codec->pix_fmt = AV_PIX_FMT_YUV420P;

    if( _context->oformat->flags & AVFMT_GLOBALHEADER )
        _stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
}

AVMuxer::~AVMuxer() throw()
{
    if( !_isTS && _oweTrailer )
        av_write_trailer( _context );

    _CloseIO();

    if( _stream->codec->extradata )
        av_freep( &_stream->codec->extradata );

    avformat_free_context( _context );
}

XString AVMuxer::GetFileName() const
{
    return _fileName;
}

void AVMuxer::SetExtraData( XIRef<XSDK::XMemory> extraData )
{
    if( !(_context->oformat->flags & AVFMT_GLOBALHEADER) )
        X_LOG_INFO("Extradata not required for %s container.",_fileName.c_str());
    else
    {
        _stream->codec->extradata = (uint8_t*)av_mallocz( extraData->GetDataSize() );
        if( !_stream->codec->extradata )
            X_THROW(("Unable to allocate extradata storage."));
        _stream->codec->extradata_size = extraData->GetDataSize();

        memcpy( _stream->codec->extradata, extraData->Map(), extraData->GetDataSize() );
    }
}

void AVMuxer::WriteVideoFrame( uint8_t* data, size_t size, bool keyFrame )
{
    if( _context->pb == NULL )
        _OpenIO();

    if( _isTS )
    {
        if( _numVideoFramesWritten == 0 )
        {
            if( _fileNum == 0 )
            {
                if( avformat_write_header( _context, NULL ) < 0 )
                    X_THROW(("Unable to write header to container."));
            }

            av_opt_set( _context->priv_data, "mpegts_flags", "resend_headers", 0 );
        }
    }
    else
    {
        if( !_oweTrailer )
        {
            if( avformat_write_header( _context, NULL ) < 0 )
                X_THROW(("Unable to write header to container."));

            _oweTrailer = true;
        }
    }

    AVPacket pkt;
    av_init_packet( &pkt );

    pkt.stream_index = _stream->index;
    pkt.data = data;
    pkt.size = size;

    pkt.pts = _ts;
    pkt.dts = _ts;

    _ts += av_rescale_q(1, _stream->codec->time_base, _stream->time_base);

    pkt.flags |= (keyFrame) ? AV_PKT_FLAG_KEY : 0;

    if( av_interleaved_write_frame( _context, &pkt ) < 0 )
        X_THROW(("Unable to write video frame."));

    _numVideoFramesWritten++;
}

void AVMuxer::WriteVideoFrame( XIRef<XMemory> frame, bool keyFrame )
{
    WriteVideoFrame( frame->Map(), frame->GetDataSize(), keyFrame );
}

void AVMuxer::Flush()
{
    if( av_interleaved_write_frame( _context, NULL ) < 0 )
        X_THROW(("Unable to flush AVMuxer."));
}

void AVMuxer::FinalizeBuffer( XIRef<XSDK::XMemory> buffer )
{
    if( _location != OUTPUT_LOCATION_BUFFER )
        X_THROW(("Unable to finalize a non buffer IO object."));

    _FinalizeCommon();

    uint8_t* fileBytes = NULL;
    int fileSize = avio_close_dyn_buf( _context->pb, &fileBytes );
    _context->pb = NULL;

    if( fileBytes == NULL || fileSize == 0 )
        X_THROW(("Unable to finalize empty buffer."));

    buffer->ResizeData( fileSize );

    memcpy( buffer->Map(), fileBytes, fileSize );

    av_freep( &fileBytes );
}

void AVMuxer::FinalizeFile()
{
    if( _location != OUTPUT_LOCATION_FILE )
        X_THROW(("Unable to finalize a non file IO object."));

    _FinalizeCommon();

    avio_close( _context->pb );
    _context->pb = NULL;
}

void AVMuxer::ApplyCodecOptions( const struct CodecOptions& options )
{
    _options = options;

    if( !_options.profile.IsNull() )
    {
        if( _options.profile.Value().ToLower() == "baseline" )
            _stream->codec->profile = FF_PROFILE_H264_BASELINE;
        else if( _options.profile.Value().ToLower() == "main" )
            _stream->codec->profile = FF_PROFILE_H264_MAIN;
        else if( _options.profile.Value().ToLower() == "high" )
            _stream->codec->profile = FF_PROFILE_H264_HIGH;

        av_opt_set( _stream->codec->priv_data, "profile", _options.profile.Value().ToLower().c_str(), 0 );
    }

    if( !_options.bit_rate.IsNull() )
        _stream->codec->bit_rate = _options.bit_rate.Value();
    else X_THROW(("Required option missing: bit_rate"));

    if( !_options.width.IsNull() )
        _stream->codec->width = _options.width.Value();
    else X_THROW(("Required option missing: width"));

    if( !_options.height.IsNull() )
        _stream->codec->height = _options.height.Value();
    else X_THROW(("Required option missing: height"));

    if( !_options.time_base_num.IsNull() )
        _stream->codec->time_base.num = _options.time_base_num.Value();
    else X_THROW(("Required option missing: time_base_num"));

    if( !_options.time_base_den.IsNull() )
        _stream->codec->time_base.den = _options.time_base_den.Value();
    else X_THROW(("Required option missing: time_base_den"));
}

void AVMuxer::_FinalizeCommon()
{
    if( !_context->pb )
        X_THROW(("Unable to finalize an unopened IO object."));

    if( !_isTS && _oweTrailer )
    {
        av_write_trailer( _context );
        _oweTrailer = false;
    }

    _fileNum++;

    _numVideoFramesWritten = 0;
}

void AVMuxer::_OpenIO()
{
    if( _context->pb == NULL )
    {
        if( _location == OUTPUT_LOCATION_BUFFER )
        {
            avio_open_dyn_buf( &_context->pb );
            if( !_context->pb )
                X_THROW(("Unable to allocate a memory IO object."));
        }
        else
        {
            if( avio_open( &_context->pb, _fileName.c_str(), AVIO_FLAG_WRITE ) < 0 )
                X_THROW(("Unable to open output file."));
        }
    }
}

void AVMuxer::_CloseIO()
{
    if( _context->pb )
    {
        if( _location == OUTPUT_LOCATION_BUFFER )
        {
            uint8_t* fileBytes = NULL;
            int fileSize = avio_close_dyn_buf( _context->pb, &fileBytes );
            av_freep( &fileBytes );
        }
        else
        {
            avio_close( _context->pb );
        }

        _context->pb = NULL;
    }
}
