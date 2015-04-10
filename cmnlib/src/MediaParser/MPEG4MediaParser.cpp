
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/MPEG4MediaParser.h"
#include "MediaParser/MPEG4Info.h"
#include "MediaParser/MediaParser.h"
#include "XSDK/XBitsy.h"

using namespace std;
using namespace MEDIA_PARSER;
using namespace XSDK;

MPEG4MediaParser::MPEG4MediaParser(const uint8_t* buffer, const size_t bufferSize)
    : _buffer(buffer),
      _bufferSize(bufferSize)
{
}

bool MPEG4MediaParser::IsYourType() const
{
    MediaParserUtil parserUtil(_buffer,_bufferSize);

    while(uint8_t* header = parserUtil.SeekToNextSig(MPEG4HeaderSig,3))
    {
        switch( *header )
        {
        case 0xB0:
        case 0xB5:
        case 0xB6:
        case 0x20:
        case 0xB2:
            return true;
        };
        if(0x20 == (*header&0xF0))
            return true;
    }
    return false;
}

bool MPEG4MediaParser::Parse(MediaInfo & info) const
{
    MPEG4Info mp4Info;
    if( !Parse( mp4Info ) )
        return false;
    info = mp4Info;
    return true;
}

bool MPEG4MediaParser::Parse(MPEG4Info & info) const
{
    info = MPEG4Info();
    MediaParserUtil parserUtil(_buffer,_bufferSize);

    info.SetContainsKeyFrame(false);
    info.SetFrameType(FRAME_TYPE_P_FRAME);

    if( _buffer && _bufferSize && IsYourType())
    {
        int nalIndex = 0;
        NALUnit nalUnit;
        vector<NALUnit> nalUnits;
        vector<pair<XIRef<XMemory>, size_t> > userData;
        const uint8_t* begin = 0;
        uint8_t* vosHeader = 0;

        for(; uint8_t* header = parserUtil.SeekToNextSig(MPEG4HeaderSig,3); ++nalIndex)
        {
            if(nalIndex)
            {
                assert(nalUnit.start != 0);
                assert(nalUnit.length == 0);
                nalUnit.length = header - nalUnit.start - 3;
                nalUnits.push_back(nalUnit);
                nalUnit = NALUnit();
            }

            nalUnit.start = header;
            nalUnit.type = *header;

            info.SetMediaType(MEDIA_TYPE_MPEG4);

            if(begin)
            {
                const size_t userDataSize = header - begin - 3;
                XIRef<XMemory> buffer = new XMemory(userDataSize);
                uint8_t& buffPtr = buffer->Extend(userDataSize);
                memcpy(&buffPtr, begin, userDataSize);
                userData.push_back(make_pair(buffer, nalIndex - 1));
                begin = 0;
            }

            if( *header == SC_VIDEO_OBJECT_LAYER )
            {
                // VOL header!
                info.SetStreamInfo(true);
                info.SetFrameType(FRAME_TYPE_I_FRAME);
                info.SetContainsKeyFrame(true);

                uint8_t* VOL = header+1;

                // XBitsy keeps track of our current position in VOL
                // and lets us read bits.
                XBitsy<const uint8_t*> bitsy( VOL, parserUtil.Sentinal() );

                // garbage is used occasionally when we just need to consume a bit
                uint32_t garbage = 0;

                // MPEG4 VOL's are bit packed and variable in size.
                // Required fields are fully left justified. Indentation shows optionial fields
                // whose presence depends upon the value of the immediately preceeding field.
                // For example: The presence of the videoObjectLayerVerid depends upon the
                // value of isObjectLayerIndentifier.

                uint32_t randomAccessibleVOL = 0;
                uint32_t videoObjectTypeIndication = 0;
                uint32_t isObjectLayerIdentifier = 0;
                uint32_t videoObjectLayerVerid = 0;
                uint32_t videoObjectLayerPriority = 0;
                uint32_t aspectRatioInfo = 0;
                uint32_t parWidth = 0;
                uint32_t parHeight = 0;
                uint32_t volControlParameters = 0;
                uint32_t chromaFormat = 0;
                uint32_t lowDelay = 0;
                uint32_t vbvParameters = 0;
                uint32_t firstHalfBitRate = 0;
                uint32_t latterHalfBitRate = 0;
                uint32_t firstHalfVBVBufferSize = 0;
                uint32_t latterHalfVBVBufferSize = 0;
                uint32_t firstHalfVBVOccupancy = 0;
                uint32_t latterHalfVBVOccupancy = 0;
                uint32_t videoObjectLayerShape = 0;
                uint32_t videoObjectLayerShapeExtension = 0;
                uint32_t videoObjectLayerWidth = 0;
                uint32_t videoObjectLayerHeight = 0;
                uint32_t vopTimeIncrementResolution = 0;
                uint32_t fixedVOPRate = 0;
                uint32_t fixedVOPTimeIncrement = 0;

                randomAccessibleVOL = bitsy.GetBits( 1 );
                videoObjectTypeIndication = bitsy.GetBits( 8 );
                isObjectLayerIdentifier = bitsy.GetBits( 1 );

                info.SetFrameType(FRAME_TYPE_I_FRAME);

                if( isObjectLayerIdentifier )
                {
                    videoObjectLayerVerid = bitsy.GetBits( 4 );
                    videoObjectLayerPriority = bitsy.GetBits( 3 );
                }

                // aspectRatioInfo
                //
                // 0000 Forbidden
                // 0001 1:1 (Square)
                // 0010 12:11 (625-type for 4:3)
                // 0011 10:11 (525-type for 4:3)
                // 0100 16:11 (625-type stretched for 16:9)
                // 0101 40:33 (525-type stretched for 16:9)
                // 0110-1110 Reserved
                // 1111 extended PAR
                aspectRatioInfo = bitsy.GetBits( 4 );

                if( aspectRatioInfo == 15 ) // extended PAR
                {
                    parWidth = bitsy.GetBits( 8 );
                    parHeight = bitsy.GetBits( 8 );
                }

                volControlParameters = bitsy.GetBits( 1 );

                info.SetChromaFormat( CHROMA_FORMAT_UNKNOWN );

                if( volControlParameters )
                {
                    chromaFormat = bitsy.GetBits( 2 );
                    if( chromaFormat == 1 )
                        info.SetChromaFormat( CHROMA_FORMAT_YUV_420 );

                    lowDelay = bitsy.GetBits( 1 );
                    vbvParameters = bitsy.GetBits( 1 );

                    if( vbvParameters )
                    {
                        firstHalfBitRate = bitsy.GetBits( 15 );
                        garbage = bitsy.GetBits( 1 );
                        latterHalfBitRate = bitsy.GetBits( 15 );
                        garbage = bitsy.GetBits( 1 );
                        firstHalfVBVBufferSize = bitsy.GetBits( 15 );
                        garbage = bitsy.GetBits( 1 );
                        latterHalfVBVBufferSize = bitsy.GetBits( 3 );
                        firstHalfVBVOccupancy = bitsy.GetBits( 11 );
                        garbage = bitsy.GetBits( 1 );
                        latterHalfVBVOccupancy = bitsy.GetBits( 15 );
                        garbage = bitsy.GetBits( 1 );
                    }
                }

                // videoObjectLayerShape
                //
                // 00 rectangular
                // 01 binary
                // 10 binary only
                // 11 grayscale
                videoObjectLayerShape = bitsy.GetBits( 2 );

                info._videoObjectLayerShape = videoObjectLayerShape;

                if( videoObjectLayerShape == 3 && (videoObjectLayerVerid != 1) )
                {
                    videoObjectLayerShapeExtension = bitsy.GetBits( 4 );
                }

                garbage = bitsy.GetBits( 1 );

                // ticks per second
                vopTimeIncrementResolution = bitsy.GetBits( 16 );

                info._vopTimeIncrementResolution = vopTimeIncrementResolution;

                garbage = bitsy.GetBits( 1 );

                fixedVOPRate = bitsy.GetBits( 1 );

                if( fixedVOPRate )
                {
                    info._fixedVOPRate = true;

                    int minBitsForVOPTimeIncrementResolution = bitsy.NeededBits( vopTimeIncrementResolution );
                    fixedVOPTimeIncrement = bitsy.GetBits( minBitsForVOPTimeIncrementResolution );
                    info._fixedVOPTimeIncrement = fixedVOPTimeIncrement;

                    if(fixedVOPTimeIncrement > 0)
                        info.SetFrameRate( (vopTimeIncrementResolution/fixedVOPTimeIncrement) );
                }

                if( videoObjectLayerShape != 2 )
                {
                    if( videoObjectLayerShape == 0 )
                    {
                        garbage = bitsy.GetBits( 1 );
                        videoObjectLayerWidth = bitsy.GetBits( 13 );
                        garbage = bitsy.GetBits( 1 );
                        videoObjectLayerHeight = bitsy.GetBits( 13 );
                        garbage = bitsy.GetBits( 1 );

                        info.SetFrameWidth( videoObjectLayerWidth );
                        info.SetFrameHeight( videoObjectLayerHeight );
                    }
                }

                // Generate the SDP config info, which includes everything from
                // the sequence header thru the VOL.
                if (vosHeader)
                {
                    XString sdp("profile-level-id=");
                    sdp += XString::Format("%d;config=", info.GetProfileLevelID());

                    // Find next start code so we know where this segment ends
                    MediaParserUtil parser(header, parserUtil.Sentinal()-VOL);
                    uint8_t* end = parser.SeekToNextSig(MPEG4HeaderSig, 3);
                    if (end)
                    {
                        end -= 3; // Move back to start of header signature.
                        size_t len = (size_t)(end - vosHeader);
                        for (size_t ii=0; ii<len; ++ii)
                            sdp += XString::Format("%02x", vosHeader[ii]);
                        info.SetSDPCodecData(sdp);
                    }
                }
            }
            else if(*header == SC_VISUAL_OBJECT_SEQUENCE)
            {
                info.SetFrameType(FRAME_TYPE_I_FRAME);
                info.SetContainsKeyFrame(true);

                info._profileLevelID = *(header + 1);
                vosHeader = header - 3; // Move back to start of header signature.
            }
            else if((*header == SC_VOP) && ((*(header+1) & 0xc0) == 0x00))
            {
                info.SetFrameType(FRAME_TYPE_I_FRAME);
                info.SetContainsKeyFrame(true);
            }
            else if(*header == SC_USER_DATA)
                begin = header + 1;
        }

        assert(nalIndex);
        assert(nalUnit.start != 0);
        assert(nalUnit.length == 0);
        nalUnit.length = (_buffer + _bufferSize) - nalUnit.start;
        nalUnits.push_back(nalUnit);
        info.SetNALUnits(nalUnits);

        if(begin)
        {
            const size_t userDataSize = _bufferSize - (begin - _buffer);
            XIRef<XMemory> buffer = new XMemory(userDataSize);
            uint8_t& buffPtr = buffer->Extend(userDataSize);
            memcpy(&buffPtr, begin, userDataSize);
            userData.push_back(make_pair(buffer, nalIndex - 1));
        }

        if(!userData.empty())
            info.SetUserData(userData);
    }
    else
        return false;

    return true;
}

