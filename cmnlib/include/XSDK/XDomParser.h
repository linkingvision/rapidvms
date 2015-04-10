
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XDOMPARSER_H_
#define _XSDK_XDOMPARSER_H_

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

class XDomParserTest;

namespace XSDK
{

enum XDomParserIndentType {
    XML_INDENT_TAB = 1,
    XML_INDENT_SPACE = 2
};

class XDomParserNode;

const X_UUID kClsIdXDomParser = {{0xc0,0xc1,0xe2,0x3c,0xa3,0x82,0x49,0x36,0x93,0x70,0x50,0x87,0x8a,0x5a,0x95,0xb9}};

//=====================================================================
/// @brief Our Fast, Simple DOM based XML parser.
//=====================================================================
class XDomParser : public XBaseObject
{
    friend class ::XDomParserTest;

public:
    //=====================================================================
    /// @brief Create a generic XDomParser object.
    //=====================================================================
    X_API XDomParser();

    //=====================================================================
    /// @brief Clean up after ourselves.
    //=====================================================================
    X_API virtual ~XDomParser() throw();

    // Declare IXUnknown
    XSDK_DECLARE_IXUNKNOWN(kClsIdXDomParser);
    XSDK_QUERY_INTERFACE_BEGIN()
    XSDK_QUERY_INTERFACE_END(XSDK::XBaseObject)

    //=====================================================================
    /// @brief Open and load the XML from the file specified.
    /// @param fileNameAndPath is the file name and or relative path.
    //=====================================================================
    X_API void OpenAndSetDocument( const XSDK::XString& fileNameAndPath );

    //=====================================================================
    /// @brief Use an XML document stored in a string to parse from.
    /// @param doc The string containing the XML document.
    //=====================================================================
    X_API void SetDocument( const XSDK::XString& doc );

    //=====================================================================
    /// @brief Return an XML document in a XString generated from the parent.
    /// @param parent The node at the top of the tree to generate XML from.
    //=====================================================================
    X_API XSDK::XString GetDocument( XIRef<XDomParserNode> parent );

    //=====================================================================
    /// @brief Convert the parent into a XML in XString format without the header.
    /// @param parent The node at the top of the tree to generate XML from.
    //=====================================================================
    X_API XSDK::XString GetXmlString( XIRef<XDomParserNode> parent );

    //=====================================================================
    /// @brief Build an in memory DOM from our xml document.
    //=====================================================================
    X_API XIRef<XDomParserNode> Parse();

    //=====================================================================
    /// @brief Search for all the nodes that match tag, and return them in
    /// standard container.
    /// @param tag The string to match against when searching.
    /// @param node The node to begin the search from.
    //=====================================================================
    X_API std::list<XIRef<XDomParserNode> > SearchForAll( const XSDK::XString& tag, XIRef<XDomParserNode> node );

    //=====================================================================
    /// @brief Set the indent character to use when generating XML.
    /// @param indentType Enum value that represents either Tabs or Spaces.
    //=====================================================================
    X_API void SetIndentChar( XDomParserIndentType indentType );

    //=====================================================================
    /// @brief Get the indent character to use when generating XML.
    //=====================================================================
    X_API char GetIndentChar() const;

    //=====================================================================
    /// @brief Set the number of indent characters to use per indent.
    /// @param indentWidth The number of characters to indent.
    //=====================================================================
    X_API void SetIndentWidth( unsigned int indentWidth );

    //=====================================================================
    /// @brief Get number of characters per indent.
    //=====================================================================
    X_API unsigned int GetIndentWidth() const;

    //=====================================================================
    /// @brief Use pretty printing when generating XML.
    //=====================================================================
    X_API void EnablePrettyPrint();

    //=====================================================================
    /// @brief Don't use pretty printing when generating XML.
    //=====================================================================
    X_API void DisablePrettyPrint();

private:
    /// Leave copy constructor unimplemented
    XDomParser( const XDomParser& obj );
    /// Leave assignment operator unimplemented
    XDomParser& operator = ( const XDomParser& obj );

    //=====================================================================
    /// @brief This internal method is phase 1 when parsing. Essentially,
    /// it tokenizes the entire XML document.
    //=====================================================================
    void _CreateTags();

    //=====================================================================
    /// @brief This is the HEART of the xml parser. This recursive function
    /// actually builds an n-array tree representation of our XML document.
    /// @param parent Used by the recursion.
    //=====================================================================
    XIRef<XDomParserNode> _BuildTree( XIRef<XDomParserNode> parent );

    //=====================================================================
    /// @brief Recursive method used to generate XML output.
    /// @param doc The XString to write to.
    /// @param parent The current node to be written.
    /// @param indentLevel Current amount of indenting at this node.
    //=====================================================================
    void _BuildDocument( XSDK::XString& doc, XIRef<XDomParserNode> parent,
                         unsigned int indentLevel );

    //=====================================================================
    /// @brief Search from node for DOM elements that match tag.
    /// @param tag The string to match against tags.
    /// @param node The root of the search.
    //=====================================================================
    void _SearchTree( const XSDK::XString& tag, XIRef<XDomParserNode> node );

    //=====================================================================
    /// @brief Returns a XString that adds indent padding to a value.
    /// @param data The XString which will have indentation added.
    /// @param indentLevel The amount of indenting to apply.
    //=====================================================================
    XSDK::XString _AddIndenting( const XSDK::XString& data, unsigned int indentLevel );

    XSDK::XString _doc;                                      ///< Cache XML document here.
    std::list<XSDK::XString> _tagList;                       ///< Populated by _CreateTags()
    std::list<XSDK::XString>::iterator _tagListIter;         ///< Iterator for tag list
    std::list<XIRef<XDomParserNode> > _nodeSet;           ///< Set of all nodes
    std::list<XIRef<XDomParserNode> > _matchingChildren;  ///< Stores node pointer result from search
    char _indentChar;                                        ///< Character to use when indenting
    unsigned int _indentWidth;                               ///< Width per indentation
    bool _prettyPrint;                                       ///< Whether we should use pretty printing or not
};

}
#endif /* _XSDK_XDOMPARSER_H_ */
