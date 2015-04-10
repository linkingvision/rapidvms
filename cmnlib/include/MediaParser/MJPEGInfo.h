
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef MJPEGInfo_h
#define MJPEGInfo_h

#include "MediaParser/MediaInfo.h"
#include "XSDK/XSharedLib.h"

namespace MEDIA_PARSER
{

class MediaParser;
class MJPEGMediaParser;

class MJPEGInfo : public MediaInfo
{
  friend class MediaParser;
public:
  X_API MJPEGInfo();
  X_API virtual ~MJPEGInfo();

private:
  MJPEGInfo( const MJPEGInfo& obj );
  MJPEGInfo& operator = ( const MJPEGInfo& obj );
};

};

#endif
