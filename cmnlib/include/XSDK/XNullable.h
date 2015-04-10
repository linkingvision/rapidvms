
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef _XSDK_XNullable_h_
#define _XSDK_XNullable_h_

#include "XSDK/XException.h"

namespace XSDK
{

/// Allows for a nullable value on the stack.
template<typename T>
class XNullable
{
public:

    X_API XNullable() : _value(), _isNull(true) {}

    X_API XNullable(T value) : _value(value), _isNull(false) {}

    X_API T Value() const { X_ASSERT(!_isNull); return _value; }

    X_API void SetValue(T value) { _value = value; _isNull = false; }

    X_API bool IsNull() const { return _isNull; }

    X_API void Clear() { _value = T(); _isNull = true; }

    X_API XNullable& operator=(const XNullable& rhs) { this->_value = rhs._value; this->_isNull = rhs._isNull; return *this; }
    X_API XNullable& operator=(const T& rhs) { this->_value = rhs; this->_isNull = false; return *this; }

    X_API friend bool operator==(const XNullable& lhs, const XNullable& rhs)
    { return (lhs._isNull && rhs._isNull) || (lhs._value == rhs._value); }

    X_API friend bool operator==(const XNullable& lhs, const T& rhs) { return !lhs._isNull && lhs._value == rhs; }
    X_API friend bool operator==(const T& lhs, const XNullable& rhs) { return rhs == lhs; }

    X_API friend bool operator!=(const XNullable& lhs, const XNullable& rhs) { return !(lhs == rhs); }
    X_API friend bool operator!=(const XNullable& lhs, const T& rhs) { return !(lhs == rhs); }
    X_API friend bool operator!=(const T& lhs, const XNullable& rhs) { return !(lhs == rhs); }

private:

    T _value;
    bool _isNull;
};

template<typename T>
bool operator==(const T& lhs, const XNullable<T>& rhs) { return rhs == lhs; }

template<typename T>
bool operator!=(const T& lhs, const XNullable<T>& rhs) { return rhs != lhs; }

}

#endif
