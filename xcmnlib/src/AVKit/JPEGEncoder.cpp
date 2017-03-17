
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/JPEGEncoder.h"

#include "XSDK/XException.h"

using namespace AVKit;
using namespace XSDK;

JPEGEncoder::JPEGEncoder( const struct CodecOptions& options, int encodeAttempts ) :
    _codec( avcodec_find_encoder( AV_CODEC_ID_MJPEG ) ),
    _context( avcodec_alloc_context3( _codec ) ),
    _options( options ),
    _encodeAttempts( encodeAttempts )
{
    if( !_codec )
        printf(("Unable to locate MJPEG codec."));

    if( !_context )
        printf(("Unable to allocate MJPEG codec context."));

	_context->codec_id = AV_CODEC_ID_MJPEG;
    _context->codec_type = AVMEDIA_TYPE_VIDEO;

    _context->time_base.num = 1;
    _context->time_base.den = 25;

	_context->pix_fmt = AV_PIX_FMT_YUVJ420P;
    _context->color_range = AVCOL_RANGE_JPEG;

    _context->gop_size = 1;
    //_context->bit_rate_tolerance = (int)((double)10000000 *
    //                                     ((double)_context->time_base.num / (double)_context->time_base.den));
    //_context->bit_rate_tolerance += 1; // Make sure we pass the comparison check

    if( !_options.width.IsNull() )
        _context->width = _options.width.Value();
    else printf(("Required option missing: width"));

    if( !_options.height.IsNull() )
        _context->height = _options.height.Value();
    else printf(("Required option missing: height"));

    if( !_options.bit_rate.IsNull() )
        _context->bit_rate = _options.bit_rate.Value();
    else printf(("Required option missing: bit_rate"));

    if( !_options.qmin.IsNull() )
        _context->qmin = _options.qmin.Value();

    if( !_options.qmax.IsNull() )
        _context->qmax = _options.qmax.Value();

    if( !_options.max_qdiff.IsNull() )
        _context->max_qdiff = _options.max_qdiff.Value();

    if( avcodec_open2( _context, _codec, NULL ) < 0 )
	{
		printf("Failed to open encoding context.\n");
	}
}

JPEGEncoder::~JPEGEncoder() throw()
{
    avcodec_close( _context );

    av_free( _context );
}

size_t JPEGEncoder::EncodeYUV420P( uint8_t* picy, uint8_t* picu, 
	uint8_t* picv, uint8_t* output, size_t outputSize )
{
    AVFrame frame;
    memset(&frame, 0, sizeof(frame));
    av_frame_unref(&frame);
int size = avpicture_get_size(AV_PIX_FMT_YUVJ420P ,_context->width, _context->height);
uint8_t *buffer = (uint8_t*)av_malloc(size*sizeof(uint8_t));
avpicture_fill((AVPicture*)&frame, buffer, _context->pix_fmt ,_context->width, _context->height);

    frame.format = AV_PIX_FMT_YUVJ420P;

    frame.width = _context->width;
    frame.height= _context->height;

    frame.data[0] = picy;
    frame.data[1] = picu;
    frame.data[2] = picv;

    //frame.linesize[0] = _context->width;
    //frame.linesize[1] = _context->width/2;
    //frame.linesize[2] = _context->width/2;

    int attempt = 0;
    int gotPacket = 0;
    AVPacket pkt;
	int y_size;
	//y_size = _context->width * _context->height;
	//av_new_packet(&pkt,y_size*3);

    do
    {
        av_init_packet( &pkt );
        pkt.data = output;
        //pkt.data = (uint8_t*)av_malloc(outputSize);
        pkt.size = outputSize;

		int ret = avcodec_encode_video2(_context,
			&pkt,
			&frame,
			&gotPacket);

        if(ret < 0 )
            printf(("Error while encoding."));

        attempt++;

    } while( gotPacket == 0 && (attempt < _encodeAttempts) );

    return pkt.size;
}


void JPEGEncoder::WriteJPEGFile( const XSDK::XString& fileName, XIRef<XSDK::XMemory> jpeg )
{
    FILE* outFile = fopen( fileName.c_str(), "wb" );
    if( !outFile )
    {
        printf(("Unable to open output file."));
	  return;
    }

    fwrite( jpeg->Map(), 1, jpeg->GetDataSize(), outFile );

    fclose( outFile );
}
