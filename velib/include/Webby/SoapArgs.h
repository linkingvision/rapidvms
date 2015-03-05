
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef Webby_SoapArgs_h
#define Webby_SoapArgs_h

#include <vector>
#include <map>

#include "XSDK/XHash.h"
#include "XSDK/XString.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XDomParserNode.h"

namespace WEBBY
{

class SoapArgs : public XSDK::XBaseObject
{
public:

    X_API SoapArgs();

    X_API virtual ~SoapArgs() throw();

    /// \brief Adds 'val' to arguments at position 'path'.
    ///
    /// 'path' specifies a position in a complex type. Components of path are separated by
    /// '.' (for example, 'foo.bar.alpha'). These components describe a heirarchy.
    X_API void AddArg( const XSDK::XString& path, const XSDK::XString& val );

    /// \brief Adds the attribute 'name' with value 'val' to the node at position 'path'.
    ///
    /// 'path' specifies a position in a complex type. Components of path are separated by
    /// '.' (for example, 'foo.bar.alpha'). These components describe a heirarchy.
    X_API void AddAttribute( const XSDK::XString& path, const XSDK::XString& name, const XSDK::XString& val );

    /// \brief Converts the arguments contained in this SoapArgs into XML.
    X_API XSDK::XString ToXML() const;

private:
    SoapArgs( const SoapArgs& obj );
    SoapArgs& operator = ( const SoapArgs& obj );

    XIRef<XSDK::XDomParserNode> _GetNode( const XSDK::XString& path, XIRef<XSDK::XDomParserNode> parent );

    XIRef<XSDK::XDomParserNode> _fakeRoot;
};

}

#endif
