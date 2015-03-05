
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XDOMPARSERNODE_H_
#define _XSDK_XDOMPARSERNODE_H_

#include <list>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <vector>

#include "XSDK/XSharedLib.h"
#include "XSDK/XException.h"
#include "XSDK/XString.h"
#include "XSDK/XHash.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XIWeakRef.h"

class XDomParserNodeTest;

namespace XSDK
{

class XDomParser;

const X_UUID kClsIdXDomParserNode = {{0xa6,0x6c,0x82,0xbf,0x20,0xb6,0x4d,0x51,0xa0,0xfd,0xc6,0xd6,0x59,0x78,0xe0,0x19}};

//=========================================================================
/// @brief This object represents a single DOM element.
//=========================================================================
class XDomParserNode : public XBaseObject
{
    friend class XDomParser;
    friend class ::XDomParserNodeTest;

public:
    //=====================================================================
    /// @brief Create a XDomParserNode.
    //=====================================================================
    X_API XDomParserNode();

    //=====================================================================
    /// @brief Create a XDomParserNode as a copy of the passed object.
    /// @param tag Tag name for creating a new node
    //=====================================================================
    X_API XDomParserNode( const XSDK::XString& tag );

    //=====================================================================
    /// @brief Create a XDomParserNode as a copy of the passed object.
    /// @param obj The object to copy from when creating this.
    //=====================================================================
    X_API XDomParserNode( const XDomParserNode& obj );

    //=====================================================================
    /// @brief Destructor
    //=====================================================================
    X_API virtual ~XDomParserNode() throw () {}

    //=====================================================================
    /// @brief Set this as a copy of the passed object.
    /// @param obj The object we'd like to copy from.
    //=====================================================================
    X_API XDomParserNode& operator=( const XDomParserNode& obj ) { return _Copy( obj ); }

    // Declare IXUnknown
    XSDK_DECLARE_IXUNKNOWN(kClsIdXDomParserNode);
    XSDK_QUERY_INTERFACE_BEGIN()
    XSDK_QUERY_INTERFACE_END(XSDK::XBaseObject)

    //=====================================================================
    /// @brief Returns the metadata associated with the passed name, on this node.
    /// @param name The metadata name you're interested in.
    //=====================================================================
    X_API XSDK::XString GetMetaData( const XSDK::XString& name ) const;

    //=====================================================================
    /// @brief Adds metadata to this tag.
    /// @param name The name of the metadata field you'd like added.
    /// @param value The value of the matadate field you are adding.
    //=====================================================================
    X_API void AddMetaData( const XSDK::XString& name, const XSDK::XString& value );

    //=====================================================================
    /// @brief Get hash of all metadata
    //=====================================================================
    X_API XSDK::XHash<XSDK::XString> GetMetaDataSet() const { return _metaData; }

    //=====================================================================
    /// @brief Returns the data section for this node.
    //=====================================================================
    X_API XSDK::XString GetData() const { return _data; }

    //=====================================================================
    /// @brief Set the data section for this node.
    /// @param newData The new data section for this node.
    //=====================================================================
    X_API void SetData( const XSDK::XString& newData );

    //=====================================================================
    /// @brief Returns a list of XDomParserNode*'s that point to the children of this node.
    //=====================================================================
    X_API const std::list<XIRef<XDomParserNode> >& GetChildren() const
    { return _children; }

    //=====================================================================
    /// @brief Append a child node to the current XDomParserNode.
    //=====================================================================
    X_API void AppendChild( XIRef<XDomParserNode> newChild );

    //=====================================================================
    /// @brief Returns a XDomParserNode* that points to the parent of this node.
    //=====================================================================
    X_API XIRef<XDomParserNode> GetParent() const { return _parent; }

    //=====================================================================
    /// @brief Sets the parent node for this node.
    /// @param parent The new parent for this node.
    //=====================================================================
    X_API void SetParent( XIRef<XDomParserNode> newParent )
    { _parent = newParent; }

    //=====================================================================
    /// @brief Returns the name of the current tag.
    //=====================================================================
    X_API XSDK::XString GetTagName() const { return _tagName; }

    //=====================================================================
    /// @brief Sets the tag name for this node.
    //=====================================================================
    X_API void SetTagName( const XSDK::XString& tagName )
    { _tagName = tagName; }

    //=====================================================================
    /// @brief Returns a begin tag including all metadata.
    //=====================================================================
    X_API XSDK::XString GetBeginTag();

    //=====================================================================
    /// @brief Returns an end tag.
    //=====================================================================
    X_API XSDK::XString GetEndTag() const;

    //=====================================================================
    /// @brief Check if this has an end tag
    //=====================================================================
    X_API bool HasEndTag() const { return _hasEndTag; }

    //=====================================================================
    /// @brief Returns the nodes at the given dot-separated path. \ can be
    ///        used to escape the dot.
    //=====================================================================
    X_API std::vector<XIRef<XDomParserNode> > GetPath( const XSDK::XString& path ) const;

private:
    //=====================================================================
    /// @brief Copy to this-> from the passed object.
    /// @param obj The object we'd like to copy from.
    //=====================================================================
    X_API XDomParserNode& _Copy( const XDomParserNode& obj );

    //=====================================================================
    /// @brief Parses out metadata and stores in member variables.
    //=====================================================================
    void _ParseMetaData( const XSDK::XString& tag );

    //=====================================================================
    /// @brief Parses out the data section of a tag in string.
    //=====================================================================
    void _ParseDataSection( const XSDK::XString& tag );

    //=====================================================================
    /// @brief Parse out tag name
    //=====================================================================
    void _ParseTagName( const XSDK::XString& tag );

    //=====================================================================
    /// @brief Parse out end tag
    //=====================================================================
    void _ParseEndTag( const XSDK::XString& tag );

    //=====================================================================
    /// @brief Parses a dot-separated path into its separate pieces, use
    ///        . as a delimeter. \ escapes the dot.
    //=====================================================================
    X_API static std::vector<XString> _ParsePath( const XString& path );

    //=====================================================================
    /// @brief Helper function for _GetPath.
    ///
    /// @param The current node in the traversal.
    /// @param An iterator to the first element in the remaining portion of
    ///        the path to the target node.
    /// @param An iterator to the end of the container holding the path to
    ///        the target node.
    /// @param The nodes collected thus far.
    //=====================================================================
    template <typename I>
    static void _GetPath(XIRef<XDomParserNode> currNode,
                         I pathIter,
                         I pathEnd,
                         std::vector<XIRef<XDomParserNode> >& nodes);

    XSDK::XString _tagName;                               ///< Holds name of the tag
    XSDK::XString _data;                                  ///< Holds data section of tag
    XIWeakRef<XDomParserNode> _parent;                ///< Point to parent node
    std::list<XIRef<XDomParserNode> > _children;  ///< List of children tags
    XSDK::XHash<XSDK::XString> _metaData;                 ///< XHash that holds tag metadata
    bool _hasEndTag;                                      ///< Tells us if it has a begin and end or just single tag
};


    class XDomParserException : public XException
    {
    public:

        X_API XDomParserException();

        X_API virtual ~XDomParserException() throw();

        X_API XDomParserException(const char* msg, ...);

        X_API XDomParserException(const XSDK::XString& msg);
    };

}

#endif /* _XSDK_DOMPARSERNODE_H_ */
