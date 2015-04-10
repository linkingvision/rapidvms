
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

#ifndef cppkit_json_h
#define cppkit_json_h

#include <list>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <memory>

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_exception.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_variant.h"

namespace cppkit
{

typedef enum _ck_json_type
{
    CK_JSON_Object_Type,
    CK_JSON_Array_Type,
    CK_JSON_Value_Type,
} ck_json_type;

class ck_json_exception : public ck_exception
{
public:
    CK_API ck_json_exception();
    CK_API virtual ~ck_json_exception() throw();

    CK_API ck_json_exception( const char* msg, ... );

    CK_API ck_json_exception( const ck_string& msg );
};

class ck_json_item
{
public:
    ck_json_item();
    virtual ~ck_json_item() throw() {}

    CK_API virtual ck_json_type get_type()=0;

    CK_API static std::shared_ptr<ck_json_item> parse_document( const ck_string& document );

    CK_API virtual size_t parse( ck_string& str )=0;

    CK_API virtual std::shared_ptr<ck_json_item> index( const ck_string& index );
    CK_API virtual std::shared_ptr<ck_json_item> index( const size_t& index );
    CK_API virtual size_t size();

    template<class T>
    CK_API T get()
    {
        if ( get_type() != CK_JSON_Value_Type )
            CK_STHROW(ck_json_exception,("Cannot call get on non XJSONValue type json element"));
        return _data.get<T>();
    }

    CK_API static std::shared_ptr<ck_json_item> find( std::shared_ptr<ck_json_item> json,
                                                      ck_string slashDelimitedPath );

protected:
    static size_t get_next( const ck_string& str, size_t from, bool& isBrace );
    static size_t search_for_non_white_space( const ck_string& str, size_t idx );
    static void check_valid_index( const size_t& index );
    static size_t parse_string( const ck_string& str, size_t start, ck_string& res );

protected:
    ck_variant _data;
    std::unordered_map<std::string, std::shared_ptr<ck_json_item> > _objectDefinition;
    std::vector< std::shared_ptr<ck_json_item> > _arrayItems;
};

class ck_json_value : public ck_json_item
{
public:
    ck_json_value();
    virtual ~ck_json_value() throw();

    CK_API ck_json_type get_type();

    CK_API ck_variant& get_value();

    CK_API bool is_null();

    CK_API size_t parse( ck_string& str );
};

class ck_json_object : public ck_json_item
{
public:
    ck_json_object();
    virtual ~ck_json_object() throw();

    CK_API ck_json_type get_type();

    CK_API std::unordered_map<std::string, std::shared_ptr<ck_json_item> >& GetObjectContents();

    CK_API std::shared_ptr<ck_json_item> get_object_member( const ck_string& name );

    CK_API size_t parse( ck_string& str );

    CK_API std::shared_ptr<ck_json_item> index( const ck_string& index );
};

class ck_json_array : public ck_json_item
{
public:
    ck_json_array();
    virtual ~ck_json_array() throw();

    CK_API ck_json_type get_type();

    CK_API std::vector< std::shared_ptr<ck_json_item> > get_array();

    CK_API std::shared_ptr<ck_json_item> access_element( const size_t& index );

    CK_API size_t parse( ck_string& str );

    CK_API std::shared_ptr<ck_json_item> index( const size_t& index );

    CK_API size_t size();
};

}

#endif
