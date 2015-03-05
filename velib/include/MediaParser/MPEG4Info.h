
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef MPEG4Info_h
#define MPEG4Info_h

#include "XSDK/XSharedLib.h"
#include "MediaParser/MediaInfo.h"

typedef enum _START_CODE
{
    SC_VIDEO_OBJECT = 0, //0X00 - 0X1F
    SC_VIDEO_OBJECT_LAYER = 0X20,          //0X20 - 0X2F
    SC_RESERVED_30 = 0X30,  //0X30 - 0XAF
    SC_VISUAL_OBJECT_SEQUENCE = 0XB0,
    SC_VISUAL_OBJECT_SEQUENCE_END = 0XB1,
    SC_USER_DATA = 0XB2,
    SC_GROUP_OF_VOP = 0XB3,
    SC_VIDEO_SESSION_ERROR = 0XB4,
    SC_VISUAL_OBJECT = 0XB5,
    SC_VOP = 0XB6,
    SC_RESERVED_B7 = 0XB7,  //0XB7 - 0XB9
    SC_FBA_OBJECT_START_CODE = 0XBA,
    SC_FBA_OBJECT_PLANE = 0XBB,
    SC_MESH_OBJECT = 0XBC,
    SC_MESH_OBJECT_PLANE = 0XBD,
    SC_STILL_TEXTURE_OBJECT = 0XBE,
    SC_TEXTURE_SPATIAL_LAYER = 0XBF,
    SC_TEXTURE_SNR_LAYER = 0XC1,
    SC_TEXTURE_TILE = 0XC2,
    SC_TEXTURE_SHAPE_LAYER = 0XC3,
    SC_RESERVED_C4 = 0XC4,  //0XC4 - 0XC5
    SC_SYSTEM = 0XC6,       //0XC6 - 0XFF

} START_CODE;

namespace MEDIA_PARSER
{
class MPEG4MediaParser;

class MPEG4Info : public MediaInfo
{
  friend class MPEG4MediaParser;
public:
  X_API MPEG4Info();
  X_API virtual ~MPEG4Info();

  X_API uint32_t GetVideoObjectLayerShape();
  X_API uint32_t GetVOPTimeIncrementResolution();
  X_API bool IsFixedVOPRate();
  X_API uint32_t GetFixedVOPTimeIncrement();
  X_API uint8_t GetProfileLevelID();

private:

  uint32_t _videoObjectLayerShape;
  uint32_t _vopTimeIncrementResolution;
  bool _fixedVOPRate;
  uint32_t _fixedVOPTimeIncrement;
  uint8_t _profileLevelID;
};

};

#endif
