
#ifndef __ffkit_av_demuxer_h
#define __ffkit_av_demuxer_h

#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_nullable.h"
#include <utility>
#include <vector>
#include <map>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

namespace ffkit
{

struct stream_statistics
{
    cppkit::ck_nullable<uint32_t> averageBitRate;
    cppkit::ck_nullable<double> frameRate;
    cppkit::ck_nullable<int> timeBaseNum;
    cppkit::ck_nullable<int> timeBaseDen;
    cppkit::ck_nullable<uint16_t> gopSize;
    cppkit::ck_nullable<uint32_t> numFrames;
};

enum stream_type
{
    STREAM_TYPE_UNKNOWN = -1,
    STREAM_TYPE_VIDEO,
    STREAM_TYPE_AUDIO,
    STREAM_TYPE_DATA,
    STREAM_TYPE_SUBTITLE,
    STREAM_TYPE_ATTACHMENT,
    STREAM_TYPE_NUM
};

struct stream_info
{
    stream_type type;
    int index;
    enum AVCodecID     codec_id; /* see AV_CODEC_ID_xxx */
    int width;
    int height;
};

class h264mp4_to_annexb;
class h264_decoder;
class jpeg_decoder;

class av_demuxer
{
    friend class h264mp4_to_annexb;
    friend class h264_decoder;
    friend class jpeg_decoder;

public:
    CK_API av_demuxer( const cppkit::ck_string& fileName, bool annexBFilter = true );

    CK_API av_demuxer( const uint8_t* buffer,
                       size_t bufferSize,
                       bool annexBFilter = true );

    CK_API av_demuxer( std::shared_ptr<cppkit::ck_memory> buffer, bool annexBFilter = true );

    CK_API virtual ~av_demuxer() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API cppkit::ck_string get_file_name() const;

    CK_API double get_seconds_between_frames(int streamIndex ) const;
    CK_API std::pair<int,int> get_time_base( int streamIndex ) const;

    CK_API bool get_stream_types(int streamIndex, 
		struct stream_info &p_stream_info);
    CK_API int get_video_stream_index() const;
    CK_API int get_primary_audio_stream_index() const;

    CK_API bool read_frame( int& streamIndex );
    CK_API bool end_of_file() const;
    CK_API bool is_key() const;

    CK_API std::shared_ptr<av_packet> get() const;

    CK_API static std::shared_ptr<cppkit::ck_memory> load_file( const cppkit::ck_string& fileName );

    CK_API static struct stream_statistics get_video_stream_statistics( const cppkit::ck_string& fileName );

private:
    av_demuxer( const av_demuxer& obj );
    av_demuxer& operator = ( const av_demuxer& obj );

    void _open_streams();
    void _open_custom_io_context( const uint8_t* buffer, size_t bufferSize );

    static int _read( void* opaque, uint8_t* dest, int size );
    static int64_t _seek( void* opaque, int64_t offset, int whence );

    void _free_packet();
    void _free_filter_packet();

    void _optional_annexb_filter();

    cppkit::ck_string _fileName;
    AVIOContext* _memoryIOContext;
    std::shared_ptr<cppkit::ck_memory> _storage;
    int64_t _pos;
    AVFormatContext* _context;
    double _secondsBetweenFrames;
    bool _eof;
    AVPacket _deMuxPkt;
    AVPacket _filterPkt;
    std::map<int , struct stream_info> _streamTypes;
    int _videoStreamIndex;
    int _audioPrimaryStreamIndex;
    AVBitStreamFilterContext* _bsfc;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
