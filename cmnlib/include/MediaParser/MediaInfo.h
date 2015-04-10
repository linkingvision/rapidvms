
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef MediaInfo_h
#define MediaInfo_h

#include <map>
#include <vector>
#include "XSDK/XMemory.h"
#include "XSDK/XSharedLib.h"
#include "MediaParser/Defines.h"

namespace MEDIA_PARSER
{
    /// Indicates the beginning of a NAL unit.
    struct NALUnit
    {
        ///
        NALUnit() : start(0), length(0), type(0) {}

        ///
        NALUnit(const uint8_t* s, size_t l, int t)
            : start(s), length(l), type(t) {}

        /// The beginning of the NAL unit (not including the header signature: 0x00, 0x00, 0x01).
        const uint8_t* start;

        /// The length of the NAL unit.
        size_t length;

        /// The type of the NAL unit / the start code.
        ///
        /// For H264, this is a value from NAL_TYPE.
        /// For MPEG-4, this is a value from START_CODE.
        int type;
    };


    class MediaInfo
    {
    public:

        ///
        X_API MediaInfo();

        ///
        X_API virtual ~MediaInfo() {}


        ///
        X_API bool HasStreamInfo() const { return _hasStreamInfo; }

        ///
        X_API void SetStreamInfo(bool hasStreamInfo) { _hasStreamInfo = hasStreamInfo; }


        /// Returns the type of the first NAL unit within the frame.
        ///
        /// Note that a given frame could include multiple NAL unit types. So, "frame type"
        /// is incredibly misleading and ideally wouldn't be used at all.
        X_API FRAME_TYPE GetFrameType() const { return _frameType; }

        ///
        X_API void SetFrameType(FRAME_TYPE type) { _frameType = type; }


        ///
        X_API unsigned int GetFrameWidth() const { return _frameWidth; }

        ///
        X_API void SetFrameWidth(unsigned int width) { _frameWidth = width; }


        ///
        X_API unsigned int GetFrameHeight() const { return _frameHeight; }

        ///
        X_API void SetFrameHeight(unsigned int height) { _frameHeight = height; }


        ///
        X_API double GetFrameRate() const { return _frameRate; }

        ///
        X_API void SetFrameRate(double frameRate) { _frameRate = frameRate; }


        ///
        X_API unsigned int GetBitRate() const { return _bitRate; }

        ///
        X_API void SetBitRate(unsigned int bitRate) { _bitRate = bitRate; }


        ///
        X_API MEDIA_TYPE GetMediaType() const { return _mediaType; }

        ///
        X_API void SetMediaType(MEDIA_TYPE type) { _mediaType = type; }


        ///
        X_API CHROMA_FORMAT GetChromaFormat() const { return _chromaFormat; }

        ///
        X_API void SetChromaFormat(CHROMA_FORMAT format) { _chromaFormat = format; }


        ///
        X_API bool ContainsKeyFrame() const { return _containsKeyFrame; }

        ///
        X_API void SetContainsKeyFrame(bool keyFrame) { _containsKeyFrame = keyFrame; }


        ///
        X_API const std::vector<NALUnit>& GetNALUnits() const { return _nalUnits; }

        /// Returns all of the NAL units in this MediaInfo with the given type.
        X_API std::vector<NALUnit> GetNALUnits(int type) const;

        ///
        X_API void SetNALUnits(const std::vector<NALUnit>& nalUnits) { _nalUnits = nalUnits; }


        /// Whether the frame has any user data.
        X_API bool HasUserData() const { return !_userData.empty(); }

        /// Returns the user data sections in this frame.
        ///
        /// The XMemory in the pair is the user data itself.
        /// The size_t is the index for the NAL unit that the user data came from.
        const X_API std::vector<std::pair<XIRef<XSDK::XMemory>, size_t> >& GetUserData() const { return _userData; }

        /// Sets the user data sections.
        X_API void SetUserData(const std::vector<std::pair<XIRef<XSDK::XMemory>, size_t> >& userData) { _userData = userData; }

        /// Adds a user data section to the end of the list of user data sections.
        X_API void AddUserData(const std::pair<XIRef<XSDK::XMemory>, size_t>& userData) { _userData.push_back(userData); }

        /// Returns the SDP codec-specific data.
        /// @note This data is codec specific. h.264 returns the sprop-parameter-sets.
        /// MPEG-4 returns the profile-level-id and config lines.
        X_API XSDK::XString GetSDPCodecData() { return _sdpCodecData; }

        /// Sets the SDP codec-specific data.
        X_API void SetSDPCodecData(const XSDK::XString& data) { _sdpCodecData = data; }


    protected:

        bool _hasStreamInfo;
        FRAME_TYPE _frameType;
        unsigned int _frameWidth;
        unsigned int _frameHeight;
        double _frameRate;
        unsigned int _bitRate;
        MEDIA_TYPE _mediaType;
        CHROMA_FORMAT _chromaFormat;
        bool _containsKeyFrame;
        std::vector<NALUnit> _nalUnits;
        std::vector<std::pair<XIRef<XSDK::XMemory>, size_t> > _userData;
        XSDK::XString _sdpCodecData;
    };

};

#endif

