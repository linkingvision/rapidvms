
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "AVKit/AVDeMuxer.h"

#include "XSDK/LargeFiles.h"
#include "XSDK/XStatistics.h"

extern "C"
{
#include "libavutil/opt.h"
}

using namespace AVKit;
using namespace XSDK;
using namespace std;

static const size_t DEFAULT_EXTRADATA_BUFFER_SIZE = (1024*256);

AVDeMuxer::AVDeMuxer( const XString& fileName, bool annexBFilter ) :
    _fileName( fileName ),
    _memoryIOContext( NULL ),
    _storage( new XMemory ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter) ? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL ),
    _firstFrame( true )
{
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    _OpenStreams();
}

AVDeMuxer::AVDeMuxer( const uint8_t* buffer,
                      size_t bufferSize,
                      bool annexBFilter ) :
    _fileName(),
    _memoryIOContext( NULL ),
    _storage( new XMemory ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter)? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL )
{
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    _OpenCustomIOContext( buffer, bufferSize );

    _OpenStreams();
}

AVDeMuxer::AVDeMuxer( XIRef<XSDK::XMemory> buffer, bool annexBFilter ) :
    _fileName(),
    _memoryIOContext( NULL ),
    _storage( new XMemory ),
    _pos( 0 ),
    _context( NULL ),
    _eof( false ),
    _deMuxPkt(),
    _filterPkt(),
    _streamTypes(),
    _videoStreamIndex( STREAM_TYPE_UNKNOWN ),
    _audioPrimaryStreamIndex( STREAM_TYPE_UNKNOWN ),
    _bsfc( (annexBFilter)? av_bitstream_filter_init( "h264_mp4toannexb" ) : NULL )
{
    _deMuxPkt.size = 0;
    _deMuxPkt.data = NULL;
    _filterPkt.size = 0;
    _filterPkt.data = NULL;

    size_t bufferSize = buffer->GetDataSize();

    _OpenCustomIOContext( buffer->Map(), bufferSize );

    _OpenStreams();
}

AVDeMuxer::~AVDeMuxer() throw()
{
    _FreePacket();
    _FreeFilterPacket();

    avformat_close_input( &_context );

    if( _memoryIOContext )
    {
        av_freep( &_memoryIOContext->buffer );
        av_free( _memoryIOContext );
    }

    if( _bsfc )
        av_bitstream_filter_close( _bsfc );
}

XString AVDeMuxer::GetFileName() const
{
    return _fileName;
}

vector<StreamInfo> AVDeMuxer::GetStreamTypes() const
{
    return _streamTypes;
}

int AVDeMuxer::GetVideoStreamIndex() const
{
    if( _videoStreamIndex != STREAM_TYPE_UNKNOWN )
        return _videoStreamIndex;

    X_THROW(("Unable to locate video stream!"));
}

int AVDeMuxer::GetPrimaryAudioStreamIndex() const
{
    if( _audioPrimaryStreamIndex != STREAM_TYPE_UNKNOWN )
        return _audioPrimaryStreamIndex;

    X_THROW(("Unable to locate an audio stream!"));
}

double AVDeMuxer::GetSecondsBetweenFrames( int streamIndex ) const
{
    const AVCodecContext* codecContext = _context->streams[streamIndex]->codec;

    return (double)codecContext->time_base.num / (double)(codecContext->time_base.den/2);
}

pair<int,int> AVDeMuxer::GetTimeBase( int streamIndex ) const
{
    AVCodecContext* codecContext = _context->streams[streamIndex]->codec;

    return make_pair(codecContext->time_base.num, (codecContext->time_base.den/2));
}

bool AVDeMuxer::ReadFrame( int& streamIndex )
{
    _FreePacket();

    if( av_read_frame( _context, &_deMuxPkt ) >= 0 )
    {
        streamIndex = _deMuxPkt.stream_index;

        _OptionalAnnexBFilter();
    }
    else _eof = true;

    return !_eof;
}

bool AVDeMuxer::EndOfFile() const
{
    return _eof;
}

bool AVDeMuxer::IsKey() const
{
    if( !_eof )
    {
        if( (_deMuxPkt.flags & AV_PKT_FLAG_KEY) )
            return true;
    }

    return false;
}

size_t AVDeMuxer::GetFrameSize() const
{
    // If we have bitstream filtering turned on and the currently demuxed packet was from the
    // video stream, use the filtered packet.
    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
        return (size_t)_filterPkt.size;
    else return (size_t)_deMuxPkt.size;
}

void AVDeMuxer::GetFrame( uint8_t* dest ) const
{
    // If we have bitstream filtering turned on and the currently demuxed packet was from the
    // video stream, use the filtered packet.
    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
        memcpy( dest, _filterPkt.data, _filterPkt.size );
    else memcpy( dest, _deMuxPkt.data, _deMuxPkt.size );
}

AVFormatContext* AVDeMuxer::GetFormatContext() const
{
    return _context;
}

XIRef<XMemory> AVDeMuxer::GetFrame() const
{
    XIRef<XMemory> frame = new XMemory;
    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
        frame->ResizeData( _filterPkt.size );
    else frame->ResizeData( _deMuxPkt.size );
    GetFrame( frame->Map() );
    return frame;
}

XIRef<XMemory> AVDeMuxer::LoadFile( const XSDK::XString& fileName )
{
    XIRef<XMemory> buffer = new XMemory;

    struct x_file_info fileInfo;
    if( x_stat( fileName, &fileInfo ) < 0 )
        X_THROW(("Unable to stat specified file."));

    FILE* inFile = fopen( fileName.c_str(), "rb" );
    if( !inFile )
        X_THROW(("Unable to open specified file."));

    uint8_t* d = &buffer->Extend( fileInfo._fileSize );

    int itemsRead = fread( d, 1, fileInfo._fileSize, inFile );

    fclose( inFile );

    if( itemsRead != fileInfo._fileSize )
        X_THROW(("Failed to read all of the data from the file."));

    return buffer;
}

struct StreamStatistics AVDeMuxer::GetVideoStreamStatistics( const XSDK::XString& fileName )
{
    struct StreamStatistics result;
    XStatistics<Average,uint32_t> avgFrameSize;

    uint32_t indexFirstKey = 0;
    bool foundFirstKey = false;
    bool foundGOPSize = false;
    uint32_t currentIndex = 0;

    AVDeMuxer dm( fileName );

    int videoStreamIndex = dm.GetVideoStreamIndex();

    result.frameRate = (((double)1.0) / dm.GetSecondsBetweenFrames( videoStreamIndex ));
    pair<int,int> tb = dm.GetTimeBase( videoStreamIndex );
    result.timeBaseNum = tb.first;
    result.timeBaseDen = tb.second;

    int streamIndex = 0;
    while( dm.ReadFrame( streamIndex ) )
    {
        if( streamIndex != videoStreamIndex )
            continue;

        if( dm.IsKey() )
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

        avgFrameSize.AddSample( dm.GetFrameSize() );

        currentIndex++;
    }

    uint32_t avgSize = 0;
    avgFrameSize.GetResult( avgSize );

    result.averageBitRate = (uint32_t)((avgSize * (1.0 / dm.GetSecondsBetweenFrames(videoStreamIndex))) * 8);

    result.numFrames = currentIndex;

    return result;
}

void AVDeMuxer::_OpenStreams()
{
    if( avformat_open_input( &_context, _fileName.c_str(), NULL, NULL ) < 0 )
        X_THROW(("Unable to open input file."));

    _streamTypes.clear();

    if( avformat_find_stream_info( _context, NULL ) >= 0 )
    {
        for( uint32_t i = 0; i < _context->nb_streams; i++ )
        {
            if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_UNKNOWN )
            {
                struct StreamInfo st = { STREAM_TYPE_UNKNOWN, i };
                _streamTypes.push_back( st );
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO )
            {
                struct StreamInfo st = { STREAM_TYPE_VIDEO, i };
                _streamTypes.push_back( st );

                if( _videoStreamIndex == STREAM_TYPE_UNKNOWN )
                    _videoStreamIndex = i;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO )
            {
                struct StreamInfo st = { STREAM_TYPE_AUDIO, i };
                _streamTypes.push_back( st );

                if( _audioPrimaryStreamIndex == STREAM_TYPE_UNKNOWN )
                    _audioPrimaryStreamIndex = i;
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_DATA )
            {
                struct StreamInfo st = { STREAM_TYPE_DATA, i };
                _streamTypes.push_back( st );
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_SUBTITLE )
            {
                struct StreamInfo st = { STREAM_TYPE_SUBTITLE, i };
                _streamTypes.push_back( st );
            }
            else if( _context->streams[i]->codec->codec_type == AVMEDIA_TYPE_ATTACHMENT )
            {
                struct StreamInfo st = { STREAM_TYPE_ATTACHMENT, i };
                _streamTypes.push_back( st );
            }
        }
    }
    else X_THROW(("Unable to locate stream info."));

    if( _videoStreamIndex == -1 )
        X_THROW(("Failed to find video stream in file."));

    AVCodecContext* codecContext = _context->streams[_videoStreamIndex]->codec;

    _secondsBetweenFrames = (double)codecContext->time_base.num / (double)(codecContext->time_base.den/2);

    _deMuxPkt.size = 0;
}

void AVDeMuxer::_OpenCustomIOContext( const uint8_t* buffer, size_t bufferSize )
{
    memcpy( &_storage->Extend( bufferSize ), buffer, bufferSize );

    _memoryIOContext = avio_alloc_context( (uint8_t*)av_malloc( bufferSize ), bufferSize, 0, this, _Read, NULL, _Seek );
    if( !_memoryIOContext )
        X_THROW(("Unable to allocate IO context."));

    _context = avformat_alloc_context();
    if( !_context )
        X_THROW(("Unable to allocate input IO context."));

    _context->pb = _memoryIOContext;
}

int AVDeMuxer::_Read( void* opaque, uint8_t* dest, int size )
{
    AVDeMuxer* obj = (AVDeMuxer*)opaque;

    if( (obj->_pos + size) > obj->_storage->GetDataSize() )
        size = obj->_storage->GetDataSize() - obj->_pos;

    memcpy( dest, obj->_storage->Map() + obj->_pos, size );
    obj->_pos += size;

    return size;
}

int64_t AVDeMuxer::_Seek( void* opaque, int64_t offset, int whence )
{
    AVDeMuxer* obj = (AVDeMuxer*)opaque;

    if( whence == SEEK_CUR )
    {
        offset += obj->_pos;
        obj->_pos = offset;
        return obj->_pos;
    }
    else if( whence == SEEK_END )
    {
        offset += obj->_storage->GetDataSize();
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

void AVDeMuxer::_FreePacket()
{
    if( _deMuxPkt.size > 0 )
    {
        av_free( _deMuxPkt.data );
        _deMuxPkt.data = NULL;
        _deMuxPkt.size = 0;
    }
}

void AVDeMuxer::_FreeFilterPacket()
{
    if( _filterPkt.size > 0 )
    {
        av_free( _filterPkt.data );
        _filterPkt.data = NULL;
        _filterPkt.size = 0;
    }
}

void AVDeMuxer::_OptionalAnnexBFilter()
{
    _FreeFilterPacket();

    if( _bsfc && (_deMuxPkt.stream_index == _videoStreamIndex) )
    {
        // If the return value is 0, the output buffer is not allocated and should be considered identical to the
        // input buffer, or in case *poutbuf was set it points to the input buffer (not necessarily to its starting
        // address).

        av_bitstream_filter_filter( _bsfc, _context->streams[_videoStreamIndex]->codec, NULL,
                                    &_filterPkt.data, &_filterPkt.size,
                                    _deMuxPkt.data, _deMuxPkt.size,
                                    (IsKey()) ? 1 : 0 );

        // Given the super tricky comment above (lifted right from the ffmpeg documentation!) our strategy is this:
        // If our output buffer overlaps our input, then allocate storage for it, copy the data into it and reset
        // our _filterPkt.data and _filterPkt.size to point to it. This way, we can always guarantee our _deMuxPkt
        // and out _filterPkt are distinct.

        if( _filterPkt.data >= _deMuxPkt.data && _filterPkt.data < (_deMuxPkt.data + _deMuxPkt.size) )
        {
            size_t filteredSize = _deMuxPkt.size - (_filterPkt.data - _deMuxPkt.data);
            uint8_t* data = (uint8_t*)av_malloc( filteredSize );
            if( !data )
                X_THROW(("Unable to allocate filter packet."));
            memcpy( data, _filterPkt.data, filteredSize );
            _filterPkt.data = data;
            _filterPkt.size = filteredSize;
        }
    }
}
