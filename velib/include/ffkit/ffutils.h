
#ifndef __ffkit_utils_h
#define __ffkit_utils_h

#include "cppkit/ck_types.h"
#include <climits>

namespace ffkit
{

// Adjusts requestedWidth X requestedHeight to maintain aspect ratio of streamWidth X streamHeight.

void aspect_correct_dimensions( uint16_t streamWidth, uint16_t streamHeight,
                                uint16_t requestedWidth, uint16_t requestedHeight,
                                uint16_t& destWidth, uint16_t& destHeight );

double q_to_d( int num, int den );

void d_to_q( double val, int& num, int& den, int max = INT_MAX );

}

#endif
