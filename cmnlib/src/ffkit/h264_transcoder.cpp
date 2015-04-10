
#include "ffkit/h264_transcoder.h"

using namespace ffkit;
using namespace cppkit;
using namespace std;

h264_transcoder::h264_transcoder( int inputTimeBaseNum, int inputTimeBaseDen,
                                  int outputTimeBaseNum, int outputTimeBaseDen,
                                  double speed,
                                  bool decodeSkipping ) :
    _step( 0.0 ),
    _speed( speed ),
    _decodeSkipping( decodeSkipping )
{
    // When the input and output framerates were specified with single integers, this was how we did it...
    // _outputFramesPerInputFrame( (((double)outputFrameRate) / (((double)inputFrameRate) * speed)) ),

    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    _outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * _speed);
}

int64_t h264_transcoder::compute_num_output_frames( int64_t numInputFrames,
                                                    int inputTimeBaseNum, int inputTimeBaseDen,
                                                    int outputTimeBaseNum, int outputTimeBaseDen,
                                                    double speed )
{
    AVRational inputRational = { inputTimeBaseNum, inputTimeBaseDen };
    AVRational outputRational = { outputTimeBaseNum, outputTimeBaseDen };

    double outputFramesPerInputFrame = av_q2d( inputRational ) / (av_q2d( outputRational ) * speed);

    return (int64_t)(outputFramesPerInputFrame * (double)numInputFrames);
}
