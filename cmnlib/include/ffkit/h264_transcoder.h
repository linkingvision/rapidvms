
#ifndef __ffkit_h264_transcoder_h
#define __ffkit_h264_transcoder_h

#include "ffkit/av_demuxer.h"
#include "ffkit/h264_decoder.h"
#include "ffkit/h264_encoder.h"
#include "ffkit/av_muxer.h"
#include "ffkit/ffoptions.h"
#include "ffkit/frame_types.h"

namespace ffkit
{

class h264_transcoder
{
public:
    CK_API h264_transcoder( int inputTimeBaseNum,
                            int inputTimeBaseDen,
                            int outputTimeBaseNum,
                            int outputTimeBaseDen,
                            double speed = 1.0,
                            bool decodeSkipping = false );

    // The job of this method is to decode frames until it is time to make an output frame. If the output
    // framerate is greater than the input framerate this means that sometimes calls to this method will not
    // actually decode a frame (because we need to encode multiple outputs from the same input in that case).
    // Likewise, when the output framerate is less than the input framerate, we will sometimes decode multiple
    // times before we return. In either case, after this method returns, we are ready to make an output frame.
    template<class T>
    CK_API bool decode( T& avDeMuxer, h264_decoder& decoder )
    {
        int videoStreamIndex = avDeMuxer.get_video_stream_index();

        if( _step < 1.0 )
        {
            while( !avDeMuxer.end_of_file() && (_step < 1.0) )
            {
                int streamIndex = 0;
                if( !avDeMuxer.read_frame( streamIndex ) )
                    return false;

                if( streamIndex == videoStreamIndex )
                {
                    _step += _outputFramesPerInputFrame;

                    if( _step > 1.0 || !_decodeSkipping )
                        decoder.decode( avDeMuxer.get() );
                }
            }
        }

        _step -= 1.0;

        return true;
    }

    template<class T>
    CK_API void encode_yuv420p_and_mux( T& encoder,
                                        av_muxer& muxer,
                                        std::shared_ptr<av_packet> pic,
                                        encoder_frame_type type = FRAME_TYPE_AUTO_GOP )
    {
        encoder.encode_yuv420p( pic, type );

        std::shared_ptr<av_packet> encodeBuffer = encoder.get();

        muxer.write_video_packet( encodeBuffer, encoder.last_was_key() );
    }

    CK_API static int64_t compute_num_output_frames( int64_t numInputFrames,
                                                     int inputTimeBaseNum, int inputTimeBaseDen,
                                                     int outputTimeBaseNum, int outputTimeBaseDen,
                                                     double speed = 1.0 );

private:
    double _outputFramesPerInputFrame;
    double _step;
    double _speed;
    bool _decodeSkipping;
};

};

#endif
