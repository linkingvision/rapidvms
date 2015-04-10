
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/MediaParser.h"
#include "MediaParser/H264MediaParser.h"
#include "MediaParser/MPEG4MediaParser.h"
#include "MediaParser/MJPEGMediaParser.h"

using namespace MEDIA_PARSER;
using namespace XSDK;

MediaParserUtil::MediaParserUtil(const uint8_t* buffer, size_t bufferSize) :
_buffer(buffer),_bufferSize(bufferSize),_sigOffset(0)
{}

uint8_t* MediaParserUtil::Seek(size_t offset) const
{
    if( offset < _bufferSize )
        return const_cast<uint8_t* >(_buffer + offset);
    return NULL;
}

uint8_t* MediaParserUtil::Sentinal() const
{
    return const_cast<uint8_t* >((_buffer + _bufferSize));
}

uint8_t* MediaParserUtil::SeekToNextSig(const uint8_t* sig, size_t sigSize)
{
    if( _bufferSize < sigSize )
        return NULL;

    for(size_t i = _sigOffset;i <= (_bufferSize-sigSize);i++)
    {
        if(memcmp((_buffer+i),sig,sigSize) == 0)
        {
            _sigOffset = i + sigSize;
            return const_cast<uint8_t* >(_buffer + i + sigSize);
        }
    }
    return NULL;
}

MEDIA_PARSER::MEDIA_TYPE MediaParser::GetMediaType(const uint8_t* buffer, size_t bufferSize)
{
    if(_GetMediaTypeHelper<H264MediaParser>(buffer,bufferSize))
        return MEDIA_TYPE_H264;

    H264MediaParser h264RawFmt(buffer, bufferSize, false);
    if(h264RawFmt.IsYourType())
        return MEDIA_TYPE_H264_RAW;

    if(_GetMediaTypeHelper<MPEG4MediaParser>(buffer,bufferSize))
        return MEDIA_TYPE_MPEG4;

    if(_GetMediaTypeHelper<MJPEGMediaParser>(buffer,bufferSize))
        return MEDIA_TYPE_MJPEG;

    return MEDIA_TYPE_UNKNOWN;
}

//=======================================================
/// A function that attempts to convert a string
/// representation of a media type to one of the
/// enumerated values.
//=======================================================
MEDIA_PARSER::MEDIA_TYPE MediaParser::GetMediaType(const XString& type)
{
    if(type == STRING_H264 || type == MEDIA_TYPE_STRING_H264 || type == MIME_TYPE_H264 || type == MIME_TYPE_H264_NSD)
        return MEDIA_TYPE_H264;

    if(type.ToUpper() == STRING_MPEG4 || type == MEDIA_TYPE_STRING_MPEG4 || type == MIME_TYPE_MPEG4 || type == MIME_TYPE_MPEG4_NSD)
        return MEDIA_TYPE_MPEG4;

    // G711
    if(type == STRING_PCM_MULAW || type.ToUpper() == MEDIA_TYPE_STRING_PCM_MULAW || type == MIME_TYPE_PCM_MULAW)
        return MEDIA_TYPE_PCM_MULAW;

    if(type == STRING_MJPEG || type == MIME_TYPE_MJPEG) //MEDIA_TYPE_STRING_MJPEG is the same as STRING_MJPEG.
        return MEDIA_TYPE_MJPEG;

    if(type == STRING_LINEAR_16BIT || type == MIME_TYPE_LINEAR_16BIT)  //MEDIA_TYPE_STRING_LINEAR_16BIT is the same as STRING_LINEAR_16BIT.
        return MEDIA_TYPE_LINEAR_16BIT;

    if(type == STRING_META)  //MEDIA_TYPE_STRING_META is the same as STRING_META.
        return MEDIA_TYPE_META;

    return MEDIA_TYPE_UNKNOWN;
}


XString MediaParser::GetMediaTypeString(MEDIA_PARSER::MEDIA_TYPE type)
{
    switch(type)
    {
        case MEDIA_TYPE_H264:         return MEDIA_TYPE_STRING_H264;
        case MEDIA_TYPE_MPEG4:        return MEDIA_TYPE_STRING_MPEG4;
        case MEDIA_TYPE_MJPEG:        return MEDIA_TYPE_STRING_MJPEG;
        case MEDIA_TYPE_PCM_MULAW:    return MEDIA_TYPE_STRING_PCM_MULAW;
        case MEDIA_TYPE_LINEAR_16BIT: return MEDIA_TYPE_STRING_LINEAR_16BIT;
        case MEDIA_TYPE_META:         return MEDIA_TYPE_STRING_META;
        case MEDIA_TYPE_UNKNOWN:
        default:                      return MEDIA_TYPE_STRING_UNKNOWN;
    }
}


XString MediaParser::GetMimeType(MEDIA_PARSER::MEDIA_TYPE type)
{
    switch(type)
    {
        case MEDIA_TYPE_MPEG4:        return MIME_TYPE_MPEG4;
        case MEDIA_TYPE_H264:         return MIME_TYPE_H264;
        case MEDIA_TYPE_MJPEG:        return MIME_TYPE_MJPEG;
        case MEDIA_TYPE_PCM_MULAW:    return MIME_TYPE_PCM_MULAW;
        case MEDIA_TYPE_LINEAR_16BIT: return MIME_TYPE_LINEAR_16BIT;
        case MEDIA_TYPE_UNKNOWN:
        default:                      return MIME_TYPE_UNKNOWN;
    };
}


MediaParserUtil& MediaParserUtil::operator=(const MediaParserUtil& obj)
{
    //Stub added to make warnings go away
    return *this;
}

MediaParserUtil::MediaParserUtil():
    _buffer(NULL),
    _bufferSize(0),
    _sigOffset(0)
{
//Stub added to make warnings go away
}




