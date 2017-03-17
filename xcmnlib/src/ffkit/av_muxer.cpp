
#include "ffkit/av_muxer.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

av_muxer::av_muxer( const struct codec_options& options,
                    const ck_string& fileName,
                    OUTPUT_LOCATION location ) :
    _options( options ),
    _fileName( fileName ),
    _context( NULL ),
    _stream( NULL ),
    _location( location ),
    _ts( 0 ),
    _oweTrailer( false ),
    _numVideoFramesWritten( 0 ),
    _isTS( _fileName.to_lower().ends_with( ".ts" ) ),
    _fileNum( 0 )
    //_pf( std::make_shared<av_packet_factory_default>() )
{
    _context = avformat_alloc_context();
    if( !_context )
        CK_THROW(("Unable to allocate output format context."));

    _context->oformat = av_guess_format( NULL, _fileName.c_str(), NULL );

    if( !_context->oformat )
        CK_THROW(("Unable to guess output format."));

    _context->oformat->video_codec = AV_CODEC_ID_H264;

    _stream = avformat_new_stream( _context, NULL );
    if( !_stream )
        CK_THROW(("Unable to allocate output stream."));

    avcodec_get_context_defaults3( _stream->codec, NULL );

    _stream->codec->codec_id = AV_CODEC_ID_H264;
    _stream->codec->codec_type = AVMEDIA_TYPE_VIDEO;

    apply_codec_options( options );

    if( !_options.gop_size.is_null() )
        _stream->codec->gop_size = _options.gop_size.value();
    else CK_THROW(("Required option missing: gop_size"));

    _stream->codec->pix_fmt = AV_PIX_FMT_YUV420P;

    if( _context->oformat->flags & AVFMT_GLOBALHEADER )
        _stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
}

av_muxer::~av_muxer() throw()
{
    if( !_isTS && _oweTrailer )
        av_write_trailer( _context );

    _close_io();

    if( _stream->codec->extradata )
        av_freep( &_stream->codec->extradata );

    avformat_free_context( _context );
}

ck_string av_muxer::get_file_name() const
{
    return _fileName;
}

void av_muxer::set_extra_data( std::shared_ptr<cppkit::ck_memory> extraData )
{
    if( !(_context->oformat->flags & AVFMT_GLOBALHEADER) )
        CK_LOG_INFO("Extradata not required for %s container.",_fileName.c_str());
    else
    {
        _stream->codec->extradata = (uint8_t*)av_mallocz( extraData->size_data() );
        if( !_stream->codec->extradata )
            CK_THROW(("Unable to allocate extradata storage."));
        _stream->codec->extradata_size = extraData->size_data();

        memcpy( _stream->codec->extradata, extraData->map().get_ptr(), extraData->size_data() );
    }
}

void av_muxer::write_video_packet( av_packet * input, bool keyFrame )
{
    if( _context->pb == NULL )
        _open_io();

    if( _isTS )
    {
        if( _numVideoFramesWritten == 0 )
        {
            if( _fileNum == 0 )
            {
                if( avformat_write_header( _context, NULL ) < 0 )
                    CK_THROW(("Unable to write header to container."));
            }

            av_opt_set( _context->priv_data, "mpegts_flags", "resend_headers", 0 );
        }
    }
    else
    {
        if( !_oweTrailer )
        {
			if (avformat_write_header(_context, NULL) < 0)
			{
				CK_THROW(("Unable to write header to container."));
			}

            _oweTrailer = true;
        }
    }

    AVPacket pkt;
    av_init_packet( &pkt );

    pkt.stream_index = _stream->index;
    pkt.data = input->map();
    pkt.size = input->get_data_size();

    pkt.pts = _ts;
    pkt.dts = _ts;

    _ts += av_rescale_q(1, _stream->codec->time_base, _stream->time_base);

    pkt.flags |= (keyFrame) ? AV_PKT_FLAG_KEY : 0;

    if( av_interleaved_write_frame( _context, &pkt ) < 0 )
        CK_THROW(("Unable to write video frame."));
   pkt.data = NULL;
   pkt.size = 0;
   av_free_packet(&pkt);
    _numVideoFramesWritten++;
}

void av_muxer::flush()
{
    if( av_interleaved_write_frame( _context, NULL ) < 0 )
        CK_THROW(("Unable to flush av_muxer."));
}

//void av_muxer::finalize_buffer( std::shared_ptr<cppkit::ck_memory> buffer )

void av_muxer::finalize_buffer( cppkit::ck_memory* buffer )
{
    if( _location != OUTPUT_LOCATION_BUFFER )
        CK_THROW(("Unable to finalize a non buffer IO object."));

    _finalize_common();

    uint8_t* fileBytes = NULL;
    int fileSize = avio_close_dyn_buf( _context->pb, &fileBytes );
    _context->pb = NULL;

    if( fileBytes == NULL || fileSize == 0 )
        CK_THROW(("Unable to finalize empty buffer."));

    buffer->set_data_size( fileSize );

    memcpy( buffer->map().get_ptr(), fileBytes, fileSize );

    av_freep( &fileBytes );
}

void av_muxer::finalize_file()
{
    if( _location != OUTPUT_LOCATION_FILE )
        CK_THROW(("Unable to finalize a non file IO object."));

    _finalize_common();

    avio_close( _context->pb );
    _context->pb = NULL;
}

void av_muxer::apply_codec_options( const struct codec_options& options )
{
    _options = options;

    if( !_options.profile.is_null() )
    {
        if( _options.profile.value().to_lower() == "baseline" )
            _stream->codec->profile = FF_PROFILE_H264_BASELINE;
        else if( _options.profile.value().to_lower() == "main" )
            _stream->codec->profile = FF_PROFILE_H264_MAIN;
        else if( _options.profile.value().to_lower() == "high" )
            _stream->codec->profile = FF_PROFILE_H264_HIGH;

        av_opt_set( _stream->codec->priv_data, "profile", _options.profile.value().to_lower().c_str(), 0 );
    }

    if( !_options.bit_rate.is_null() )
        _stream->codec->bit_rate = _options.bit_rate.value();
    else CK_THROW(("Required option missing: bit_rate"));

    if( !_options.width.is_null() )
        _stream->codec->width = _options.width.value();
    else CK_THROW(("Required option missing: width"));

    if( !_options.height.is_null() )
        _stream->codec->height = _options.height.value();
    else CK_THROW(("Required option missing: height"));

    if( !_options.time_base_num.is_null() )
        _stream->codec->time_base.num = _options.time_base_num.value();
    else CK_THROW(("Required option missing: time_base_num"));

    if( !_options.time_base_den.is_null() )
        _stream->codec->time_base.den = _options.time_base_den.value();
    else CK_THROW(("Required option missing: time_base_den"));
}

void av_muxer::_finalize_common()
{
    if( !_context->pb )
        CK_THROW(("Unable to finalize an unopened IO object."));

    if( !_isTS && _oweTrailer )
    {
        av_write_trailer( _context );
        _oweTrailer = false;
    }

    _fileNum++;

    _numVideoFramesWritten = 0;
}

void av_muxer::_open_io()
{
    if( _context->pb == NULL )
    {
        if( _location == OUTPUT_LOCATION_BUFFER )
        {
            avio_open_dyn_buf( &_context->pb );
            if( !_context->pb )
                CK_THROW(("Unable to allocate a memory IO object."));
        }
        else
        {
            if( avio_open( &_context->pb, _fileName.c_str(), AVIO_FLAG_WRITE ) < 0 )
                CK_THROW(("Unable to open output file."));
        }
    }
}

void av_muxer::_close_io()
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
