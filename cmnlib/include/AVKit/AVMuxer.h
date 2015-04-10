
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __AVKit_AVMuxer_h
#define __AVKit_AVMuxer_h

#include "AVKit/Options.h"

#include "XSDK/XMemory.h"

extern "C"
{
#include "libavformat/avformat.h"
}

namespace AVKit
{

class AVMuxer
{
public:

    X_API enum OUTPUT_LOCATION
    {
        OUTPUT_LOCATION_FILE,
        OUTPUT_LOCATION_BUFFER
    };

    X_API AVMuxer( const struct CodecOptions& options,
                   const XSDK::XString& fileName,
                   OUTPUT_LOCATION location );

    X_API virtual ~AVMuxer() throw();

    X_API XSDK::XString GetFileName() const;

    X_API void SetExtraData( XIRef<XSDK::XMemory> extraData );

    X_API void WriteVideoFrame( uint8_t* data, size_t size, bool keyFrame );
    X_API void WriteVideoFrame( XIRef<XSDK::XMemory> frame, bool keyFrame );

    X_API void Flush();

    X_API void FinalizeBuffer( XIRef<XSDK::XMemory> buffer );
    X_API void FinalizeFile();

    X_API void ApplyCodecOptions( const struct CodecOptions& options );

private:
    AVMuxer( const AVMuxer& obj );
    AVMuxer& operator = ( const AVMuxer& obj );

    void _FinalizeCommon();

    void _OpenIO();

    void _CloseIO();

    struct CodecOptions _options;
    XSDK::XString _fileName;
    AVFormatContext* _context;
    AVStream* _stream;
    OUTPUT_LOCATION _location;
    int64_t _ts;
    bool _oweTrailer;
    int64_t _numVideoFramesWritten;
    bool _isTS;       // true if our container type is mpeg2ts
    int64_t _fileNum; // the number of files made (really only useful for mpeg2ts)o
};

}

#endif
