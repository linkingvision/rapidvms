
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/H264MediaParser.h"

#include <climits>
#include <cstdio>
#include <cstring>
#include <exception>
#include <vector>

#include "MediaParser/MediaParser.h"
#include "XSDK/XLogger.h"
#include "XSDK/XString.h"

using namespace std;
using namespace MEDIA_PARSER;
using namespace XSDK;

class ParsingException : public std::exception
{
public:
    ParsingException(size_t line)
        : _msg(XString::Format("%s(%u): H264 Media Parsing Exception.", __FILE__, line))
    {}

    virtual const char* what() const throw() { return _msg.c_str(); }

private:
    XString _msg;
};

H264MediaParser::H264MediaParser(const uint8_t* buffer,
                                 const size_t bufferSize,
                                 bool byteStreamFormat)
    : _buffer(buffer),
      _bufferSize(bufferSize),
      _byteStreamFormat(byteStreamFormat),
      _sps(0),
      _spsLength(0),
      _pps(0),
      _ppsLength(0)
{
}

bool H264MediaParser::IsYourType() const
{
    if(_byteStreamFormat)
    {
        MediaParserUtil parserUtil(_buffer,_bufferSize);
        while(uint8_t* header = parserUtil.SeekToNextSig(H264HeaderSig,4))
            return _IsNalType(*header);
    }
    else
        return _IsNalType(*_buffer);

    return false;
}

// [pma] Find the start and end of a H.264 NAL unit in a byte buffer containing H.264 bitstream.
size_t H264MediaParser::FindH264NalUnit(const uint8_t* frame,
                                        const size_t length,
                                        size_t& nal_start,
                                        size_t& nal_end)
{
    size_t i = 0;
    nal_start = 0;
    nal_end = 0;

    while(//( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
          (frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0x01) &&
          (frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0 || frame[i+3] != 0x01))
    {
        ++i; // skip leading zero
        if(i + 4 >= length) // did not find nal start
            return 0;
    }

    if(frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0x01) // ( next_bits( 24 ) != 0x000001 )
        ++i;

    if(frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0x01)
    {
        //error, should never happen
        return 0;
    }

    i += 3;
    nal_start = i;

    while(//( next_bits( 24 ) != 0x000000 && next_bits( 24 ) != 0x000001 )
          (frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0) &&
          (frame[i] != 0 || frame[i+1] != 0 || frame[i+2] != 0x01))
    {
        ++i;

        if(i + 3 >= length)
        {
            nal_end = length;

            return nal_end - nal_start;
        } // did not find nal end, stream ended first
    }

    nal_end = i;

    return nal_end - nal_start;
}

bool H264MediaParser::Parse(MediaInfo& info, bool parseFull)
{
    H264Info h264Info;

    if(!Parse(h264Info, parseFull))
        return false;

    info = h264Info;

    return true;
}

bool H264MediaParser::Parse(H264Info& info, bool parseFull)
{
    info = H264Info();

    if(_buffer && _bufferSize && IsYourType())
    {
        _sps = _pps = 0;
        _spsLength = _ppsLength = 0;

        if(_byteStreamFormat)
        {
            vector<NALUnit> nalUnits;
            const uint8_t* buffPtr = _buffer;
            info.SetMediaType(MEDIA_TYPE_H264);

            for(int i = 0; buffPtr < _buffer + _bufferSize; ++i)
            {
                size_t nalStart = 0;
                size_t nalEnd = 0;
                const size_t nalLength = FindH264NalUnit(buffPtr,
                                                         _bufferSize - (buffPtr - _buffer),
                                                         nalStart,
                                                         nalEnd);

                if(nalLength != 0)
                {
                    nalUnits.push_back(NALUnit(buffPtr + nalStart, nalLength, *(buffPtr + nalStart) & 31));

                    if(!_Parse(buffPtr + nalStart, nalLength, info, i, parseFull))
                        return false;

                    buffPtr += nalStart + nalLength;
                }
                else
                    break;
            }

            info.SetNALUnits(nalUnits);
        }
        else
        {
            //Does _anything_ that we do involve raw H264 frames? If not, they
            //should be removed entirely from the media parser.
            X_LOG_WARNING("Attempting to process a raw H264 frame. This has not been properly tested and may not work.");

            info.SetMediaType(MEDIA_TYPE_H264_RAW);

            if(!_Parse(_buffer, _bufferSize, info, 0, parseFull))
                return false;

            vector<NALUnit> nalUnits;
            nalUnits.push_back(NALUnit(_buffer, _bufferSize, *_buffer & 31));
            info.SetNALUnits(nalUnits);
        }

        // Generate the SDP sprop-parameter-sets.
        if (_sps && _spsLength && _pps && _ppsLength)
        {
            XString sdp("sprop-parameter-sets=");
            sdp += XString::Base64Encode(_sps, _spsLength);
            sdp += ',';
            sdp += XString::Base64Encode(_pps, _ppsLength);
            info.SetSDPCodecData(sdp);
        }

        return true;
    }

    return false;
}

bool H264MediaParser::_IsNalType(const uint8_t type) const
{
    if(_IsSPS(type))
        return true;

    if(_IsPPS(type))
        return true;

    switch(type)
    {
        //Others
        case 0x06:
        case 0x61:
        case 0x41:
        case 0x21:
        case 0x01:
        case 0x65:
        case 0x45:
        case 0x25:
            return true;
        default:
            return false;
    }
}

bool H264MediaParser::_IsPPS(const uint8_t type) const
{
    switch(type)
    {
         // PPS
        case 0x68:
        case 0x48:
        case 0x28:
            return true;
        default:
            return false;
    }
}

bool H264MediaParser::_IsSPS(const uint8_t type) const
{
    switch(type)
    {
        // SPS
        case 0x67:
        case 0x47:
        case 0x27:
            return true;
        default:
            return false;
    }
}

bool H264MediaParser::_Parse(const uint8_t* frame, size_t length, H264Info& info, int nalIndex, bool parseFull)
{
    if(length < 2)
        return false;

    uint32_t nal_unit_type;

    {
        XBitsy<const uint8_t*> bitsy(frame, frame + length);
        bitsy.GetBits(1); //forbidden_zero_bit
        bitsy.GetBits(2); //nal_ref_idc
        nal_unit_type = bitsy.GetBits(5); //nal_unit_type
    }

    // omits first byte of NAL which contains nal_ref_idc and nal_unit_type, this is NOT part of the RBSP
    const uint8_t* rbspPtr = frame + 1;
    const size_t dst_length = length - 1;

    switch(nal_unit_type)
    {
        case NAL_IDR_SLICE:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_I_FRAME);

            info.SetContainsKeyFrame(true);

            return true;
        }
        case NAL_SPS:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_SPS_FRAME);

            _sps = const_cast<uint8_t*>(frame);
            _spsLength = length;

            RBSPIter rbspEnd;
            RBSPIter rbspIter = GetRBSPFromNAL(rbspPtr, dst_length, &rbspEnd);

            try
            {
                XBitsy<RBSPIter> bitsy(rbspIter, rbspEnd);
                SPSInfo spsInfo = _Parse_seq_parameter_set_data(bitsy, parseFull);

                info.SetFrameWidth(spsInfo.frameWidth);
                info.SetFrameHeight(spsInfo.frameHeight);
                info.SetFrameRate(spsInfo.frameRate);
                info.SetBitRate(spsInfo.bitRate);
                info.SetChromaFormat(spsInfo.chromaFormat);
                info.SetLevelIDC(spsInfo.levelIDC);
                info.SetStreamInfo(true);
            }
            catch(ParsingException& pe)
            {
                return false;
            }
            catch(XException& e)
            {
                return false;
            }

            return true;
        }
        case NAL_PPS:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_PPS_FRAME);

            _pps = const_cast<uint8_t*>(frame);
            _ppsLength = length;

            return true;
        }
        case NAL_SEI:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_SEI_FRAME);

            RBSPIter rbspEnd;
            RBSPIter rbspIter = GetRBSPFromNAL(rbspPtr, dst_length, &rbspEnd);

            try
            {
                XIRef<XMemory> data = _Parse_sei_message(rbspIter, rbspEnd);

                if(data.IsValid())
                    info.AddUserData(make_pair(data, nalIndex));
            }
            catch(XException& e)
            {
                return false;
            }

            return true;
        }
        case NAL_SLICE:
        case NAL_DPA:
        case NAL_DPB:
        case NAL_DPC:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_P_FRAME);

            return true;
        }
        default:
        {
            if(nalIndex == 0)
                info.SetFrameType(FRAME_TYPE_UNKNOWN);

            //return false;
            return true;
        }
    }
}


XIRef<XMemory> H264MediaParser::_Parse_sei_message(RBSPIter iter, RBSPIter end) const
{
    if(iter >= end)
        X_THROW(("RBSP data missing for SEI unit"));

    size_t payloadType = 0;

    while(*iter == 0XFF)
    {
        payloadType += 0XFF;

        if(++iter == end)
            X_THROW(("Malformed SEI payload type"));
    }

    payloadType += *iter;
    if(++iter == end)
        X_THROW(("Malformed SEI payload size"));

    //We only care about the type user_data_unregistered.
    if(payloadType != 5)
        return NULL;

    size_t payloadSize = 0;

    while(*iter == 0XFF)
    {
        payloadSize += 0XFF;

        if(++iter == end)
            X_THROW(("Malformed SEI payload size"));
    }

    payloadSize += *iter;
    if(++iter == end)
        X_THROW(("Malformed SEI payload"));

    return _Parse_sei_payload(iter, end, payloadSize);
}


XIRef<XMemory> H264MediaParser::_Parse_sei_payload(RBSPIter iter, RBSPIter end, const size_t payloadSize) const
{
    XIRef<XMemory> userData = new XMemory(payloadSize);
    uint8_t* ptr = &userData->Extend(payloadSize);
    size_t copied = 0;

    for(; iter != end && copied < payloadSize; ++iter, ++ptr, ++copied)
        *ptr = *iter;

    if(copied < payloadSize)
        X_THROW(("Malformed SEI payload"));

    return userData;
}


H264MediaParser::SPSInfo H264MediaParser::_Parse_seq_parameter_set_data(XBitsy<RBSPIter>& bitsy, bool parseFull) const
{
    SPSInfo info;

    const uint32_t profile_idc = bitsy.GetExactBits(8); //profile_idc
    bitsy.GetExactBits(1); //constraint_set0_flag
    bitsy.GetExactBits(1); //constraint_set1_flag
    bitsy.GetExactBits(1); //constraint_set2_flag
    bitsy.GetExactBits(1); //constraint_set3_flag

    if(bitsy.GetExactBits(4)) //reserved_zero_bits
        throw ParsingException(__LINE__);

    info.levelIDC = bitsy.GetExactBits(8); //level_idc
    _ParseUEV(bitsy);  //seq_parameter_set_id
    bool separate_colour_plane_flag = 0;
    uint32_t chroma_format_idc = 0;

    if(profile_idc == 100 ||
       profile_idc == 110 ||
       profile_idc == 122 ||
       profile_idc == 244 ||
       profile_idc == 44 ||
       profile_idc == 83 ||
       profile_idc == 86)
    {
        chroma_format_idc = _ParseUEV(bitsy);

        if(chroma_format_idc == 0)
            info.chromaFormat = CHROMA_FORMAT_MONOCHROME;
        else if(chroma_format_idc == 1)
            info.chromaFormat = CHROMA_FORMAT_YUV_420;
        else if(chroma_format_idc == 2)
            info.chromaFormat = CHROMA_FORMAT_YUV_422;
        else if(chroma_format_idc == 3)
        {
            //YUV 444 did not work at all before the H264 Media Parser was overhauled
            //to use XBitsy<const uint8_t*>. It's rare (I know of no cases where we would ever see
            //it used) and difficult to test for, so it hasn't been properly tested.
            //We've decided not to worry about testing it until it's actually used.
            //In the meantime, this warning should help diagnose problems if a camera
            //happens to use it without our knowledge.
            X_LOG_WARNING("Attempting to process frame using YUV 444. It _should_ work but YUV 444 is rare and not properly tested, so it may not.");
            info.chromaFormat = CHROMA_FORMAT_YUV_444;
            separate_colour_plane_flag = bitsy.GetExactBits(1);
        }

        _ParseUEV(bitsy); //bit_depth_luma_minus8
        _ParseUEV(bitsy); //bit_depth_chroma_minus8
        bitsy.GetExactBits(1); //qpprime_y_zero_transform_bypass_flag

        if(bitsy.GetExactBits(1)) //seq_scaling_matrix_present_flag
        {
            for(int i = 0, e = chroma_format_idc != 3 ? 8 : 12;
                i < e;
                ++i)
            {
                if(bitsy.GetExactBits(1)) //seq_scaling_list_present_flag
                    _Parse_scaling_list(bitsy, i < 6 ? 16 : 64);
            }
        }
    }
    else
        info.chromaFormat = CHROMA_FORMAT_NONE;

    _ParseUEV(bitsy); //log2_max_frame_num_minus4
    uint32_t pic_order_cnt_type = _ParseUEV(bitsy); //pic_order_cnt_type

    if(pic_order_cnt_type == 0)
        _ParseUEV(bitsy); //log2_max_pic_order_cnt_lsb_minus4
    else if(pic_order_cnt_type == 1)
    {
        bitsy.GetExactBits(1); //delta_pic_order_always_zero_flag
        _ParseSEV(bitsy); //offset_for_non_ref_pic
        _ParseSEV(bitsy); //offset_for_top_to_bottom_field
        const uint32_t num_ref_frames_in_pic_order_cnt_cycle = _ParseUEV(bitsy); //num_ref_frames_in_pic_order_cnt_cycle

        for(uint32_t i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; ++i)
            _ParseSEV(bitsy); //offset_for_ref_frame
    }

    _ParseUEV(bitsy); //num_ref_frames
    bitsy.GetExactBits(1); //gaps_in_frame_num_value_allowed_flag
    const uint32_t pic_width_in_mbs_minus1 = _ParseUEV(bitsy); //pic_width_in_mbs_minus1
    const uint32_t pic_height_in_map_units_minus1 = _ParseUEV(bitsy); //pic_height_in_map_units_minus1

    uint32_t width = (pic_width_in_mbs_minus1 + 1) * 16;
    uint32_t height = (pic_height_in_map_units_minus1 + 1) * 16;
    const bool frame_mbs_only_flag = bitsy.GetExactBits(1);

    if(!frame_mbs_only_flag)
        bitsy.GetExactBits(1); //mb_adaptive_frame_field_flag

    bitsy.GetExactBits(1); //direct_8x8_inference_flag

    if(bitsy.GetExactBits(1)) //frame_cropping_flag
    {
        const uint32_t frame_crop_left_offset   = _ParseUEV(bitsy);
        const uint32_t frame_crop_right_offset  = _ParseUEV(bitsy);
        const uint32_t frame_crop_top_offset    = _ParseUEV(bitsy);
        const uint32_t frame_crop_bottom_offset = _ParseUEV(bitsy);

        int chromaArrayType;
        int subWidthC = 0;
        int subHeightC = 0;

        if(separate_colour_plane_flag)
            chromaArrayType = 0;
        else
        {
            chromaArrayType = chroma_format_idc;

            switch(chroma_format_idc)
            {
                case 1:
                    subWidthC = 2;
                    subHeightC = 2;
                    break;
                case 2:
                    subWidthC = 2;
                    subHeightC = 1;
                    break;
                case 3:
                    subWidthC = 1;
                    subHeightC = 1;
                    break;
                default:
                    break;
            }
        }

        int cropUnitX;
        int cropUnitY;

        if(!chromaArrayType)
        {
            cropUnitX = 1;
            cropUnitY = 2 - frame_mbs_only_flag;
        }
        else
        {
            cropUnitX = subWidthC;
            cropUnitY = subHeightC * (2 - frame_mbs_only_flag);
        }

        width  -= cropUnitX * frame_crop_left_offset;
        width  -= cropUnitX * frame_crop_right_offset;
        height -= cropUnitY * frame_crop_top_offset;
        height -= cropUnitY * frame_crop_bottom_offset;
    }

    info.frameWidth = width;
    info.frameHeight = height;

    if(bitsy.GetExactBits(1)) //vui_parameters_present_flag
        _Parse_vui_parameters(bitsy, info, parseFull);

    return info;
}

void H264MediaParser::_Parse_vui_parameters(XBitsy<RBSPIter>& bitsy, SPSInfo& info, bool parseFull) const
{
    if(bitsy.GetExactBits(1)) //aspect_ratio_info_present_flag
    {
        const uint32_t aspect_ratio_idc = bitsy.GetExactBits(8); //aspect_ratio_idc
        const uint32_t extendedSAR = 255;

        if(aspect_ratio_idc == extendedSAR)
        {
            bitsy.GetExactBits(16); //sar_width
            bitsy.GetExactBits(16); //sar_height
        }
    }

    if(bitsy.GetExactBits(1)) //overscan_info_present_flag
        bitsy.GetExactBits(1); //overscan_appropriate_flag

    if(bitsy.GetExactBits(1)) //video_signal_type_present_flag
    {
        bitsy.GetExactBits(3); //video_format
        bitsy.GetExactBits(1); //video_full_range_flag

        if(bitsy.GetExactBits(1)) //couler_description_present_flag
        {
            bitsy.GetExactBits(8); //coulour_primaries
            bitsy.GetExactBits(8); //transfer_characteristics
            bitsy.GetExactBits(8); //matrix_coefficients
        }
    }

    if(bitsy.GetExactBits(1)) //chroma_loc_info_present_flag
    {
        _ParseUEV(bitsy); //chroma_sample_loc_type_top_field
        _ParseUEV(bitsy); //chroma_sample_loc_type_bottom_field
    }

    if(bitsy.GetExactBits(1)) //timing_info_present_flag
    {
        const uint32_t num_units_in_tick = bitsy.GetExactBits(32); //num_units_in_tick
        const uint32_t time_scale = bitsy.GetExactBits(32); //time_scale
        bitsy.GetExactBits(1); //fixed_frame_rate_flag

        info.frameRate = (double)time_scale / num_units_in_tick / 2;
    }

    const bool nal_hrd_parameters_present_flag = bitsy.GetExactBits(1); //nal_hrd_parameters_present_flag
    if(nal_hrd_parameters_present_flag)
        _Parse_hrd_parameters(bitsy, info, true, true);

    if(parseFull)
    {
        const bool vcl_hrd_parameters_present_flag = bitsy.GetExactBits(1); //vcl_hrd_parameters_present_flag
        if(vcl_hrd_parameters_present_flag)
            _Parse_hrd_parameters(bitsy, info, false, false);

        if(nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag)
            bitsy.GetExactBits(1); //low_delay_hrd_flag

        bitsy.GetExactBits(1); //pic_struct_present_flag

        if(bitsy.GetExactBits(1)) //bitstream_restriction_flag
        {
            bitsy.GetExactBits(1); //motion_vectors_over_pic_boundaries_flag
            _ParseUEV(bitsy); //max_bytes_per_pic_denom
            _ParseUEV(bitsy); //max_bytes_per_mb_denom
            _ParseUEV(bitsy); //log2_max_mv_length_horizontal
            _ParseUEV(bitsy); //log2_max_mv_length_vertical
            _ParseUEV(bitsy); //num_reorder_frames
            _ParseUEV(bitsy); //max_dec_frame_buffering
        }
    }
}

void H264MediaParser::_Parse_hrd_parameters(XBitsy<RBSPIter>& bitsy, SPSInfo& info, bool setBitrate, bool parseFull) const
{
    const uint32_t cpb_cnt_minus1 = _ParseUEV(bitsy); //cpb_cnt_minus1
    const uint32_t bit_rate_scale = bitsy.GetExactBits(4); //bit_rate_scale
    bitsy.GetExactBits(4); //cpb_size_scale

    for(uint32_t i = 0; i < cpb_cnt_minus1 + 1; ++i)
    {
        const uint32_t bit_rate_value_minus1 = _ParseUEV(bitsy); //bit_rate_value_minus1
        _ParseUEV(bitsy); //cpb_size_value_minus1
        bitsy.GetExactBits(1); //cbr_flag

        if(setBitrate)
            info.bitRate = (bit_rate_value_minus1 + 1) * (1 << (6 + bit_rate_scale));
    }

    if(parseFull)
    {
        bitsy.GetExactBits(5); //initial_cpb_removal_delay_length_minus1
        bitsy.GetExactBits(5); //cpb_removal_delay_length_minus1
        bitsy.GetExactBits(5); //dpb_output_delay_length_minus1
        bitsy.GetExactBits(5); //time_offset_length
    }
}

void H264MediaParser::_Parse_scaling_list(XBitsy<RBSPIter>& bitsy, size_t sizeOfScalingList) const
{
    vector<int> scalingList(sizeOfScalingList, 0);
    int lastScale = 8;
    int nextScale = 8;

    for(vector<int>::iterator iter = scalingList.begin(),
                              end  = scalingList.end();
        iter != end;
        ++iter)
    {
        if(nextScale != 0)
        {
            const int32_t delta_scale = _ParseSEV(bitsy); //delta_scale
            nextScale = (lastScale + delta_scale + 256) % 256;
        }

        *iter = nextScale == 0 ? lastScale : nextScale;
        lastScale = *iter;
    }
}
