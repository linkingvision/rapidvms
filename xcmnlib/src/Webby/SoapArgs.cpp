
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Webby/SoapArgs.h"

#include <vector>

#include "XSDK/XDomParser.h"

using namespace WEBBY;
using namespace XSDK;
using namespace std;

SoapArgs::SoapArgs()
    : _fakeRoot(new XDomParserNode("<fake>"))
    //_complex()
{
}

SoapArgs::~SoapArgs() throw()
{
}

void SoapArgs::AddArg( const XString& path, const XString& val )
{
    XIRef<XDomParserNode> node = _GetNode(path, _fakeRoot);
    node->SetData(val);
}

void SoapArgs::AddAttribute( const XString& path, const XString& name, const XString& val )
{
    XIRef<XDomParserNode> node = _GetNode(path, _fakeRoot);
    node->AddMetaData(name, val);
}

XIRef<XDomParserNode> SoapArgs::_GetNode( const XString& path, XIRef<XDomParserNode> parent )
{
    const size_t dot = path.find('.');
    XString front = path.substr(0, dot);
    XString rest = dot == string::npos ? "" : path.substr(dot + 1);

    const list<XIRef<XDomParserNode> >::const_iterator end = parent->GetChildren().end();
    list<XIRef<XDomParserNode> >::const_iterator found = end;

    for(list<XIRef<XDomParserNode> >::const_iterator iter = parent->GetChildren().begin();
        iter != end;
        ++iter)
    {
        if((*iter)->GetTagName() == front)
        {
            found = iter;
            break;
        }
    }

    if(found != end)
        return rest.empty() ? *found : _GetNode(rest, *found);

    XIRef<XDomParserNode> node(new XDomParserNode(XString::Format("<%s>", front.c_str())));
    parent->AppendChild(node);

    if(!rest.empty())
        return _GetNode(rest, node);

    return node;


    /*
    // Key - "foo.bar.alpha"
    // In this case, foo and bar are CONTAINER_TYPE and alpha is a VALUE_TYPE

    SoapArgs* existingNode = current->Find( path);

    if(existingNode)
    {
    }

    vector<XString> parts;
    key.Split( ".", parts );

    XHash<SoapArgsNode>* current = &_complex;


    if( !parts.empty() )
    {
        XString currPath = parts[0];
        size_t limit = parts.size() - 1;

        // Loop over all the CONTAINER_TYPE parts of the path...
        for( size_t i = 0; i < limit; ++i )
        {
            SoapArgsNode* found = current->Find( currPath );

            if( found )
            {
                current = &found->_children;
            }
            else
            {
                SoapArgsNode node;
                node._nodeType = CONTAINER_TYPE;
                node._name = parts[i];
                current->Add( currPath, node );
                SoapArgsNode* foundNode = current->Find( currPath );
                current = &foundNode->_children;
            }

            currPath += "." + parts[i];
        }

        SoapArgsNode* found == current->Find( path );

        if(found)
        {
            SoapArgs node = *found;
            current->Remove( parts[i] );

            return found;
        }
    }

    SoapArgsNode node;
    node._nodeType = VALUE_TYPE;
    node._name = !parts.empty() ? parts[i] : key;
    */
}


XString SoapArgs::ToXML() const
{
    XDomParser parser;
    XString xml;

    for(list<XIRef<XDomParserNode> >::const_iterator iter = _fakeRoot->GetChildren().begin(),
                                                        end  = _fakeRoot->GetChildren().end();
        iter != end;
        ++iter)
    {
        xml += parser.GetXmlString(*iter);
    }

    return xml;

    //return _ToXMLInternal( _complex );
}

/*
XString SoapArgs::_ToXMLInternal( const XSDK::XHash<SoapArgsNode>& current ) const
{
    XDomParser parser;
    XString xml;

    for(list<XIRef<XDomParserNode> >::const_iterator iter = _fakeRoot.GetChildren().begin(),
                                                        end  = _fakeRoot.GetChildren().end();
        iter != end;
        ++iter)
    {
        xml += parser.GetXmlString(*iter);
    }

    return xml;

    XString xml;

    XHash<SoapArgsNode>::XHashIter iter = current.GetIterator();

    for( ; iter.IterValid(); iter.Iterate() )
    {
        SoapArgsNode node = *iter.IterData();

        if( node._nodeType == VALUE_TYPE )
        {
            xml += XString::Format( "<%s", node._name.c_str());

            for(vector<pair<XString, XString> >::const_iterator iter = node._attributes.begin(),
                                                                end  = node._attributes.end();
                iter != end;
                ++iter)
            {
                xml += XString::Format( " %s=\"%s\"", iter->first.c_str(), iter->second.c_str());
            }

            xml += XString::Format( ">%s</%s>", node._value.c_str(), node._name.c_str() );
        }
        else
        {
            xml += XString::Format( "<%s>", node._name.c_str() );

            xml += _ToXMLInternal( node._children );

            xml += XString::Format( "</%s>", node._name.c_str() );
        }
    }

    return xml;
}
*/
