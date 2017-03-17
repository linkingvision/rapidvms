
#include "ffkit/av_demuxer.h"

#include "cppkit/os/ck_large_files.h"
#include "cppkit/ck_statistics.h"
#include <utility>

extern "C"
{
#include "libavutil/opt.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

static const size_t DEFAULT_EXTRADATA_BUFFER_SIZE = (1024*256);

av_demuxer::av_demuxer( const ck_string& fileName, bool annexBFilter ) :
    _fileName( fileName ),
    _memoryIOContext( NULL ),
    _storage( make_shared<ck_memory>() ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter) ? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL ),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    memset(&_deMuxPkt, 0, sizeof(_deMuxPkt));
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    _open_streams();
}

av_demuxer::av_demuxer( const uint8_t* buffer,
                        size_t bufferSize,
                        bool annexBFilter ) :
    _fileName(),
    _memoryIOContext( NULL ),
    _storage( make_shared<ck_memory>() ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter)? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL ),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    memset(&_deMuxPkt, 0, sizeof(_deMuxPkt));
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    _open_custom_io_context( buffer, bufferSize );

    _open_streams();
}

av_demuxer::av_demuxer( shared_ptr<cppkit::ck_memory> buffer, bool annexBFilter ) :
    _fileName(),
    _memoryIOContext( NULL ),
    _storage( make_shared<ck_memory>() ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter)? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL ),
    _pf( std::make_shared<av_packet_factory_default>() )
{
    memset(&_deMuxPkt, 0, sizeof(_deMuxPkt));
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    size_t bufferSize = buffer->size_data();

    _open_custom_io_context( buffer->map().get_ptr(), bufferSize );

    _open_streams();
}

av_demuxer::~av_demuxer() throw()
{
    _free_packet();
    _free_filter_packet();

    avformat_close_input( &_context );

    if( _memoryIOContext )
    {
        av_freep( &_memoryIOContext->buffer );
        av_free( _memoryIOContext );
    }

    if( _bsfc )
        av_bitstream_filter_close( _bsfc );
}

ck_string av_demuxer::get_file_name() const
{
    return _fileName;
}

bool av_demuxer::get_stream_types(int streamIndex, 
	struct stream_info &p_stream_info)
{
	std::map<int, struct stream_info>::iterator it = _streamTypes.find(streamIndex), ite = _streamTypes.end();
    if (it == ite)
    {
        return false;
    }
    p_stream_info =  (*it).second;

    return true;
}

int av_demuxer::get_video_stream_index() const
{
    if( _videoStreamIndex != STREAM_TYPE_UNKNOWN )
        return _videoStreamIndex;

    CK_THROW(("Unable to locate video stream!"));
}

int av_demuxer::get_primary_audio_stream_index() const
{
    if( _audioPrimaryStreamIndex != STREAM_TYPE_UNKNOWN )
        return _audioPrimaryStreamIndex;

    CK_THROW(("Unable to locate an audio stream!"));
}

double av_demuxer::get_seconds_between_frames( int streamIndex ) const
{
    const AVCodecContext* codecContext = _context->streams[streamIndex]->codec;

    return (double)codecContext->time_base.num / (double)(codecContext->time_base.den/2);
}

pair<int,int> av_demuxer::get_time_base( int streamIndex ) const
{
    AVCodecContext* codecContext = _context->streams[streamIndex]->codec;

    return make_pair(codecContext->time_base.num, (codecContext->time_base.den/2));
}

bool av_demuxer::read_frame( int& streamIndex )
{
    _free_packet();

    if( av_read_frame( _context, &_deMuxPkt ) >= 0 )
    {
        streamIndex = _deMuxPkt.stream_index;

        _optional_annexb_filter();
    }
    else _eof = true;

    return !_eof;
}

bool av_demuxer::end_of_file() const
{
    return _eof;
}

bool av_demuxer::is_key() const
{
    if( !_eof )
    {
        if( (_deMuxPkt.flags & AV_PKT_FLAG_KEY) )
            return true;
    }

    return false;
}

shared_ptr<av_packet> av_demuxer::get() const
{
    shared_ptr<av_packet> pkt;

    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
    {
        pkt = _pf->get( (size_t)_filterPkt.size );
        pkt->set_data_size( _filterPkt.size );
        memcpy( pkt->map(), _filterPkt.data, _filterPkt.size );
    }
    else
    {
        pkt = _pf->get( (size_t)_deMuxPkt.size );
        pkt->set_data_size( _deMuxPkt.size );
        memcpy( pkt->map(), _deMuxPkt.data, _deMuxPkt.size );
    }

    if( is_key() )
        pkt->set_key( true );

    return std::move( pkt );
}

shared_ptr<ck_memory> av_demuxer::load_file( const cppkit::ck_string& fileName )
{
    shared_ptr<ck_memory> buffer = make_shared<ck_memory>();

    struct ck_file_info fileInfo;
    if( ck_stat( fileName, &fileInfo ) < 0 )
        CK_THROW(("Unable to stat specified file."));

    FILE* inFile = fopen( fileName.c_str(), "rb" );
    if( !inFile )
        CK_THROW(("Unable to open specified file."));

    uint8_t* d = buffer->extend_data( fileInfo.file_size ).get_ptr();

    int itemsRead = fread( d, 1, fileInfo.file_size, inFile );

    fclose( inFile );

    if( itemsRead != fileInfo.file_size )
        CK_THROW(("Failed to read all of the data from the file."));

    return buffer;
}

struct stream_statistics av_demuxer::get_video_stream_statistics( const cppkit::ck_string& fileName )
{
    struct stream_statistics result;
    ck_statistics<average,uint32_t> avgFrameSize;

    uint32_t indexFirstKey = 0;
    bool foundFirstKey = false;
    bool foundGOPSize = false;
    uint32_t currentIndex = 0;

    av_demuxer dm( fileName );

    int videoStreamIndex = dm.get_video_stream_index();

    result.frameRate = (((double)1.0) / dm.get_seconds_between_frames( videoStreamIndex ));
    pair<int,int> tb = dm.get_time_base( videoStreamIndex );
    result.timeBaseNum = tb.first;
    result.timeBaseDen = tb.second;

    int streamIndex = 0;
    while( dm.read_frame( streamIndex ) )
    {
        if( streamIndex != videoStreamIndex )
            continue;

        if( dm.is_key() )
        {
            if( !foundFirstKey )
            {
                indexFirstKey = currentIndex;
                foundFirstKey = true;
            }
            else
            {
                if( !foundGOPSize )
                {
                    result.gopSize = currentIndex - indexFirstKey;
                    foundGOPSize = true;
                }
            }
        }

        shared_ptr<av_packet> pkt = dm.get();
        avgFrameSize.add_sample( pkt->get_data_size() );

        currentIndex++;
    }

    uint32_t avgSize = 0;
    avgFrameSize.get_result( avgSize );

    result.averageBitRate = (uint32_t)((avgSize * (1.0 / dm.get_seconds_between_frames(videoStreamIndex))) * 8);

    result.numFrames = currentIndex;

    return result;
}

void av_demuxer::_open_streams()
{
    if( avformat_open_input( &_context, _fileName.c_str(), NULL, NULL ) < 0 )
        CK_THROW(("Unable to open input file."));

    _streamTypes.clear();

    if( avformat_find_stream_info( _context, NULL ) >= 0 )
    {
        for( int i = 0; i < _context->nb_streams; i++ )
        {
            if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_UNKNOWN )
            {
                struct stream_info st = { STREAM_TYPE_UNKNOWN, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO )
            {
                struct stream_info st = { STREAM_TYPE_VIDEO, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;

                if( _videoStreamIndex == STREAM_TYPE_UNKNOWN )
                    _videoStreamIndex = i;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO )
            {
                struct stream_info st = { STREAM_TYPE_AUDIO, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;

                if( _audioPrimaryStreamIndex == STREAM_TYPE_UNKNOWN )
                    _audioPrimaryStreamIndex = i;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_DATA )
            {
                struct stream_info st = { STREAM_TYPE_DATA, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_SUBTITLE )
            {
                struct stream_info st = { STREAM_TYPE_SUBTITLE, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_ATTACHMENT )
            {
                struct stream_info st = { STREAM_TYPE_ATTACHMENT, i , 
					_context->streams[i]->codec->codec_id, 
					_context->streams[i]->codec->width,
					_context->streams[i]->codec->height};
                _streamTypes[i] = st;
            }
        }
    }
    else CK_THROW(("Unable to locate stream info."));

    if( _videoStreamIndex == -1 )
        CK_THROW(("Failed to find video stream in file."));

    AVCodecContext* codecContext = _context->streams[_videoStreamIndex]->codec;

    _secondsBetweenFrames = (double)codecContext->time_base.num / (double)(codecContext->time_base.den/2);

    _deMuxPkt.size = 0;
}

void av_demuxer::_open_custom_io_context( const uint8_t* buffer, size_t bufferSize )
{
    memcpy( _storage->extend_data( bufferSize ).get_ptr(), buffer, bufferSize );

    _memoryIOContext = avio_alloc_context( (uint8_t*)av_malloc( bufferSize ), bufferSize, 0, this, _read, NULL, _seek );
    if( !_memoryIOContext )
        CK_THROW(("Unable to allocate IO context."));

    _context = avformat_alloc_context();
    if( !_context )
        CK_THROW(("Unable to allocate input IO context."));

    _context->pb = _memoryIOContext;
}

int av_demuxer::_read( void* opaque, uint8_t* dest, int size )
{
    av_demuxer* obj = (av_demuxer*)opaque;

    if( (obj->_pos + size) > obj->_storage->size_data() )
        size = obj->_storage->size_data() - obj->_pos;

    memcpy( dest, obj->_storage->map().get_ptr() + obj->_pos, size );
    obj->_pos += size;

    return size;
}

int64_t av_demuxer::_seek( void* opaque, int64_t offset, int whence )
{
    av_demuxer* obj = (av_demuxer*)opaque;

    if( whence == SEEK_CUR )
    {
        offset += obj->_pos;
        obj->_pos = offset;
        return obj->_pos;
    }
    else if( whence == SEEK_END )
    {
        offset += obj->_storage->size_data();
        obj->_pos = offset;
        return obj->_pos;
    }
    else if( whence == SEEK_SET )
    {
        obj->_pos = offset;
        return obj->_pos;
    }

    return -1;
}

void av_demuxer::_free_packet()
{
    if( _deMuxPkt.size > 0 )
    {
        //av_free( _deMuxPkt.data );
        //_deMuxPkt.data = NULL;
        //_deMuxPkt.size = 0;
	av_free_packet(&_deMuxPkt);
    }
}

void av_demuxer::_free_filter_packet()
{
    if( _filterPkt.size > 0 )
    {
        av_free( _filterPkt.data );
        _filterPkt.data = NULL;
        _filterPkt.size = 0;
    }
}

void av_demuxer::_optional_annexb_filter()
{
    _free_filter_packet();

    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
    {
        // If the return value is 0, the output buffer is not allocated and should be considered identical to the
        // input buffer, or in case *poutbuf was set it points to the input buffer (not necessarily to its starting
        // address).

        av_bitstream_filter_filter( _bsfc, _context->streams[_videoStreamIndex]->codec, NULL,
                                    &_filterPkt.data, &_filterPkt.size,
                                    _deMuxPkt.data, _deMuxPkt.size,
                                    (is_key()) ? 1 : 0 );

        // Given the super tricky comment above (lifted right from the ffmpeg documentation!) our strategy is this:
        // If our output buffer overlaps our input, then allocate storage for it, copy the data into it and reset
        // our _filterPkt.data and _filterPkt.size to point to it. This way, we can always guarantee our _deMuxPkt
        // and out _filterPkt are distinct.

        if( _filterPkt.data >= _deMuxPkt.data && _filterPkt.data < (_deMuxPkt.data + _deMuxPkt.size) )
        {
            size_t filteredSize = _deMuxPkt.size - (_filterPkt.data - _deMuxPkt.data);
            uint8_t* data = (uint8_t*)av_malloc( filteredSize );
            if( !data )
                CK_THROW(("Unable to allocate filter packet."));
            memcpy( data, _filterPkt.data, filteredSize );
            _filterPkt.data = data;
            _filterPkt.size = filteredSize;
        }
    }
}
