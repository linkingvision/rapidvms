
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_FrameTypes_h
#define __AVKit_FrameTypes_h

namespace AVKit
{

X_API enum FrameType
{
    FRAME_TYPE_KEY,
    FRAME_TYPE_PARTIAL,
    FRAME_TYPE_AUTO_GOP
};

}

#endif
