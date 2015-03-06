
#ifndef __ffkit_jpeg_decoder_h
#define __ffkit_jpeg_decoder_h

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

const int JPEG_DECODE_ATTEMPTS = 16;

class jpeg_decoder
{
public:
    CK_API jpeg_decoder( const struct codec_options& options, int decodeAttempts = JPEG_DECODE_ATTEMPTS );
    CK_API jpeg_decoder( av_demuxer& deMuxer, const struct codec_options& options, int decodeAttempts = JPEG_DECODE_ATTEMPTS );
    CK_API virtual ~jpeg_decoder() throw();

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
    jpeg_decoder( const jpeg_decoder& obj );
    jpeg_decoder& operator = ( const jpeg_decoder& );

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
