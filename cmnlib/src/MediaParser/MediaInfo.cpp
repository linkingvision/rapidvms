
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/MediaInfo.h"

#include <cstring>

using namespace std;
using namespace MEDIA_PARSER;

MediaInfo::MediaInfo() :
    _hasStreamInfo(false),
    _frameType(FRAME_TYPE_UNKNOWN),
    _frameWidth(0),
    _frameHeight(0),
    _frameRate(0),
    _bitRate(0),
    _mediaType(MEDIA_TYPE_UNKNOWN),
    _chromaFormat(CHROMA_FORMAT_UNKNOWN),
    _containsKeyFrame(false),
    _nalUnits(),
    _userData(),
    _sdpCodecData()
{
}


vector<NALUnit> MediaInfo::GetNALUnits(int type) const
{
    vector<NALUnit> retval;

    for(size_t i = 0, e = _nalUnits.size(); i < e; ++i)
    {
        if(_nalUnits[i].type == type)
            retval.push_back(_nalUnits[i]);
    }

    return retval;
}
