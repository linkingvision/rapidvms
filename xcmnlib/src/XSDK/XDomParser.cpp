
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XDomParser.h"
#include "XSDK/XDomParserNode.h"
#include "XSDK/OS.h"

#include <algorithm>
#include <cctype>
#include <vector>
#include <cstring>

using namespace std;
using namespace XSDK;


XDomParser::XDomParser() :
    _doc(),
    _tagList(),
    _tagListIter(),
    _nodeSet(),
    _matchingChildren(),
    _indentChar( ' ' ),
    _indentWidth( 4 ),
    _prettyPrint( false )
{
}

XDomParser::~XDomParser() throw()
{
}

void XDomParser::OpenAndSetDocument( const XString& fileNameAndPath )
{
    FILE* inFile = x_fopen( fileNameAndPath, "r" );

    _doc.clear();

    while( !feof( inFile ) )
    {
        char lineBuffer[1024];
        if( fgets( lineBuffer, 1024, inFile ) == NULL )
            continue;
        _doc += lineBuffer;
    }

    if( fclose( inFile ) == EOF )
        X_STHROW(XDomParserException, ( "Could not close file" ));
}

void XDomParser::SetDocument( const XString& doc )
{
    _doc = doc;
}

XString XDomParser::GetDocument( XIRef<XDomParserNode> parent )
{
    XString doc;
    doc += _AddIndenting( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>", 0 );
    _BuildDocument( doc, parent, 0 );
    return doc;
}

XString XDomParser::GetXmlString( XIRef<XDomParserNode> parent )
{
    XString doc;
    _BuildDocument( doc, parent, 0 );
    return doc;
}

XIRef<XDomParserNode> XDomParser::Parse()
{
    _CreateTags();

    if ( _tagList.empty() )
        X_STHROW( XDomParserException, ( "XML File Format Invalid." ));

    _tagListIter = _tagList.begin();

    return _BuildTree( NULL );
}

list<XIRef<XDomParserNode> > XDomParser::SearchForAll( const XString& tag, XIRef<XDomParserNode> node )
{
    _matchingChildren.clear();

    if( tag.length() == 0 )
        return _matchingChildren;

    if( node != NULL )
        _SearchTree( tag, node );

    return _matchingChildren;
}

void XDomParser::SetIndentChar( XDomParserIndentType indentType )
{
    switch(indentType)
    {
    case XML_INDENT_TAB:
        _indentChar = '\t';
        break;
    case XML_INDENT_SPACE:
        _indentChar = ' ';
        break;
    default:
        X_STHROW( XDomParserException, ("Invalid Indent Type." ));
        break;
    }
}

char XDomParser::GetIndentChar() const
{
    return _indentChar;
}

void XDomParser::SetIndentWidth( unsigned int indentWidth )
{
    _indentWidth = indentWidth;
}

unsigned int XDomParser::GetIndentWidth() const
{
    return _indentWidth;
}

void XDomParser::EnablePrettyPrint()
{
    _prettyPrint = true;
}

void XDomParser::DisablePrettyPrint()
{
    _prettyPrint = false;
}

void XDomParser::_CreateTags()
{
    if( _doc.size() == 0 )
        return;

    const char* beginTag = _doc.c_str();
    const char* endTag = _doc.c_str();
    const char* sentry = _doc.c_str() + _doc.length();
    bool done = false;

    while( !done )
    {
        // Move begin pointer to beginning of tag name
        for( ; ( beginTag < sentry ) && *beginTag != '<'; ++beginTag );

        // Skip if this is an XML comment
        if( ( beginTag < sentry ) && ( *( beginTag + 1 ) == '!' ) )
        {
            for( ; ( strncmp( beginTag, "-->", 3 ) != 0 ) &&
                     beginTag < sentry; ++beginTag );
            continue;
        }

        // Skip XML headers
        if( ( beginTag < sentry ) && ( *( beginTag + 1 ) == '?' ) )
        {
            for( ; ( strncmp( beginTag, "?>", 2 ) != 0 ) &&
                     beginTag < sentry; ++beginTag );
            continue;
        }

        endTag = beginTag + 1;

        for( ; ( endTag < sentry ) && *( endTag + 1 ) != '<'; ++endTag );

        if( ( beginTag < sentry ) && *beginTag == '<' )
        {
            size_t tagLen = ( endTag - beginTag ) + 1;
            XString tag;
            tag.assign( beginTag, tagLen );
            tag.StripInPlace();
            _tagList.push_back( tag );
        }

        beginTag = endTag;

        if( ( beginTag + 1 ) >= sentry )
            done = true;
    }
}

XIRef<XDomParserNode> XDomParser::_BuildTree( XIRef<XDomParserNode> parent )
{
    XIRef<XDomParserNode> emptyNode;

    if( _tagList.empty() )
        return emptyNode;

    XString currentTag = *_tagListIter;

    if( currentTag[1] == '/' )
    {
        // Check to make sure end tag was well formed
        if ( currentTag.find( ">" ) != string::npos )
            return emptyNode;
        else
            X_STHROW( XDomParserException, ( "Invalid XML: bad end tag when parsing [%s]", currentTag.c_str()));
    }
    else
    {
        XIRef<XDomParserNode> newNode = new XDomParserNode( currentTag );

        if( newNode->HasEndTag() )
        {
            _nodeSet.push_front( newNode );
            ++_tagListIter;

            for( ; _tagListIter != _tagList.end(); )
            {
                XIRef<XDomParserNode> newChild = _BuildTree( newNode );

                if ( newChild.IsValid() )
                    newNode->AppendChild( newChild );
                else
                    break;

                // Your probably wondering... "Hey, why doesn't he just increment _tagListIter in the
                // for loop?
                // Well, we used to... But it caused "undefined" behaviour.. Because, as we recurse
                // we actually incrment _tagListIter (line 224)... That increment causes it to == end().
                // If we do it as part of the for loop, then another ++ will happen unconditionally...
                // which is undefined... As of this change, we increment it at the bottom of the loop
                // only if it is not equal to end().
                // (the old code caused an infinite recursion segfault with xml like this:
                //    <foo><bar>
                if( _tagListIter != _tagList.end() )
                    _tagListIter++;
            }
        }

        return newNode;
    }
}

void XDomParser::_SearchTree( const XString& tag, XIRef<XDomParserNode> node )
{
    list<XIRef<XDomParserNode> > kids = node->GetChildren();

    if( node->GetTagName() == tag )
        _matchingChildren.push_back( node );

    for ( list<XIRef<XDomParserNode> >::iterator it = kids.begin();
          it != kids.end(); ++it )
    {
        _SearchTree( tag, *it );
    }
}

void XDomParser::_BuildDocument( OUT XString& doc, XIRef<XDomParserNode> parent, unsigned int indentLevel )
{
    X_ASSERT( parent.IsValid() );

    doc += _AddIndenting( parent->GetBeginTag(), indentLevel );

    if ( parent->HasEndTag() )
    {
        if ( !parent->GetData().empty() )
            doc += _AddIndenting( parent->GetData(), indentLevel + 1 );

        list<XIRef<XDomParserNode> > children = parent->GetChildren();

        for ( list<XIRef<XDomParserNode> >::iterator it = children.begin(); it != children.end(); ++it )
        {
            _BuildDocument( doc, *it, indentLevel + 1 );
        }

        doc += _AddIndenting( parent->GetEndTag(), indentLevel );
    }
}

XString XDomParser::_AddIndenting( const XString& data, unsigned int indentLevel )
{
    int numChar = indentLevel * _indentWidth;
    XString newData;

    if ( numChar > 0 && _prettyPrint )
        newData.insert( 0, numChar, _indentChar );

    newData += data;

    if ( _prettyPrint )
        newData += '\n';

    return newData;
}

