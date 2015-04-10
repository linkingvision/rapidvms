
#include "ffkit/ffutils.h"
#include <algorithm>

extern "C"
{
#include "libavutil/rational.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

namespace ffkit
{

void aspect_correct_dimensions( uint16_t streamWidth, uint16_t streamHeight,
                                uint16_t requestedWidth, uint16_t requestedHeight,
                                uint16_t& destWidth, uint16_t& destHeight )
{
    destWidth = requestedWidth;
    destHeight = requestedHeight;

    // encode size
    if(streamWidth != 0 && streamHeight !=0)
    {
        uint16_t newEncodeWidth;
        uint16_t newEncodeHeight;

        if(requestedHeight !=0 && requestedWidth != 0)
        {
            float streamAspectRatio = streamWidth * 1.0 / streamHeight;
            float maxAspectRatio = requestedWidth * 1.0 / requestedHeight;
            float scaleFactor;

            if(maxAspectRatio < streamAspectRatio)
                scaleFactor = requestedWidth * 1.0 / streamWidth;
            else
                scaleFactor = requestedHeight * 1.0 / streamHeight;

            scaleFactor = min(scaleFactor, 1.0f);

            uint16_t scaledRoundedPixelWidth = streamWidth * scaleFactor + 0.5;
            uint16_t scaledRoundedPixelHeight = streamHeight * scaleFactor + 0.5;

            uint16_t multipleOfEightWidth = max( scaledRoundedPixelWidth / 2, 1) * 2;
            uint16_t multipleOfEightHeight = max( scaledRoundedPixelHeight / 2, 1) * 2;

            newEncodeWidth = min(multipleOfEightWidth, streamWidth);
            newEncodeHeight = min(multipleOfEightHeight, streamWidth);
        }
        else
        {
            newEncodeWidth = streamWidth;
            newEncodeHeight = streamHeight;
        }

        if(requestedWidth != newEncodeWidth)
            destWidth = newEncodeWidth;

        if(requestedHeight != newEncodeHeight)
            destHeight = newEncodeHeight;
    }
}

double q_to_d( int num, int den )
{
    AVRational q = { num, den };
    return av_q2d( q );
}

void d_to_q( double val, int& num, int& den, int max )
{
    AVRational q = av_d2q( val, max );
    num = q.num;
    den = q.den;
}

}
