
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _H264UserData_H_
#define _H264UserData_H_

#include <vector>

#include "XSDK/Types.h"
#include "XSDK/XRef.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XMemory.h"
#include "XSDK/XString.h"
#include "XSDK/XUuid.h"

class H264UserDataTest;

namespace MEDIA_PARSER
{

///
class SEIPayload : public XSDK::XBaseObject
{
public:

    /// Constructor
    ///
    /// AFAIK, the UUID really doesn't matter unless we decide to do something
    /// in particular with it. The default comes from uuidgen and has no
    /// significance. You should be able to ignore the fact that a UUID is used
    /// at all.
    X_API SEIPayload(const XIRef<XSDK::XMemory> data,
                     const XSDK::XUuid& uuid = XSDK::XString("7e0858c4-38fe-48ea-852d-dace39badb30"))
        : XBaseObject(),
          _data(data),
          _uuid(uuid)
    {}

    ///
    X_API SEIPayload(const uint8_t* data,
                     size_t length,
                     const XSDK::XUuid uuid = XSDK::XString("7e0858c4-38fe-48ea-852d-dace39badb30"));

    ///
    X_API const XIRef<XSDK::XMemory> GetData() const {return _data;}

    ///
    X_API const XSDK::XUuid& GetUUID() const {return _uuid;}

private:

    XIRef<XSDK::XMemory> _data;
    XSDK::XUuid _uuid;
};


///
class H264UserData
{
    friend class ::H264UserDataTest;
public:

    /// Reads in an existing SEI nal unit from a buffer.
    X_API H264UserData(const uint8_t* buffer, size_t length);

    ///
    X_API H264UserData(XRef<SEIPayload> payload)
        : _payloads(1, payload)
    {}

    ///
    X_API H264UserData(std::vector<XRef<SEIPayload> > payloads)
        : _payloads(payloads)
    {
        X_ASSERT(!_payloads.empty());
    }

    ///
    X_API XIRef<XSDK::XMemory> GenSEI() const;

    ///
    X_API const std::vector<XRef<SEIPayload> >& GetPayloads() const {return _payloads;}

private:

    static void _AddPayloadData(XIRef<XSDK::XMemory> data, const XIRef<XSDK::XMemory> payloadData);
    static void _AddPayloadNum(XIRef<XSDK::XMemory> data, int n);
    static void _AddSEIMessage(XIRef<XSDK::XMemory> data, XRef<SEIPayload> payload);
    static void _AddUUID(XIRef<XSDK::XMemory> data, const X_UUID& uuid);
    X_API static XIRef<XSDK::XMemory> _Denalify(const uint8_t* buffer, size_t length);
    X_API static XIRef<XSDK::XMemory> _Nalify(const XIRef<XSDK::XMemory> data);
    static XRef<SEIPayload> _ProcessSEIMessage(const XIRef<XSDK::XMemory> data, size_t& i);
    static XRef<SEIPayload> _ProcessPayload(const XIRef<XSDK::XMemory> data,
                                            size_t& i,
                                            uint32_t payloadType,
                                            size_t payloadSize);

    std::vector<XRef<SEIPayload> > _payloads;
};

}

#endif
