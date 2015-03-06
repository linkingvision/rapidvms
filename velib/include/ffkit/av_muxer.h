
#ifndef __ffkit_av_muxer_h
#define __ffkit_av_muxer_h

#include "ffkit/ffoptions.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_memory.h"
#include <memory>

extern "C"
{
#include "libavformat/avformat.h"
}

namespace ffkit
{

class av_muxer
{
public:

    CK_API enum OUTPUT_LOCATION
    {
        OUTPUT_LOCATION_FILE,
        OUTPUT_LOCATION_BUFFER
    };

    CK_API av_muxer( const struct codec_options& options,
                     const cppkit::ck_string& fileName,
                     OUTPUT_LOCATION location );

    CK_API virtual ~av_muxer() throw();

    //CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API cppkit::ck_string get_file_name() const;

    CK_API void set_extra_data( std::shared_ptr<cppkit::ck_memory> extraData );

    CK_API void write_video_packet( av_packet* input, bool keyFrame );

    CK_API void flush();

    //CK_API void finalize_buffer( std::shared_ptr<cppkit::ck_memory> buffer );
    CK_API void finalize_buffer( cppkit::ck_memory* buffer );
    CK_API void finalize_file();

    CK_API void apply_codec_options( const struct codec_options& options );

private:
    av_muxer( const av_muxer& obj );
    av_muxer& operator = ( const av_muxer& obj );

    void _finalize_common();

    void _open_io();

    void _close_io();

    struct codec_options _options;
    cppkit::ck_string _fileName;
    AVFormatContext* _context;
    AVStream* _stream;
    OUTPUT_LOCATION _location;
    int64_t _ts;
    bool _oweTrailer;
    int64_t _numVideoFramesWritten;
    bool _isTS;       // true if our container type is mpeg2ts
    int64_t _fileNum; // the number of files made (really only useful for mpeg2ts)o
    //std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
