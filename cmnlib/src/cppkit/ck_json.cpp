
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#include "cppkit/ck_json.h"
#include <algorithm>

using namespace std;
using namespace cppkit;

static const char BRACE = '{';
static const char BRACKET = '[';
static const char ESCAPED_SLASH = '\\';
static const ck_string ESCAPED_QUOTE = "\"";
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

ck_json_item::ck_json_item() :
    _data(),
    _objectDefinition(),
    _arrayItems()
{
}

shared_ptr<ck_json_item> ck_json_item::index( const ck_string& index )
{
   CK_STHROW(ck_json_exception,("Not Implemented"));
}

shared_ptr<ck_json_item> ck_json_item::index( const size_t& index )
{
   CK_STHROW(ck_json_exception,("Not Implemented"));
}

size_t ck_json_item::size()
{
   CK_STHROW(ck_json_exception,("Not Implemented"));
}

shared_ptr<ck_json_item> ck_json_item::find( shared_ptr<ck_json_item> json, ck_string slashDelimitedPath )
{
    const static char delimiter = '/';
    shared_ptr<ck_json_item> result;
    if ( json.get() == NULL )
    {
        CK_LOG_INFO("Arg is empty, so returning NULL");
        return result;
    }
    std::vector<ck_string> parts = slashDelimitedPath.split(delimiter);
    shared_ptr<ck_json_item> current = json;
    for( ck_string part : parts )
    {
       // current is re-assigned below.  It is possible that current could be set to NULL if the previous
       // iteration did not find the previous item in the slashDelimitedPath
        if( current.get() == NULL )
           return current;

       if ( part.is_integer() )
       {
           //Means we want to index an item in an array
           if ( current->get_type() != CK_JSON_Array_Type )
               CK_STHROW(ck_json_exception, ("The %s item in the path(%s) does not match JSON structure!",part.c_str(),slashDelimitedPath.c_str()));
           shared_ptr<ck_json_array> ary = static_pointer_cast<ck_json_array>(current);
           current = ary->access_element(part.to_int());
       }
       else
       {
           //Means we are looking by name, in a hash
           if ( current->get_type() != CK_JSON_Object_Type )
               CK_STHROW(ck_json_exception, ("The %s item in the path(%s) does not match JSON structure!",part.c_str(),slashDelimitedPath.c_str()));
           shared_ptr<ck_json_object> obj = static_pointer_cast<ck_json_object>(current);
           current = obj->get_object_member(part);

       }
    }
    return current;
}

size_t ck_json_item::get_next( const ck_string& str, size_t from, bool& isBrace )
{
    size_t brace = str.find_first_of(BRACE,from);
    size_t bracket = str.find_first_of(BRACKET,from);
    if ( brace == std::string::npos && bracket == std::string::npos )
        return std::string::npos;
    if ( (isBrace = (brace < bracket)) )
        return brace;
    else return bracket;
}

size_t ck_json_item::search_for_non_white_space( const ck_string& str, size_t idx )
{
    char whitey[] = { 0x20 /*space*/, 0x09 /*tab*/, 0x0A /*new line*/, 0x0D/*carriage return*/ };

    return str.find_first_not_of( whitey, idx, sizeof( whitey ) );
}

size_t ck_json_item::parse_string( const ck_string& str, size_t start, ck_string& res )
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

void ck_json_item::check_valid_index( const size_t& index )
{
    if ( index == std::string::npos )
        CK_STHROW(ck_json_exception, ("Index(%d) too big",index));
}

shared_ptr<ck_json_item> ck_json_item::parse_document( const ck_string& document )
{
    shared_ptr<ck_json_item> result;
    bool isObject = false;
    size_t next = get_next(document,0,isObject);
    if ( next == std::string::npos || next+1 >= document.size() )
        CK_STHROW(ck_json_exception, ("Document is not valid. next(%d), doc.sz(%d)",next,document.size()));

    if( isObject )
    {
        shared_ptr<ck_json_object> obj = make_shared<ck_json_object>();
        result = static_pointer_cast<ck_json_item>( obj );
    }
    else
    {
        shared_ptr<ck_json_array> obj = make_shared<ck_json_array>();
        result = static_pointer_cast<ck_json_item>(obj);
    }
    ck_string temp = document.substr(next+1);
    result->parse(temp);

    return result;
}

ck_json_value::ck_json_value() :
    ck_json_item()
{
}

ck_json_value::~ck_json_value() throw()
{
}

ck_json_type ck_json_value::get_type()
{
    return CK_JSON_Value_Type;
}

ck_variant& ck_json_value::get_value()
{
    return _data;
}

bool ck_json_value::is_null()
{
    return _data.is_empty();
}

size_t ck_json_value::parse( ck_string& str )
{
    size_t index = std::string::npos;
    if ( str.starts_with(ESCAPED_QUOTE) )
    {//Is String
        ck_string result = "";
        index = parse_string(str,0,result);
        _data = result;
    }
    else if ( str.starts_with(FALSE_START) || str.starts_with(NULL_START) || str.starts_with(TRUE_START) )
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
            CK_STHROW(ck_json_exception, ("Found(%c) in(%s) should have been(t,f, or n)",str.c_str()[0],str.c_str()));
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
        _data = str.substr(0,index).strip().to_double();
    }

    return index;
}

ck_json_object::ck_json_object() :
    ck_json_item()
{
}

ck_json_object::~ck_json_object() throw()
{
}

ck_json_type ck_json_object::get_type()
{
    return CK_JSON_Object_Type;
}

unordered_map<std::string, shared_ptr<ck_json_item> >& ck_json_object::GetObjectContents()
{
    return _objectDefinition;
}

shared_ptr<ck_json_item> ck_json_object::get_object_member( const ck_string& name )
{
    if ( _objectDefinition.find(name) != _objectDefinition.end() )
        return (*_objectDefinition.find(name)).second;

    return shared_ptr<ck_json_item>();
}

shared_ptr<ck_json_item> ck_json_object::index( const ck_string& index )
{
    if ( _objectDefinition.find(index) == _objectDefinition.end() )
        CK_STHROW(ck_json_exception,("Index(%s) is not a valid key to the Object Definition",index.c_str()));

    return (*_objectDefinition.find(index)).second;
}

size_t ck_json_object::parse( ck_string& str )
{
    size_t index = 0;

    if ( str.length() == 1 || str.c_str()[0] == BRACE_CLOSE )
        return index;

    while ( index != std::string::npos )
    {
        ck_string key = "";
        index = str.find(ESCAPED_QUOTE,index);
        check_valid_index( index );
        index = parse_string(str,index,key);
        check_valid_index( index );
        index = str.find(COLON,index);
        check_valid_index( ++index );
        index = search_for_non_white_space(str,index);
        check_valid_index( index );
        char character = str.c_str()[index];
        shared_ptr<ck_json_item> item;
        ck_string temp = "";
        switch ( character )
        {
        case BRACE_CLOSE:
            return index;
        case  BRACE:
            {
              item = make_shared<ck_json_object>();
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        case BRACKET:
            {
              item = make_shared<ck_json_array>();
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        default:
            {
                item = make_shared<ck_json_value>();
                temp = str.substr(index);
                index += item->parse(temp);
                break;
            }
        }
        _objectDefinition[key] = item;
        index = search_for_non_white_space(str,index);
        character = str.c_str()[index];
        if ( character == BRACKET_CLOSE )
        {
            index = search_for_non_white_space(str,index+1);
            check_valid_index( index );
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
            CK_STHROW(ck_json_exception, ("Token should be (\',\' or \'}\') but is(%c) in(%s)",character,str.c_str()));
            break;
        }
    }

    return 0;
}

ck_json_array::ck_json_array() :
    ck_json_item()
{
}

ck_json_array::~ck_json_array() throw()
{
}

shared_ptr<ck_json_item> ck_json_array::index( const size_t& index )
{
   if ( index >= _arrayItems.size() )
      CK_STHROW(ck_json_exception,("Index(%d) out of range(%d)",index,_arrayItems.size()));

   return _arrayItems[index];
}

size_t ck_json_array::size()
{
   return _arrayItems.size();
}

ck_json_type ck_json_array::get_type()
{
    return CK_JSON_Array_Type;
}

std::vector< shared_ptr<ck_json_item> > ck_json_array::get_array()
{
    return _arrayItems;
}

shared_ptr<ck_json_item> ck_json_array::access_element( const size_t& index )
{
    if ( index > _arrayItems.size() )
        return shared_ptr<ck_json_item>();

    return _arrayItems[index];
}

size_t ck_json_array::parse( ck_string& str )
{
    size_t index = 0;
    while ( index != std::string::npos )
    {
        index = search_for_non_white_space(str,index);
        check_valid_index( index );
        char character = str.c_str()[index];
        shared_ptr<ck_json_item> item;
        ck_string temp = "";
        switch ( character )
        {
        case BRACKET_CLOSE:
            return index;
        case BRACE:
            {
              item = make_shared<ck_json_object>();
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        case BRACKET:
            {
              item = make_shared<ck_json_array>();
              temp = str.substr(index+1);
              size_t beforeIndex = index;
              index += item->parse(temp);
              if ( beforeIndex == index )
                index+=2;
              break;
            }
        default:
            {
                item = make_shared<ck_json_value>();
                temp = str.substr(index);
                index += item->parse(temp);
                break;
            }
        }
        _arrayItems.push_back(item);
        index = search_for_non_white_space(str,index);
        character = str.c_str()[index];
        switch ( character )
        {
        case BRACKET_CLOSE:
            return index+2;
        case COMMA:
            ++index;
            break;
        default:
            CK_STHROW(ck_json_exception, ("Token should be (\',\' or \'}\') but is(%c) in(%s)",character,str.c_str()));
        }
    }

    return 0;
}

ck_json_exception::ck_json_exception() :
    ck_exception()
{
}

ck_json_exception::~ck_json_exception() throw()
{
}

ck_json_exception::ck_json_exception( const char* msg, ... ) :
    ck_exception()
{
    va_list args;
    va_start(args, msg);
    set_msg(ck_string::format(msg, args));
    va_end(args);
}

ck_json_exception::ck_json_exception( const ck_string& msg ) :
    ck_exception(msg)
{
}
