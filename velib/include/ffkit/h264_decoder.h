
#ifndef __ffkit_h264_decoder_h
#define __ffkit_h264_decoder_h

#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

namespace ffkit
{

const int H264_DECODE_ATTEMPTS = 16;

class h264_decoder
{
public:
    CK_API h264_decoder( const struct codec_options& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    CK_API h264_decoder( av_demuxer& deMuxer, const struct codec_options& options, int decodeAttempts = H264_DECODE_ATTEMPTS );
    CK_API virtual ~h264_decoder() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void decode( std::shared_ptr<av_packet> frame );

    CK_API uint16_t get_input_width() const;
    CK_API uint16_t get_input_height() const;

    CK_API void set_output_width( uint16_t outputWidth );
    CK_API uint16_t get_output_width() const;

    CK_API void set_output_height( uint16_t outputHeight );
    CK_API uint16_t get_output_height() const;

    CK_API std::shared_ptr<av_packet> get();

private:
    h264_decoder( const h264_decoder& obj );
    h264_decoder& operator = ( const h264_decoder& );

    void _destroy_scaler();

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    AVFrame* _frame;
    SwsContext* _scaler;
    uint16_t _outputWidth;
    uint16_t _outputHeight;
    int _decodeAttempts;
    std::shared_ptr<av_packet_factory> _pf;
};

}

#endif
