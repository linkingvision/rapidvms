
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef MediaParserDefines_h
#define MediaParserDefines_h

#include "XSDK/Types.h"

namespace MEDIA_PARSER
{
    //=======================================================
    // The media class
    //=======================================================
    typedef enum _MEDIA_CLASS
    {
        MEDIA_CLASS_UNKNOWN=0,
        MEDIA_CLASS_VIDEO,
        MEDIA_CLASS_AUDIO,
        MEDIA_CLASS_DATA,
        MEDIA_CLASS_MAX
    } MEDIA_CLASS;


    //=======================================================
    // The media types
    //=======================================================
    typedef enum _MEDIA_TYPE
    {
        MEDIA_TYPE_UNKNOWN=0,
        // Video Types
        MEDIA_TYPE_MPEG4,
        MEDIA_TYPE_H264,
        MEDIA_TYPE_H264_RAW,
        MEDIA_TYPE_MJPEG,
        // Audio Types
        MEDIA_TYPE_PCM_MULAW,
        MEDIA_TYPE_LINEAR_16BIT, // PCM 16-bit "raw"
        // Meta Data
        MEDIA_TYPE_META,
        MEDIA_TYPE_MAX

    } MEDIA_TYPE;


    //=======================================================
    // The frame types
    //=======================================================
    typedef enum _FRAME_TYPE
    {
        FRAME_TYPE_UNKNOWN=0,
        FRAME_TYPE_I_FRAME,
        FRAME_TYPE_P_FRAME,
        FRAME_TYPE_B_FRAME,
        FRAME_TYPE_SPS_FRAME,
        FRAME_TYPE_PPS_FRAME,
        FRAME_TYPE_SEI_FRAME,
        FRAME_TYPE_MAX
    } FRAME_TYPE;

    //=======================================================
    // The chroma format
    //=======================================================
    typedef enum _CHROMA_FORMAT
    {
        CHROMA_FORMAT_UNKNOWN = 0,
        CHROMA_FORMAT_NONE,
        CHROMA_FORMAT_MONOCHROME,
        CHROMA_FORMAT_RGB24,
        CHROMA_FORMAT_YUV_420,
        CHROMA_FORMAT_YUV_422,
        CHROMA_FORMAT_YUV_444,
        CHROMA_FORMAT_MAX
    } CHROMA_FORMAT;

    //=======================================================
    // Some header signatures
    //=======================================================
    const uint8_t H264HeaderSig[] = { 0, 0, 0, 1 };
    const uint8_t MPEG4HeaderSig[] = { 0, 0, 1 };

    const unsigned int MAX_H264_PPS_SPS = 128;

    //=======================================================
    // The mime types
    //=======================================================
    const char MIME_TYPE_UNKNOWN[] = "video/unknown";
    const char MIME_TYPE_MPEG4[] = "video/MP4V-ES";
    const char MIME_TYPE_MPEG4_NSD[] = "video/mp4";
    const char MIME_TYPE_H264[] = "video/H264";
    const char MIME_TYPE_H264_NSD[] = "video/h264";
    const char MIME_TYPE_MJPEG[] = "video/JPEG";
    const char MIME_TYPE_PCM_MULAW[] = "audio/basic";
    const char MIME_TYPE_LINEAR_16BIT[] = "audio/L16"; // RFC 4856

    //=======================================================
    // The friendly names of the codecs (Used for parameters)
    //=======================================================
    const char STRING_UNKNOWN[] = "UNKNOWN";
    const char STRING_MPEG4[] = "MPEG4";
    const char STRING_H264[] = "H264";
    const char STRING_MJPEG[] = "JPEG";
    const char STRING_PCM_MULAW[] = "PCM_MULAW";
    const char STRING_LINEAR_16BIT[] = "LINEAR_16BIT";
    const char STRING_META[] = "META";

    //=======================================================
    // The friendly names of the codecs which are used by MediaParser::GetMediaTypeString
    //=======================================================
    const char MEDIA_TYPE_STRING_UNKNOWN[] = "unknown";
    const char MEDIA_TYPE_STRING_MPEG4[] = "Mpeg-4";
    const char MEDIA_TYPE_STRING_H264[] = "H.264";
    const char MEDIA_TYPE_STRING_MJPEG[] = "JPEG";
    const char MEDIA_TYPE_STRING_PCM_MULAW[] = "G711";
    const char MEDIA_TYPE_STRING_LINEAR_16BIT[] = "LINEAR_16BIT";
    const char MEDIA_TYPE_STRING_META[] = "META";

};

#endif

