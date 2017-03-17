
#include "ffkit/jpeg_encoder.h"

#include "cppkit/ck_exception.h"

using namespace ffkit;
using namespace cppkit;
using namespace std;

static const size_t DEFAULT_JPEG_ENCODE_BUFFER_SIZE = (1024*1024);

jpeg_encoder::jpeg_encoder( const struct codec_options& options, int encodeAttempts ) :
    _codec( avcodec_find_encoder( AV_CODEC_ID_MJPEG ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _encodeAttempts( encodeAttempts ),
    _output(),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    if( !_codec )
        CK_THROW(("Unable to locate MJPEG codec."));

    if( !_context )
        CK_THROW(("Unable to allocate MJPEG codec context."));

    _context->codec_id = AV_CODEC_ID_MJPEG;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;

    _context->time_base.num = 1;
    _context->time_base.den = 15;

	_context->pix_fmt = AV_PIX_FMT_YUVJ420P;
    _context->color_range = AVCOL_RANGE_JPEG;

    _context->gop_size = 1;
    _context->bit_rate_tolerance = (int)((double)10000000 *
                                         ((double)_context->time_base.num / (double)_context->time_base.den));
    _context->bit_rate_tolerance += 1; // Make sure we pass the comparison check

    if( !_options.width.is_null() )
        _context->width = _options.width.value();
    else CK_THROW(("Required option missing: width"));

    if( !_options.height.is_null() )
        _context->height = _options.height.value();
    else CK_THROW(("Required option missing: height"));

    if( !_options.bit_rate.is_null() )
        _context->bit_rate = _options.bit_rate.value();
    else CK_THROW(("Required option missing: bit_rate"));

    if( !_options.qmin.is_null() )
        _context->qmin = _options.qmin.value();

    if( !_options.qmax.is_null() )
        _context->qmax = _options.qmax.value();

    if( !_options.max_qdiff.is_null() )
        _context->max_qdiff = _options.max_qdiff.value();

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
        CK_THROW(("Failed to open encoding context."));
}

jpeg_encoder::~jpeg_encoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

void jpeg_encoder::encode_yuv420p( shared_ptr<av_packet> input )
{
    AVFrame frame;
	av_frame_unref(&frame);

    _output = _pf->get( DEFAULT_JPEG_ENCODE_BUFFER_SIZE );

    uint8_t* pic = input->map();

    frame.data[0] = pic;
    pic += (_context->width * _context->height);
    frame.data[1] = pic;
    pic += ((_context->width/4) * _context->height);
    frame.data[2] = pic;

    frame.linesize[0] = _context->width;
    frame.linesize[1] = (_context->width/2);
    frame.linesize[2] = (_context->width/2);

    int attempt = 0;
    int gotPacket = 0;
    AVPacket pkt;

    do
    {
        av_init_packet( &pkt );
        pkt.data = _output->map();
        pkt.size = _output->get_buffer_size();

        if( avcodec_encode_video2( _context,
                                   &pkt,
                                   &frame,
                                   &gotPacket ) < 0 )
            CK_THROW(("Error while encoding."));

        attempt++;

    } while( gotPacket == 0 && (attempt < _encodeAttempts) );

    _output->set_data_size( pkt.size );
}

shared_ptr<av_packet> jpeg_encoder::get()
{
    return std::move( _output );
}

void jpeg_encoder::write_jpeg_file( const cppkit::ck_string& fileName, shared_ptr<av_packet> jpeg )
{
    FILE* outFile = fopen( fileName.c_str(), "wb" );
    if( !outFile )
        CK_THROW(("Unable to open output file."));

    fwrite( jpeg->map(), 1, jpeg->get_data_size(), outFile );

    fclose( outFile );
}
