
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "MediaParser/H264UserData.h"

#include <algorithm>
#include "MediaParser/H264MediaParser.h"

using namespace MEDIA_PARSER;
using namespace std;
using namespace XSDK;

const uint8_t USER_DATA_SEI_NUMBER = 5;
const uint8_t RBSP_TRAILING_BITS = 0x80;

SEIPayload::SEIPayload(const uint8_t* data,
                       size_t length,
                       const XSDK::XUuid uuid)
    : XBaseObject(),
      _data(new XMemory),
      _uuid(uuid)
{
    _data->Extend(length);
    memcpy(_data->begin(), data, length);
}

H264UserData::H264UserData(const uint8_t* buffer, size_t length)
    : _payloads()
{
    try
    {
        XIRef<XMemory> data = _Denalify(buffer, length);
        size_t i = 0;

        while(data->Index(i) != RBSP_TRAILING_BITS)
            _payloads.push_back(_ProcessSEIMessage(data, i));
    }
    catch(XException& e)
    {
        throw;
    }
    catch(std::exception e)
    {
        X_THROW(("Failed to parse SEI."));
    }
}


XIRef<XMemory> H264UserData::GenSEI() const
{
    size_t totalPayload = 0;

    for(vector<XRef<SEIPayload> >::const_iterator iter = _payloads.begin(),
                                                  end  = _payloads.end();
        iter != end;
        ++iter)
    {
        //16 for the UUID + at least 1 each for the payload type and size +
        //padding for a relatively arbitrary value of 30.
        totalPayload += (*iter)->GetData()->GetDataSize() + 30;
    }

    //4 bytes on front + 1 byte trailing bits + padding for a relatively arbitrary value of 10.
    XIRef<XMemory> data = new XMemory(totalPayload + 10);

    for(vector<XRef<SEIPayload> >::const_iterator iter = _payloads.begin(),
                                                  end  = _payloads.end();
        iter != end;
        ++iter)
    {
        _AddSEIMessage(data, *iter);
    }

    data->Append(RBSP_TRAILING_BITS);

    return _Nalify(data);
}


void H264UserData::_AddPayloadData(XIRef<XMemory> data, const XIRef<XMemory> payloadData)
{
    data->ResizeData(data->GetDataSize() + payloadData->GetDataSize());
    copy(payloadData->begin(), payloadData->end(), data->end() - payloadData->GetDataSize());
}


void H264UserData::_AddPayloadNum(XIRef<XMemory> data, int n)
{
    for(; n > 255; n -= 255)
        data->Append<uint8_t>(0xff);

    data->Append<uint8_t>(n);
}


void H264UserData::_AddSEIMessage(XIRef<XMemory> data, XRef<SEIPayload> payload)
{
    _AddPayloadNum(data, USER_DATA_SEI_NUMBER);
    _AddPayloadNum(data, payload->GetData()->GetDataSize() + 16);
    _AddUUID(data, payload->GetUUID());
    _AddPayloadData(data, payload->GetData());
}


void H264UserData::_AddUUID(XIRef<XMemory> data, const X_UUID& uuid)
{
    data->Extend(16);
    copy(&uuid.uc[0], &uuid.uc[16], data->end() - 16);
}


XIRef<XMemory> H264UserData::_Denalify(const uint8_t* buffer, size_t length)
{
    size_t start = 0;
    size_t end = 0;
    const size_t size = H264MediaParser::FindH264NalUnit(buffer, length, start, end);

    if(size == 0)
        X_THROW(("Invalid NAL Unit."));

    buffer += start;
    length = end - start;

    uint32_t nal_unit_type;
    XIRef<XMemory> retval = new XMemory;
    retval->Extend(length);

    {
        XBitsy<const uint8_t*> bitsy(buffer, buffer + length);
        bitsy.GetBits(1); //forbidden_zero_bit
        bitsy.GetBits(2); //nal_ref_idc
        nal_unit_type = bitsy.GetBits(5); //nal_unit_type
    }

    if(nal_unit_type != NAL_SEI)
        X_THROW(("Wrong NAL Unit type. Not SEI."));

    ++buffer;
    --length;

    const size_t limit = length;
    for(size_t i = 0, j = 0; i < limit;)
    {
        if(i < limit - 2 && buffer[i] == 0 && buffer[i+1] == 0 && buffer[i+2] == 3)
        {
            retval->Index(j++) = buffer[i++];
            retval->Index(j++) = buffer[i++];
            ++i;
            --length;
        }
        else
            retval->Index(j++) = buffer[i++];
    }

    retval->ResizeData(length);

    return retval;
}


XIRef<XMemory> H264UserData::_Nalify(const XIRef<XMemory> data)
{
    XIRef<XMemory> retval = new XMemory;

    retval->Append<uint8_t>(0);
    retval->Append<uint8_t>(0);
    retval->Append<uint8_t>(1);
    retval->Append<uint8_t>(NAL_SEI);

    for(const uint8_t* iter = data->begin(),
                       *end = data->end();
        iter != end;
        ++iter)
    {
        if(iter + 1 < end && *iter == 0 && *(iter + 1) == 0)
        {
            retval->Append<uint8_t>(*(iter++));
            retval->Append<uint8_t>(*iter);
            retval->Append<uint8_t>(3); //emulation_prevention_three_byte
        }
        else
            retval->Append<uint8_t>(*iter);
    }

    return retval;
}


XRef<SEIPayload> H264UserData::_ProcessSEIMessage(const XIRef<XMemory> data, size_t& i)
{
    uint32_t payloadType = 0;
    const size_t dataSize = data->GetDataSize();

    for(; data->Index(i) == 0xff; ++i)
    {
        payloadType += 255; //ff_byte

        if(i >= dataSize)
            X_THROW(("Failed to parse SEI."));
    }

    payloadType += data->Index(i++); //last_payload_type_byte

    if(i >= dataSize)
        X_THROW(("Failed to parse SEI."));

    size_t payloadSize = 0;
    for(; data->Index(i) == 0xff; ++i)
    {
        payloadSize += 255; //ff_byte

        if(i >= dataSize)
            X_THROW(("Failed to parse SEI."));
    }

    payloadSize += data->Index(i++); //last_payload_size_byte

    return _ProcessPayload(data, i, payloadType, payloadSize);
}


XRef<SEIPayload> H264UserData::_ProcessPayload(const XIRef<XMemory> data,
                                                   size_t& i,
                                                   uint32_t payloadType,
                                                   size_t payloadSize)
{
    if(payloadType != USER_DATA_SEI_NUMBER)
        X_THROW(("SEI has wrong payload type."));

    if(data->GetDataSize() - i < payloadSize)
        X_THROW(("Buffer is too short to hold a valid SEI payload."));

    X_UUID uuid;
    copy(data->begin() + i, data->begin() + i + 16, &uuid.uc[0]);
    i += 16;
    payloadSize -= 16;

    XIRef<XMemory> payload = new XMemory;
    payload->Extend(payloadSize);
    copy(data->begin() + i, data->begin() + i + payloadSize, payload->begin());
    i += payload->GetDataSize();

    return new SEIPayload(payload, uuid);
}

