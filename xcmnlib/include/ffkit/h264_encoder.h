
#ifndef __ffkit_h264_encoder_h
#define __ffkit_h264_encoder_h

#include "ffkit/ffoptions.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "ffkit/frame_types.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavcodec/avcodec.h"
}

namespace ffkit
{

const int H264_ENCODE_ATTEMPTS = 16;

class h264_encoder
{
public:

    CK_API h264_encoder( const struct codec_options& options,
                         bool annexB = true,
                         int encodeAttempts = H264_ENCODE_ATTEMPTS );

    CK_API virtual ~h264_encoder() throw();

    CK_API void set_packet_factory( std::shared_ptr<av_packet_factory> pf ) { _pf = pf; }

    CK_API void encode_yuv420p( std::shared_ptr<av_packet> input,
                                encoder_frame_type type = FRAME_TYPE_AUTO_GOP );

    CK_API std::shared_ptr<av_packet> get();

    CK_API bool last_was_key() const { return _lastWasKey; }

    CK_API struct codec_options get_options() const;

    CK_API std::shared_ptr<cppkit::ck_memory> get_extra_data() const;

private:
    h264_encoder( const h264_encoder& obj );
    h264_encoder& operator = ( const h264_encoder& );

    AVCodec* _codec;
    AVCodecContext* _context;
    struct codec_options _options;
    uint8_t _gopSize;
    uint8_t _numTillKey;
    int _encodeAttempts;
    cppkit::ck_memory _extraData;
    bool _lastWasKey;
    bool _annexB;
    std::shared_ptr<av_packet_factory> _pf;
    std::shared_ptr<av_packet> _output;
};

}

#endif
