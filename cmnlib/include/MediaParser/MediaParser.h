
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef MediaParser_h
#define MediaParser_h

#include "MediaParser/MediaInfo.h"
#include "MediaParser/MPEG4Info.h"
#include "MediaParser/H264Info.h"
#include "MediaParser/MJPEGInfo.h"
#include "XSDK/XSharedLib.h"
#include "MediaParser/Defines.h"
#include <cstdlib>
#include "MediaParser/MPEG4MediaParser.h"
#include "MediaParser/MJPEGMediaParser.h"
#include "MediaParser/H264MediaParser.h"
#include "XSDK/XString.h"

namespace MEDIA_PARSER
{
    class MediaParserUtil
    {
    public:
        MediaParserUtil(const uint8_t* buffer, const size_t bufferSize);
        //=======================================================
        // _Seek is a format agnostic method of moving our current _pos variable
        // to some offset within the buffer pointed to by _buffer. Usually, the
        // argument here will be 0, and this method will be used at the beginning
        // of a type determination.
        //=======================================================
        uint8_t*  Seek(const size_t offset) const;

        //=======================================================
        // Returns a pointer 1 byte past the passed in buffer. Used only for
        // comparisons internally!
        //=======================================================
        uint8_t*  Sentinal() const;

        uint8_t*  SeekToNextSig(const uint8_t* sig, const size_t sigSize);

    private:
        MediaParserUtil();
        MediaParserUtil& operator=(const MediaParserUtil& obj);

        const uint8_t*  _buffer;
        const size_t _bufferSize;
        size_t _sigOffset;
    };

    class MediaParser
    {
    public:
        //=======================================================
        // Creates a MediaInfo object
        //
        // Arg1: Pointer to a buffer
        // Arg2: Buffer size
        // Arg3: MediaInfo type
        // Return: Success = true Failure = false
        //=======================================================
        template<class MType>
        static X_API bool GetMediaInfo(const uint8_t* buffer, size_t bufferSize, MType& type)
        {
            H264MediaParser h264(buffer,bufferSize);
            if(h264.IsYourType())
                return h264.Parse(type);

            H264MediaParser h264Raw(buffer, bufferSize, false);
            if(h264Raw.IsYourType())
                return h264Raw.Parse(type);
            MPEG4MediaParser mpeg4(buffer, bufferSize);
            if(mpeg4.IsYourType())
                return mpeg4.Parse(type);
            MJPEGMediaParser mjpeg(buffer, bufferSize);
            if(mjpeg.IsYourType())
                return mjpeg.Parse(type);

            return false;
        }

        //=======================================================
        // Get the media type of the buffer
        //
        // Arg1: Pointer to a buffer
        // Arg2: Buffer size
        // Return: Media type
        //=======================================================
        static X_API MEDIA_TYPE GetMediaType(const uint8_t* buffer, size_t bufferSize);

        //=======================================================
        // A function that attempts to convert a string
        // representation of a media type to one of the
        // enumerated values.
        //
        // Arg1: Constant reference to a media type string
        // Return: MEDIA_TYPE
        //=======================================================
        static X_API MEDIA_PARSER::MEDIA_TYPE GetMediaType(const XSDK::XString& type);

        ///
        static X_API XSDK::XString GetMediaTypeString(MEDIA_PARSER::MEDIA_TYPE type);

        /// Get mime type of a media type
        static X_API XSDK::XString GetMimeType(MEDIA_PARSER::MEDIA_TYPE type);

    private:
        template<class T>
        static bool _GetMediaTypeHelper(const uint8_t*  buffer, size_t bufferSize)
        {
            T parser(buffer,bufferSize);
            return parser.IsYourType();
        }
    };
};

#endif
