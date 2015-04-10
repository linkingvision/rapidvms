
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/MPEG4Info.h"

using namespace MEDIA_PARSER;

MPEG4Info::MPEG4Info() :
  MediaInfo(),
  _videoObjectLayerShape( 0 ),
  _vopTimeIncrementResolution( 0 ),
  _fixedVOPRate( false ),
  _fixedVOPTimeIncrement( 0 ),
  _profileLevelID( 0 )
{
}

MPEG4Info::~MPEG4Info()
{
}

uint32_t MPEG4Info::GetVideoObjectLayerShape()
{
  return _videoObjectLayerShape;
}

uint32_t MPEG4Info::GetVOPTimeIncrementResolution()
{
  return _vopTimeIncrementResolution;
}

bool MPEG4Info::IsFixedVOPRate()
{
  return _fixedVOPRate;
}

uint32_t MPEG4Info::GetFixedVOPTimeIncrement()
{
  return _fixedVOPTimeIncrement;
}

uint8_t MPEG4Info::GetProfileLevelID()
{
  return _profileLevelID;
}
