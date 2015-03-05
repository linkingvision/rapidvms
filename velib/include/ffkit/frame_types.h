
#ifndef __ffkit_frame_types_h
#define __ffkit_frame_types_h

namespace ffkit
{

CK_API enum encoder_frame_type
{
    FRAME_TYPE_KEY,
    FRAME_TYPE_PARTIAL,
    FRAME_TYPE_AUTO_GOP
};

}

#endif
