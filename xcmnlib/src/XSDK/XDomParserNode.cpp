
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XDomParserNode.h"
#include "XSDK/XDomParser.h"
#include "XSDK/OS.h"

#include <algorithm>
#include <cctype>
#include <vector>
#include <cstring>

using namespace std;
using namespace XSDK;


XDomParserNode::XDomParserNode() :
    _tagName(),
    _data(),
    _parent(),
    _children(),
    _metaData(),
    _hasEndTag( true )
{
}

XDomParserNode::XDomParserNode( const XString& tag ) :
    _tagName(),
    _data(),
    _parent(),
    _children(),
    _metaData(),
    _hasEndTag( true )
{
    _ParseTagName( tag );
    _ParseMetaData( tag );
    _ParseDataSection( tag );
    _ParseEndTag( tag );
}

XDomParserNode::XDomParserNode( const XDomParserNode& obj ) :
    _tagName(obj._tagName),
    _data(obj._data),
    _parent(obj._parent),
    _children(obj._children),
    _metaData(obj._metaData),
    _hasEndTag(obj._hasEndTag)
{
}

XString XDomParserNode::GetMetaData( const XString& name ) const
{
    XString* keyValue = _metaData.Find( name );

    if ( keyValue )
        return *keyValue;
    else
        return "";
}

void XDomParserNode::AddMetaData( const XSDK::XString& name, const XSDK::XString& value )
{
    XString* keyValue = _metaData.Find( name );

    if ( keyValue )
        _metaData.Remove( name );

    _metaData.Add( name, value );
}

void XDomParserNode::SetData( const XString& newData )
{
    if ( _data.empty() )
        _hasEndTag = true;

    _data = newData;
}

void XDomParserNode::AppendChild( XIRef<XDomParserNode> newChild )
{
    newChild->SetParent( this );
    _children.push_back( newChild );
}

XString XDomParserNode::GetBeginTag()
{
    XString tag = XString::Format( "<%s", _tagName.c_str() );

    for ( _metaData.IterStart(); _metaData.IterValid();
          _metaData.Iterate() )
    {
        XString name = _metaData.IterKey();
        XString value = *_metaData.IterData();
        tag = XString::Format( "%s %s=\"%s\"", tag.c_str(), name.c_str(), value.c_str() );
    }

    if ( HasEndTag() )
        tag += ">";
    else
        tag += " />";

    return tag;
}

XString XDomParserNode::GetEndTag() const
{
    if ( HasEndTag() )
        return XString::Format( "</%s>", _tagName.c_str() );

    X_STHROW(XDomParserException, ("No end tag for [%s]", _tagName.c_str()));
}

std::vector<XIRef<XDomParserNode> > XDomParserNode::GetPath(const XString& path ) const
{
    vector<XString> pathList = _ParsePath(path);
    vector<XIRef<XDomParserNode> > nodes;

    if(!pathList.empty())
        _GetPath(const_cast<XDomParserNode*>(this), pathList.begin(), pathList.end(), nodes);

    return nodes;
}

XDomParserNode& XDomParserNode::_Copy( const XDomParserNode& obj )
{
    if ( &obj != this )
    {
        _tagName = obj._tagName;
        _data = obj._data;
        _parent = obj._parent;
        _children = obj._children;
        _metaData = obj._metaData;
        _hasEndTag = obj._hasEndTag;
    }
    return *this;
}

void XDomParserNode::_ParseMetaData( const XString& tag )
{
    vector<XString> mainParts;
    tag.Split( '>', mainParts );

    if ( mainParts.size() <= 0 )
        _metaData.Clear();

    const char* beginPtr = mainParts.begin()->c_str();
    size_t tagLen = mainParts.begin()->length();
    const char* endPtr = beginPtr + tagLen;
    const char* sentry = beginPtr;

    while( sentry < endPtr )
    {
        // Bypass spaces
        sentry = strchr( sentry, ' ' );
        if ( NULL == sentry )
            break;
        for( ; *sentry == ' ' && sentry < endPtr; ++sentry );

        // Find end of attribute name
        const char* endValuePtr = strchr( sentry, '=' );
        if ( NULL == endValuePtr )
            break;

        // Store attribute name
        XString key;
        key.assign( sentry, ( endValuePtr - sentry ) );
        sentry = endValuePtr + 2;

        // Find end of attribute value
        endValuePtr = strchr( sentry, '"' );
        if ( NULL == endValuePtr )
            break;

        // Store attribute value
        XString value;
        value.assign( sentry, ( endValuePtr - sentry ) );

        _metaData.Add( key, value );
        sentry = endValuePtr;
    }
}

void XDomParserNode::_ParseDataSection( const XString& tag )
{
    XString data = tag;

    // Erase the begin tag
    size_t pos = data.find_first_of( '>' );

    if ( data.size() > pos + 1 )
    {
        data.erase( 0, pos + 1 );
        // Look for the end tag
        pos = data.find_first_of( '<' );
        if ( pos != string::npos )
            data.erase( pos );

        data.StripInPlace();

        if ( data.size() > 0 )
            _data = data;
        else
            _data.clear();
    }
    else
        _data.clear();
}

void XDomParserNode::_ParseTagName( const XString& tag )
{
    XString tagName = tag;
    size_t pos = 0;

    pos = tagName.find_first_of( '<' );
    if ( pos != string::npos )
        tagName.erase( 0, pos + 1 );
    else
        X_STHROW(XDomParserException, ("Invalid XML: missing '<' when parsing [%s]", tagName.c_str()));

    pos = tagName.find_first_of( " \t\r\n>" );
    if ( pos != string::npos )
        tagName.erase( pos );
    else
        X_STHROW(XDomParserException, ("Invalid XML: missing space or '>' when parsing [%s]", tagName.c_str()));

    tagName.StripInPlace();

    int tagLen = (int)tagName.length();
    unsigned int colonIndex = (unsigned int)tagName.find( ':' );
    if( (colonIndex != string::npos) &&
        ((tagLen-colonIndex) > 1) )
    {
        int nameLen = (tagLen - colonIndex);
        tagName = tagName.substr( colonIndex+1, nameLen );
    }

    _tagName = tagName;
}

void XDomParserNode::_ParseEndTag( const XString& tag )
{
    XString tagName = tag;
    if ( tagName.EndsWith( "/>" ) )
        _hasEndTag = false;
    else if ( tagName.find( ">" ) != string::npos )
        _hasEndTag = true;
    else
        X_STHROW(XDomParserException, ("Invalid XML: missing '>' when parsing [%s]", tagName.c_str()));
}

vector<XString> XDomParserNode::_ParsePath( const XString& path )
{
    vector<XString> pathList;

    if(!path.empty())
    {
        size_t begin = 0;
        size_t i = 0;

        while(i != string::npos)
        {
            i = path.find('.', i);

            if(i != string::npos)
            {
                if(i > 0 && path[i - 1] != '\\')
                {
                    XString pathPart = path.substr(begin, i - begin);

                    if(pathPart.find('\\'))
                        pathList.push_back(pathPart.EraseAll('\\'));
                    else
                        pathList.push_back(pathPart);

                    begin = ++i;
                }
                else
                    ++i;
            }
            else
                pathList.push_back(path.substr(begin));
        }
    }

    return pathList;
}

template <typename I>
void XDomParserNode::_GetPath(XIRef<XDomParserNode> currNode,
                              I pathIter,
                              I pathEnd,
                              vector<XIRef<XDomParserNode> >& nodes)
{
    X_ASSERT(currNode.IsValid());
    X_ASSERT(pathIter != pathEnd);

    if(currNode->GetTagName() == *pathIter)
    {
        if(++pathIter == pathEnd)
            nodes.push_back(currNode);
        else
        {
            for(list<XIRef<XDomParserNode> >::const_iterator childIter = currNode->GetChildren().begin(),
                                                                      childEnd  = currNode->GetChildren().end();
                childIter != childEnd;
                ++childIter)
            {
                _GetPath(*childIter, pathIter, pathEnd, nodes);
            }
        }
    }
}


XDomParserException::XDomParserException()
    : XException()
{}

XDomParserException::~XDomParserException() throw()
{}

XDomParserException::XDomParserException(const char* msg, ...)
    : XException()
{
    va_list args;
    va_start(args, msg);
    SetMsg(XString::Format(msg, args));
    va_end(args);
}

XDomParserException::XDomParserException(const XString& msg)
    : XException(msg)
{}

