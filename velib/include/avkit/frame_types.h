
#ifndef __avkit_frame_types_h
#define __avkit_frame_types_h

namespace avkit
{

CK_API enum encoder_frame_type
{
    FRAME_TYPE_KEY,
    FRAME_TYPE_PARTIAL,
    FRAME_TYPE_AUTO_GOP
};

}

#endif
