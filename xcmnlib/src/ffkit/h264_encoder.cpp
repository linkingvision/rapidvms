
#include "ffkit/h264_encoder.h"

#include "cppkit/ck_exception.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

static const size_t DEFAULT_PADDING = 16;
static const size_t DEFAULT_ENCODE_BUFFER_SIZE = (1024*1024);
static const size_t DEFAULT_EXTRADATA_BUFFER_SIZE = (1024*256);

h264_encoder::h264_encoder( const struct codec_options& options,
                            bool annexB,
                            int encodeAttempts ) :
    _codec( avcodec_find_encoder( AV_CODEC_ID_H264 ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _numTillKey( 0 ),
    _encodeAttempts( encodeAttempts ),
    _extraData( DEFAULT_EXTRADATA_BUFFER_SIZE ),
    _lastWasKey( false ),
    _annexB( annexB ),
    _pf( std::make_shared<av_packet_factory_default>() ),
    _output()
{
    if( !_codec )
        CK_THROW(("Unable to locate H264 codec."));

    if( !_context )
        CK_THROW(("Unable to allocate H264 codec context."));

    _context->codec_id = AV_CODEC_ID_H264;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;
    _context->pix_fmt = AV_PIX_FMT_YUV420P;

    if( !_options.gop_size.is_null() )
        _context->gop_size = _options.gop_size.value();

    if( !_options.bit_rate.is_null() )
        _context->bit_rate = _options.bit_rate.value();
    else CK_THROW(("Required option missing: bit_rate"));

    if( !_options.width.is_null() )
        _context->width = _options.width.value();
    else CK_THROW(("Required option missing: width"));

    if( !_options.height.is_null() )
        _context->height = _options.height.value();
    else CK_THROW(("Required option missing: height"));

    if( !_options.time_base_num.is_null() )
        _context->time_base.num = _options.time_base_num.value();
    else CK_THROW(("Required option missing: time_base.num"));

    if( !_options.time_base_den.is_null() )
        _context->time_base.den = _options.time_base_den.value();
    else CK_THROW(("Required option missing: time_base.den"));

    if( !_options.me_method.is_null() )
        _context->me_method = _options.me_method.value();

    if( !_options.me_subpel_quality.is_null() )
        _context->me_subpel_quality = _options.me_subpel_quality.value();

    if( !_options.delay.is_null() )
        _context->delay = _options.delay.value();

    if( !_options.thread_count.is_null() )
    {
        _context->thread_count = _options.thread_count.value();
        _context->thread_type = FF_THREAD_FRAME;
    }

    if( !_options.refs.is_null() )
        _context->refs = _options.refs.value();

    if( !_options.qmin.is_null() )
        _context->qmin = _options.qmin.value();

    if( !_options.qmax.is_null() )
        _context->qmax = _options.qmax.value();

    if( !_options.max_qdiff.is_null() )
        _context->max_qdiff = _options.max_qdiff.value();

    if( !_options.rc_buffer_size.is_null() )
        _context->rc_buffer_size = _options.rc_buffer_size.value();

    if( !_options.profile.is_null() )
    {
        if( _options.profile.value().to_lower() == "baseline" )
            _context->profile = FF_PROFILE_H264_BASELINE;
        else if( _options.profile.value().to_lower() == "main" )
            _context->profile = FF_PROFILE_H264_MAIN;
        else if( _options.profile.value().to_lower() == "high" )
            _context->profile = FF_PROFILE_H264_HIGH;

        av_opt_set( _context->priv_data, "profile", _options.profile.value().to_lower().c_str(), 0 );
    }

    if( !_options.preset.is_null() )
        av_opt_set( _context->priv_data, "preset", _options.preset.value().c_str(), 0 );

    if( !_options.tune.is_null() )
        av_opt_set( _context->priv_data, "tune", _options.tune.value().c_str(), 0 );

    if( !_options.x264opts.is_null() )
        av_opt_set( _context->priv_data, "x264opts", _options.x264opts.value().c_str(), 0 );

    // We set this here to force FFMPEG to populate the extradata field (necessary if the output stream
    // is going to be muxed into a format with a global header (for example, mp4)).
    _context->flags |= CODEC_FLAG_GLOBAL_HEADER;

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        CK_THROW(("Failed to open encoding context."));

    if( _context->extradata == NULL || _context->extradata_size == 0 )
        CK_THROW(("No codec extradata available!"));

    memcpy( _extraData.extend_data( _context->extradata_size ).get_ptr(), _context->extradata, _context->extradata_size );
}

h264_encoder::~h264_encoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

void h264_encoder::encode_yuv420p( shared_ptr<av_packet> input, encoder_frame_type type )
{
    AVFrame frame;
	av_frame_unref(&frame);

    uint8_t* pic = input->map();

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

    _output = _pf->get( DEFAULT_ENCODE_BUFFER_SIZE );

    int attempts = 0;
    int gotPacket = 0;
    AVPacket pkt;

    do
    {
        uint8_t* dest = _output->map();

        if( _annexB )
        {
            memcpy( dest, _context->extradata, _context->extradata_size );
            dest += _context->extradata_size;
        }

        av_init_packet( &pkt );
        pkt.data = dest;
        pkt.size = (_annexB) ? _output->get_buffer_size() - _context->extradata_size : _output->get_buffer_size();

        if( avcodec_encode_video2( _context,
                                   &pkt,
                                   &frame,
                                   &gotPacket ) < 0 )
            CK_THROW(("Error while encoding."));

        attempts++;

    } while( gotPacket == 0 && (attempts < _encodeAttempts) );

    if( pkt.flags & AV_PKT_FLAG_KEY )
        _lastWasKey = true;
    else _lastWasKey = false;

    _output->set_data_size( (_annexB) ? pkt.size + _context->extradata_size : pkt.size );
}

shared_ptr<av_packet> h264_encoder::get()
{
    return std::move( _output );
}

#if 0
size_t h264_encoder::encode_yuv420p( uint8_t* pic, uint8_t* output, size_t outputSize,
                                     encoder_frame_type type )
{
    AVFrame frame;
    avcodec_get_frame_defaults( &frame );

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
            CK_THROW(("Error while encoding."));

        attempts++;

    } while( gotPacket == 0 && (attempts < _encodeAttempts) );

    if( pkt.flags & AV_PKT_FLAG_KEY )
        _lastWasKey = true;
    else _lastWasKey = false;

    return (_annexB) ? pkt.size + _context->extradata_size : pkt.size;
}

shared_ptr<ck_memory> h264_encoder::encode_yuv420p( shared_ptr<ck_memory> pic,
                                                    encoder_frame_type type )
{
    shared_ptr<ck_memory> frame = make_shared<ck_memory>( DEFAULT_ENCODE_BUFFER_SIZE + DEFAULT_PADDING );

    uint8_t* p = frame->extend_data( DEFAULT_ENCODE_BUFFER_SIZE ).get_ptr();

    size_t outputSize = encode_yuv420p( pic->map().get_ptr(),
                                        p,
                                        frame->size_data(),
                                       type );
    frame->set_data_size( outputSize );

    return frame;
}
#endif

struct codec_options h264_encoder::get_options() const
{
    return _options;
}

shared_ptr<ck_memory> h264_encoder::get_extra_data() const
{
    shared_ptr<ck_memory> ed = make_shared<ck_memory>( DEFAULT_EXTRADATA_BUFFER_SIZE );

    memcpy( ed->extend_data( _extraData.size_data() ).get_ptr(), _extraData.map().get_ptr(), _extraData.size_data() );

    return ed;
}
