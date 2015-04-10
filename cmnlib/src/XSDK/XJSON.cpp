
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XJSON.h"
#include "XSDK/OS.h"
#include "XSDK/XListBox.h"

using namespace XSDK;

static const char BRACE = '{';
static const char BRACKET = '[';
static const char ESCAPED_SLASH = '\\';
static const XString ESCAPED_QUOTE = "\"";
static const char FALSE_START = 'f';
static const char TRUE_START = 't';
static const char NULL_START = 'n';
static const size_t TRUE_LENGTH = 5;
static const size_t FALSE_LENGTH = 6;
static const size_t NULL_LENGTH = 5;
static const char BRACE_CLOSE = '}';
static const char BRACKET_CLOSE = ']';
static const char COMMA = ',';
static const char COLON = ':';


XJSONItem::XJSONItem():XBaseObject(),
_data(),
_objectDefinition(),
_arrayItems()
{
}

bool XJSONItem::HasIndex(const XSDK::XString& index)
{
   X_STHROW(XJSONException,("Not Implemented"));
}

XIRef<XJSONItem> XJSONItem::Index(const XSDK::XString& index)
{
   X_STHROW(XJSONException,("Not Implemented"));
}

bool XJSONItem::HasIndex(const size_t& index)
{
   X_STHROW(XJSONException,("Not Implemented"));
}

XIRef<XJSONItem> XJSONItem::Index(const size_t& index)
{
   X_STHROW(XJSONException,("Not Implemented"));
}

size_t XJSONItem::Count()
{
   X_STHROW(XJSONException,("Not Implemented"));
}


XIRef<XJSONItem> XJSONItem::Find(XIRef<XJSONItem> json, XSDK::XString slashDelimitedPath)
{
    const static char delimiter = '/';
    XIRef<XJSONItem> result;
    if ( json.IsEmpty() )
    {
        X_LOG_INFO("Arg is empty, so returning NULL");
        return result;
    }
    std::vector<XString> parts;
    slashDelimitedPath.Split(delimiter,parts);
    XIRef<XJSONItem> current = json;
    for ( size_t i = 0; i < parts.size(); ++i )
    {

       // current is re-assigned below.  It is possible that current could be set to NULL if the previous
       // iteration did not find the previous item in the slashDelimitedPath
       if( ! current.IsValid() )
           return current;

       if ( parts[i].IsInteger() )
       {//Means we want to index an item in an array
           if ( current->GetType() != XJSONArrayType )
               X_STHROW(XJSONException, ("The %d(th) item in the path(%s) does not match JSON structure!",i,slashDelimitedPath.c_str()));
           XIRef<XJSONArray> ary(current);
           current = ary->AccessElement(parts[i].ToInt());
       }
       else
       {//Means we are looking by name, in a hash
           if ( current->GetType() != XJSONObjectType )
               X_STHROW(XJSONException, ("The %d(th) item in the path(%s) does not match JSON structure!",i,slashDelimitedPath.c_str()));
           XIRef<XJSONObject> obj(current);
           current = obj->GetObjectMember(parts[i]);

       }
    }
    return current;
}


size_t XJSONItem::GetNext(const XString& str, size_t from, bool& isBrace)
{
    size_t brace = str.find_first_of(BRACE,from);
    size_t bracket = str.find_first_of(BRACKET,from);
    if ( brace == std::string::npos && bracket == std::string::npos )
        return std::string::npos;
    if ( (isBrace = (brace < bracket)) )
        return brace;
    else return bracket;
}

size_t XJSONItem::SearchForNonWhiteSpace(const XString& str, size_t idx)
{
    static const char SPACE = 0x20;
    static const char HORIZONTAL_TAB = 0x09;
    static const char NEW_LINE = 0x0A;
    static const char CARRIAGE_RETURN = 0x0D;
    for ( size_t i = idx; i < str.size(); i++ )
        switch ( str.c_str()[i] )
        {
        case SPACE:
        case HORIZONTAL_TAB:
        case NEW_LINE:
        case CARRIAGE_RETURN:
            continue;
        default:
            return i;
        }
    return std::string::npos;
}

size_t XJSONItem::ParseString(const XString& str, size_t start, XString& res)
{
    size_t index = str.find(ESCAPED_QUOTE,start+1);
    while( index != std::string::npos )
    {
        if ( str.c_str()[index-1] != ESCAPED_SLASH )
        {
            res = str.substr(start+1,(index)-(start+1));
            return index+1;
        }
        else index = str.find(ESCAPED_QUOTE,index+1);
    }
    return std::string::npos;
}

void XJSONItem::CheckValidIndex(const size_t& index)
{
    if ( index == std::string::npos )
        X_STHROW(XJSONException, ("Index(%d) too big",index));
}

XIRef<XJSONItem> XJSONItem::ParseDocument(const XString& document)
{
    XIRef<XJSONItem> result;
    bool isObject = false;
    size_t next = GetNext(document,0,isObject);
    if ( next == std::string::npos || next+1 >= document.size() )
        X_STHROW(XJSONException, ("Document is not valid. next(%d), doc.sz(%d)",next,document.size()));
    result = isObject ? (XJSONItem*)new XJSONObject : (XJSONItem*)new XJSONArray;
    XString temp = document.substr(next+1);
    result->Parse(temp);
    return result;
}

XJSONValue::XJSONValue(): XJSONItem()
{}

XJSONValue::~XJSONValue(){}

XJSONType XJSONValue::GetType()
{
    return XJSONValueType;
}

XVariant& XJSONValue::GetValue()
{
    return _data;
}

bool XJSONValue::IsNull()
{
    return _data.IsEmpty();
}

size_t XJSONValue::Parse(XString& str)
{
    size_t index = std::string::npos;
    if ( str.StartsWith(ESCAPED_QUOTE) )
    {//Is String
        XString result = "";
        index = ParseString(str,0,result);
        _data = result;
    }
    else if ( str.StartsWith(FALSE_START) || str.StartsWith(NULL_START) || str.StartsWith(TRUE_START) )
    {//Is Literal
        switch( str.c_str()[0] )
        {
        case FALSE_START:
            index+=FALSE_LENGTH;
            _data = false;
            break;
        case NULL_START:
            index+=NULL_LENGTH;
            break;
        case TRUE_START:
            _data = true;
            index+=TRUE_LENGTH;
            break;
        default:
            X_STHROW(XJSONException, ("Found(%c) in(%s) should have been(t,f, or n)",str.c_str()[0],str.c_str()));
        }
    }
    else
    {//Is Number
        size_t comma=str.find(COMMA), closeBrace=str.find(BRACE_CLOSE), closeBracket=str.find(BRACKET_CLOSE);
        if ( comma < closeBrace && comma < closeBracket )
            index = comma;
        else if ( closeBrace < closeBracket )
            index = closeBrace;
        else index = closeBracket;
        _data = str.substr(0,index).StripInPlace().ToDouble();
    }
    return index;
}


XJSONObject::XJSONObject(): XJSONItem()
{
}
XJSONObject::~XJSONObject()
{
}

XJSONType XJSONObject::GetType()
{
    return XJSONObjectType;
}

XHash< XIRef<XJSONItem> >& XJSONObject::GetObjectContents()
{
    return _objectDefinition;
}

XIRef<XJSONItem> XJSONObject::GetObjectMember(const XSDK::XString& name)
{
    if ( _objectDefinition.Find(name) != NULL )
        return *(_objectDefinition.Find(name));

    return XIRef<XJSONItem>();
}

bool XJSONObject::HasIndex(const XSDK::XString& index)
{
    if ( _objectDefinition.Find(index) != NULL )
        return true;
    return false;
}

XIRef<XJSONItem> XJSONObject::Index(const XSDK::XString& index)
{
   if ( _objectDefinition.Find(index) == NULL )
      X_STHROW(XJSONException,("Index(%s) is not a valid key to the Object Definition",index.c_str()));
   return *(_objectDefinition.Find(index));
}


size_t XJSONObject::Parse(XSDK::XString& str)
{
    size_t index = 0;
    if ( str.length() == 1 || str.c_str()[0] == BRACE_CLOSE )
        return index;
    while ( index != std::string::npos )
    {
        XString key = "";
        index = str.find(ESCAPED_QUOTE,index);
        CheckValidIndex( index );
        index = ParseString(str,index,key);
        CheckValidIndex( index );
        index = str.find(COLON,index);
        CheckValidIndex( ++index );
        index = SearchForNonWhiteSpace(str,index);
        CheckValidIndex( index );
        char character = str.c_str()[index];
        XIRef<XJSONItem> item;
        XString temp = "";
        switch ( character )
        {
        case BRACE_CLOSE:
            return index;
        case  BRACE:
            {
              item = new XJSONObject;
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->Parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        case BRACKET:
            {
              item = new XJSONArray;
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->Parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        default:
            {
                item = new XJSONValue;
                temp = str.substr(index);
                index += item->Parse(temp);
                break;
            }
        }
        if( key.empty() )
            X_LOG_DEBUG("Parsed an empty JSON key, skipping...");
        else
            _objectDefinition.Add(key,item);

        index = SearchForNonWhiteSpace(str,index);
        character = str.c_str()[index];
        if ( character == BRACKET_CLOSE )
        {
            index = SearchForNonWhiteSpace(str,index+1);
            CheckValidIndex( index );
            character = str.c_str()[index];
        }
        switch ( character )
        {
        case BRACE_CLOSE:
            return index+2;
        case COMMA:
            ++index;
            break;
        default:
            X_STHROW(XJSONException, ("Token should be (\',\' or \'}\') but is(%c) in(%s)",character,str.c_str()));
            break;
        }
    }
    return 0;
}


XJSONArray::XJSONArray(): XJSONItem()
{
}
XJSONArray::~XJSONArray()
{
}

bool XJSONArray::HasIndex(const size_t& index)
{
    if( index < _arrayItems.size() )
        return true;
    return false;
}

XIRef<XJSONItem> XJSONArray::Index(const size_t& index)
{
   if ( index >= _arrayItems.size() )
      X_STHROW(XJSONException,("Index(%d) out of range(%d)",index,_arrayItems.size()));
   return _arrayItems[index];
}

size_t XJSONArray::Count()
{
   return _arrayItems.size();
}



XJSONType XJSONArray::GetType()
{
    return XJSONArrayType;
}

std::vector< XIRef<XJSONItem> > XJSONArray::GetArray()
{
    return _arrayItems;
}

XIRef<XJSONItem> XJSONArray::AccessElement(const size_t& index)
{
    if ( index > _arrayItems.size() )
        return XIRef<XJSONItem>();
    return _arrayItems[index];
}



size_t XJSONArray::Parse(XSDK::XString& str)
{
    size_t index = 0;
    while ( index != std::string::npos )
    {
        index = SearchForNonWhiteSpace(str,index);
        CheckValidIndex( index );
        char character = str.c_str()[index];
        XIRef<XJSONItem> item;
        XString temp = "";
        switch ( character )
        {
        case BRACKET_CLOSE:
            return index;
        case BRACE:
            {
              item = new XJSONObject;
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->Parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        case BRACKET:
            {
              item = new XJSONArray;
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->Parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        default:
            {
                item = new XJSONValue;
                temp = str.substr(index);
                index += item->Parse(temp);
                break;
            }
        }
        _arrayItems.push_back(item);
        index = SearchForNonWhiteSpace(str,index);
        character = str.c_str()[index];
        switch ( character )
        {
        case BRACKET_CLOSE:
            return index+2;
        case COMMA:
            ++index;
            break;
        default:
            X_STHROW(XJSONException, ("Token should be (\',\' or \'}\') but is(%c) in(%s)",character,str.c_str()));
        }
    }
    return 0;

}


XJSONException::XJSONException()
    : XException()
{}

XJSONException::~XJSONException() throw()
{}

XJSONException::XJSONException(const char* msg, ...)
    : XException()
{
    va_list args;
    va_start(args, msg);
    SetMsg(XString::Format(msg, args));
    va_end(args);
}

XJSONException::XJSONException(const XString& msg)
    : XException(msg)
{}







/*

    "He's making his first mistake.", Tom Cronin.

    "It's not a mistake. They don't make mistakes. They don't do random.
    There's always an objective. Always a target", Nicky

    "The objectives and targets always came from us. Who's giving them to him now?", Pamela Landy

    "Scary version? He is.", Nicky

    --The Bourne Supremacy
*/
