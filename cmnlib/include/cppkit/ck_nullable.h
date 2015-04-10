
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

#ifndef cppkit_nullable_h
#define cppkit_nullable_h

#include "cppkit/ck_exception.h"

namespace cppkit
{

/// Allows for a nullable value on the stack.
template<typename T>
class ck_nullable
{
public:
    CK_API ck_nullable() :
        _value(),
        _is_null( true )
    {
    }

    CK_API ck_nullable( T value ) :
        _value( value ),
        _is_null( false )
    {
    }

    CK_API T value() const
    {
        return _value;
    }

    CK_API void set_value( T value )
    {
        _value = value;
        _is_null = false;
    }

    CK_API bool is_null() const
    {
        return _is_null;
    }

    CK_API void clear()
    {
        _value = T();
        _is_null = true;
    }

    CK_API ck_nullable& operator = ( const ck_nullable& rhs )
    {
        this->_value = rhs._value;
        this->_is_null = rhs._is_null;
        return *this;
    }

    CK_API ck_nullable& operator = ( const T& rhs )
    {
        this->_value = rhs;
        this->_is_null = false;
        return *this;
    }

    CK_API friend bool operator == ( const ck_nullable& lhs, const ck_nullable& rhs )
    {
        return( lhs._is_null && rhs._is_null ) || (lhs._value == rhs._value);
    }

    CK_API friend bool operator == ( const ck_nullable& lhs, const T& rhs )
    {
        return !lhs._is_null && lhs._value == rhs;
    }

    CK_API friend bool operator == ( const T& lhs, const ck_nullable& rhs )
    {
        return rhs == lhs;
    }

    CK_API friend bool operator != ( const ck_nullable& lhs, const ck_nullable& rhs )
    {
        return !(lhs == rhs);
    }

    CK_API friend bool operator != ( const ck_nullable& lhs, const T& rhs )
    {
        return !(lhs == rhs);
    }

    CK_API friend bool operator != ( const T& lhs, const ck_nullable& rhs )
    {
        return !(lhs == rhs);
    }

private:
    T _value;
    bool _is_null;
};

template<typename T>
bool operator == ( const T& lhs, const ck_nullable<T>& rhs )
{
    return rhs == lhs;
}

template<typename T>
bool operator != ( const T& lhs, const ck_nullable<T>& rhs )
{
    return rhs != lhs;
}

}

#endif
