
#include "ffkit/h264_decoder.h"

#include "cppkit/ck_exception.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

static const size_t DEFAULT_PADDING = 16;

h264_decoder::h264_decoder( const struct codec_options& options, int decodeAttempts ) :
    _codec( avcodec_find_decoder( AV_CODEC_ID_H264 ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
	_frame(av_frame_alloc()),
    _scaler( NULL ),
    _outputWidth( 0 ),
    _outputHeight( 0 ),
    _decodeAttempts( decodeAttempts ),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    if( !_codec )
        CK_THROW(( "Failed to find H264 decoder." ));

    if( !_context )
        CK_THROW(( "Failed to allocate decoder context." ));

    if( !_frame )
        CK_THROW(( "Failed to allocate frame." ));

    _context->extradata = NULL;
    _context->extradata_size = 0;

    if( !_options.thread_count.is_null() )
    {
        _context->thread_count = _options.thread_count.value();
        _context->thread_type == FF_THREAD_FRAME;
    }

    if( !_options.tune.is_null() )
        av_opt_set( _context->priv_data, "tune", _options.tune.value().c_str(), 0 );

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        CK_THROW(( "Unable to open H264 decoder." ));
}

h264_decoder::h264_decoder( av_demuxer& deMuxer, const struct codec_options& options, int decodeAttempts ) :
    _codec( avcodec_find_decoder( AV_CODEC_ID_H264 ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
	_frame(av_frame_alloc()),
    _scaler( NULL ),
    _outputWidth( 0 ),
    _outputHeight( 0 ),
    _decodeAttempts( decodeAttempts ),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    if( !_codec )
        CK_THROW(( "Failed to find H264 decoder." ));

    if( !_context )
        CK_THROW(( "Failed to allocate decoder context." ));

    if( !_frame )
        CK_THROW(( "Failed to allocate frame." ));

    if( avcodec_copy_context( _context, deMuxer._context->streams[deMuxer._videoStreamIndex]->codec ) != 0 )
        CK_THROW(("Unable to copy codec context from demuxer."));

    if( !_options.thread_count.is_null() )
    {
        _context->thread_count = _options.thread_count.value();
        _context->thread_type == FF_THREAD_FRAME;
    }

    if( !_options.tune.is_null() )
        av_opt_set( _context->priv_data, "tune", _options.tune.value().c_str(), 0 );

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        CK_THROW(( "Unable to open H264 decoder." ));
}

h264_decoder::~h264_decoder() throw()
{
    _destroy_scaler();

    if( _frame )
        av_free( _frame );

    if( _context )
    {
        avcodec_close( _context );

        av_free( _context );
    }
}

void h264_decoder::decode( std::shared_ptr<av_packet> frame )
{
    AVPacket inputPacket;
    av_init_packet( &inputPacket );
    inputPacket.data = frame->map();
    inputPacket.size = frame->get_data_size();

    int gotPicture = 0;
    int ret = 0;
    int attempts = 0;

    do
    {
        ret = avcodec_decode_video2( _context,
                                     _frame,
                                     &gotPicture,
                                     &inputPacket );

        attempts++;

    } while( (gotPicture == 0) && (attempts < _decodeAttempts) );

    if( attempts >= _decodeAttempts )
        CK_THROW(( "Unable to decode frame." ));

    if( ret < 0 )
        CK_THROW(( "Decoding returned error: %d", ret ));

    if( gotPicture < 1 )
        CK_THROW(( "Unable to decode frame." ));
}

uint16_t h264_decoder::get_input_width() const
{
    return (uint16_t)_context->width;
}

uint16_t h264_decoder::get_input_height() const
{
    return (uint16_t)_context->height;
}

void h264_decoder::set_output_width( uint16_t outputWidth )
{
    if( _outputWidth != outputWidth )
    {
        _outputWidth = outputWidth;

        if( _scaler )
            _destroy_scaler();
    }
}

uint16_t h264_decoder::get_output_width() const
{
    return _outputWidth;
}

void h264_decoder::set_output_height( uint16_t outputHeight )
{
    if( _outputHeight != outputHeight )
    {
        _outputHeight = outputHeight;

        if( _scaler )
            _destroy_scaler();
    }
}

uint16_t h264_decoder::get_output_height() const
{
    return _outputHeight;
}

shared_ptr<av_packet> h264_decoder::get()
{
    if( _outputWidth == 0 )
        _outputWidth = _context->width;

    if( _outputHeight == 0 )
        _outputHeight = _context->height;

    size_t pictureSize = _outputWidth * _outputHeight * 1.5;
    shared_ptr<av_packet> pkt = _pf->get( pictureSize );
    pkt->set_data_size( pictureSize );

    if( _scaler == NULL )
    {
        _scaler = sws_getContext( _context->width,
                                  _context->height,
                                  _context->pix_fmt,
                                  _outputWidth,
                                  _outputHeight,
                                  (_options.jpeg_source.is_null()) ? AV_PIX_FMT_YUV420P : AV_PIX_FMT_YUVJ420P,
                                  SWS_BICUBIC,
                                  NULL,
                                  NULL,
                                  NULL );

        if( !_scaler )
            CK_THROW(( "Unable to allocate scaler context "
                      "(input_width=%u,input_height=%u,output_width=%u,output_height=%u).",
                      _context->width, _context->height, _outputWidth, _outputHeight ));
    }

    uint8_t* dest = pkt->map();

    AVPicture pict;
    pict.data[0] = dest;
    dest += _outputWidth * _outputHeight;
    pict.data[1] = dest;
    dest += (_outputWidth/4) * _outputHeight;
    pict.data[2] = dest;

    pict.linesize[0] = _outputWidth;
    pict.linesize[1] = _outputWidth/2;
    pict.linesize[2] = _outputWidth/2;

    int ret = sws_scale( _scaler,
                         _frame->data,
                         _frame->linesize,
                         0,
                         _context->height,
                         pict.data,
                         pict.linesize );
    if( ret <= 0 )
        CK_THROW(( "Unable to create YUV420P image." ));

    return std::move( pkt );
}

void h264_decoder::_destroy_scaler()
{
    if( _scaler )
    {
        sws_freeContext( _scaler );
        _scaler = NULL;
    }
}
