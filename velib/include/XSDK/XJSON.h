
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XJSON_H_
#define _XSDK_XJSON_H_

#include <list>
#include <iostream>
#include <fstream>
#include <cstdarg>

#include "XSDK/XSharedLib.h"
#include "XSDK/XException.h"
#include "XSDK/Errors.h"
#include "XSDK/XString.h"
#include "XSDK/XHash.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XVariant.h"


namespace XSDK
{
    const X_UUID kClsIdXJSON = {{0xc0,0xc1,0xe2,0x3c,0xa3,0x82,0x49,0x36,0x93,0x70,0x50,0x87,0x8a,0x5a,0x95,0xb9}};

    typedef enum _XJSONType
    {
        XJSONObjectType,
        XJSONArrayType,
        XJSONValueType,
    }XJSONType;

    class XJSONException : public XException
    {
    public:

        X_API XJSONException();

        X_API virtual ~XJSONException() throw();

        X_API XJSONException(const char* msg, ...);

        X_API XJSONException(const XSDK::XString& msg);
    };

    class XJSONItem : public XBaseObject
    {
    public:
        XJSONItem();
        virtual ~XJSONItem()
        {}

        XSDK_DECLARE_IXUNKNOWN(kClsIdXJSON);
        XSDK_QUERY_INTERFACE_BEGIN()
        XSDK_QUERY_INTERFACE_END(XSDK::XBaseObject)

        X_API virtual XJSONType GetType()=0;

        X_API static XIRef<XJSONItem> ParseDocument(const XString& document);

        X_API virtual size_t Parse(XSDK::XString& str)=0;

        X_API virtual bool HasIndex( const XSDK::XString& index );
        X_API virtual XIRef<XJSONItem> Index(const XSDK::XString& index);
        X_API virtual bool HasIndex( const size_t& index );
        X_API virtual XIRef<XJSONItem> Index(const size_t& index);
        X_API virtual size_t Count();

        template<class T>
        X_API T Get()
        {
           if ( GetType() != XJSONValueType )
              X_STHROW(XJSONException,("Cannot call get on non XJSONValue type json element"));
           return _data.Get<T>();
        }


        X_API static XIRef<XJSONItem> Find(XIRef<XJSONItem> json, XSDK::XString slashDelimitedPath);

    protected:

        static size_t GetNext(const XString& str, size_t from, bool& isBrace);
        static size_t SearchForNonWhiteSpace(const XString& str, size_t idx);
        static void CheckValidIndex(const size_t& index);
        static size_t ParseString(const XString& str, size_t start, XString& res);

    protected:
        XVariant _data;
        XHash< XIRef<XJSONItem> > _objectDefinition;
        std::vector< XIRef<XJSONItem> > _arrayItems;

    };



    class XJSONValue : public XJSONItem
    {
    public:
        XJSONValue();
        virtual ~XJSONValue();

        X_API XJSONType GetType();

        X_API XVariant& GetValue();

        X_API bool IsNull();

        X_API size_t Parse(XString& str);
    };


    class XJSONObject : public XJSONItem
    {
    public:
        XJSONObject();
        virtual ~XJSONObject();

        X_API XJSONType GetType();

        X_API XHash< XIRef<XJSONItem> >& GetObjectContents();

        X_API XIRef<XJSONItem> GetObjectMember(const XSDK::XString& name);

        X_API size_t Parse(XSDK::XString& str);

        X_API bool HasIndex(const XSDK::XString& index);
        X_API XIRef<XJSONItem> Index(const XSDK::XString& index);


    };

    class XJSONArray : public XJSONItem
    {
    public:
        XJSONArray();
        virtual ~XJSONArray();

        X_API XJSONType GetType();

        X_API std::vector< XIRef<XJSONItem> > GetArray();

        X_API XIRef<XJSONItem> AccessElement(const size_t& index);

        X_API size_t Parse(XSDK::XString& str);

        X_API bool HasIndex(const size_t& index);

        X_API XIRef<XJSONItem> Index(const size_t& index);

        X_API size_t Count();

    };


}



#endif /* _XSDK_XJSON_H_ */
