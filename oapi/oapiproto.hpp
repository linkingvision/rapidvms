#pragma once

// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct OapiID {
 unsigned int nId;

explicit OapiID():nId() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::OapiID > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< unsigned int > handler_0;bool has_nId;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "nId";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_nId = false;
    }

public:
    explicit SAXEventHandler( ::oapi::OapiID * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->nId)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x6e\x49\x64", 3))
                         { state=0; has_nId = true; }
            else {
                state = -1;
                the_error.reset(new error::UnknownFieldError(str, length)); return false;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_nId) set_missing_required("nId");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();

    }
};

template < class Writer6cf752b8863726693755b2f60ee4a73793e8f2d1148dfd21417b593560a90597 >
struct Serializer< Writer6cf752b8863726693755b2f60ee4a73793e8f2d1148dfd21417b593560a90597, ::oapi::OapiID > {

    void operator()( Writer6cf752b8863726693755b2f60ee4a73793e8f2d1148dfd21417b593560a90597& w, const ::oapi::OapiID& value) const
    {
        w.StartObject();

        w.Key("\x6e\x49\x64", 3, false); Serializer< Writer6cf752b8863726693755b2f60ee4a73793e8f2d1148dfd21417b593560a90597, unsigned int >()(w, value.nId);

        w.EndObject(1);
    }

};
}


// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct OapiStatus {
 unsigned int Status;

explicit OapiStatus():Status() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::OapiStatus > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< unsigned int > handler_0;bool has_Status;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "Status";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_Status = false;
    }

public:
    explicit SAXEventHandler( ::oapi::OapiStatus * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->Status)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x53\x74\x61\x74\x75\x73", 6))
                         { state=0; has_Status = true; }
            else {
                state = -1;
                the_error.reset(new error::UnknownFieldError(str, length)); return false;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_Status) set_missing_required("Status");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();

    }
};

template < class Writer78947f028eaa1365e2cdf921b5f81ffe5bc1e54438912f7b4f39106fc1d4fb95 >
struct Serializer< Writer78947f028eaa1365e2cdf921b5f81ffe5bc1e54438912f7b4f39106fc1d4fb95, ::oapi::OapiStatus > {

    void operator()( Writer78947f028eaa1365e2cdf921b5f81ffe5bc1e54438912f7b4f39106fc1d4fb95& w, const ::oapi::OapiStatus& value) const
    {
        w.StartObject();

        w.Key("\x53\x74\x61\x74\x75\x73", 6, false); Serializer< Writer78947f028eaa1365e2cdf921b5f81ffe5bc1e54438912f7b4f39106fc1d4fb95, unsigned int >()(w, value.Status);

        w.EndObject(1);
    }

};
}


// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct AuthNonce {
 std::string Nonce;

explicit AuthNonce():Nonce() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::AuthNonce > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::string > handler_0;bool has_Nonce;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "Nonce";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_Nonce = false;
    }

public:
    explicit SAXEventHandler( ::oapi::AuthNonce * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->Nonce)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x4e\x6f\x6e\x63\x65", 5))
                         { state=0; has_Nonce = true; }
            else {
                state = -1;
                the_error.reset(new error::UnknownFieldError(str, length)); return false;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_Nonce) set_missing_required("Nonce");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();

    }
};

template < class Writer05ddfc9785c77fe964012b9c852eef7913ed05c4ca1c1f834eca981de3d5a10c >
struct Serializer< Writer05ddfc9785c77fe964012b9c852eef7913ed05c4ca1c1f834eca981de3d5a10c, ::oapi::AuthNonce > {

    void operator()( Writer05ddfc9785c77fe964012b9c852eef7913ed05c4ca1c1f834eca981de3d5a10c& w, const ::oapi::AuthNonce& value) const
    {
        w.StartObject();

        w.Key("\x4e\x6f\x6e\x63\x65", 5, false); Serializer< Writer05ddfc9785c77fe964012b9c852eef7913ed05c4ca1c1f834eca981de3d5a10c, std::string >()(w, value.Nonce);

        w.EndObject(1);
    }

};
}


// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct Login {
 std::string User;
std::string Password;

explicit Login():User(), Password() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::Login > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::string > handler_0;
SAXEventHandler< std::string > handler_1;bool has_User;
bool has_Password;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "User";
case 1:
    return "Password";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_User = false;
has_Password = false;
    }

public:
    explicit SAXEventHandler( ::oapi::Login * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->User)
, handler_1(&obj->Password)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

case 1:
    return checked_event_forwarding(handler_1.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

case 1:
    return checked_event_forwarding(handler_1.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

case 1:
    return checked_event_forwarding(handler_1.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

case 1:
    return checked_event_forwarding(handler_1.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

case 1:
    return checked_event_forwarding(handler_1.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

case 1:
    return checked_event_forwarding(handler_1.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

case 1:
    return checked_event_forwarding(handler_1.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x55\x73\x65\x72", 4))
                         { state=0; has_User = true; }
else if (utility::string_equal(str, length, "\x50\x61\x73\x73\x77\x6f\x72\x64", 8))
                         { state=1; has_Password = true; }
            else {
                state = -1;
                the_error.reset(new error::UnknownFieldError(str, length)); return false;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

case 1:
    return checked_event_forwarding(handler_1.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

case 1:
    return checked_event_forwarding(handler_1.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

case 1:
    return checked_event_forwarding(handler_1.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

case 1:
    return checked_event_forwarding(handler_1.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_User) set_missing_required("User");
if (!has_Password) set_missing_required("Password");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;
case 1:
     handler_1.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();

    }
};

template < class Writer861c4992def473bcd449640e44383a1f0dc0f0773d6fc9bdd4859a5258d69e9b >
struct Serializer< Writer861c4992def473bcd449640e44383a1f0dc0f0773d6fc9bdd4859a5258d69e9b, ::oapi::Login > {

    void operator()( Writer861c4992def473bcd449640e44383a1f0dc0f0773d6fc9bdd4859a5258d69e9b& w, const ::oapi::Login& value) const
    {
        w.StartObject();

        w.Key("\x55\x73\x65\x72", 4, false); Serializer< Writer861c4992def473bcd449640e44383a1f0dc0f0773d6fc9bdd4859a5258d69e9b, std::string >()(w, value.User);
w.Key("\x50\x61\x73\x73\x77\x6f\x72\x64", 8, false); Serializer< Writer861c4992def473bcd449640e44383a1f0dc0f0773d6fc9bdd4859a5258d69e9b, std::string >()(w, value.Password);

        w.EndObject(2);
    }

};
}


// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct Device {
 unsigned int nId;
unsigned int nType;
unsigned int nSubType;
std::string Name;
std::string Param;
std::string IP;
std::string Port;
std::string User;
std::string Password;
std::string RtspLocation;
std::string FileLocation;
std::string OnvifAddress;
std::string CameraIndex;
unsigned int UseProfileToken;
std::string OnvifProfileToken;
unsigned int Recording;
unsigned int GroupId;
unsigned int HdfsRecording;
std::string OnvifProfileToken2;
unsigned int ConnectType;
unsigned int Mining;
unsigned int HWAccel;
unsigned int IPV6;

explicit Device():nId(), nType(), nSubType(), Name(), Param(), IP(), Port(), User(), Password(), RtspLocation(), FileLocation(), OnvifAddress(), CameraIndex(), UseProfileToken(), OnvifProfileToken(), Recording(), GroupId(), HdfsRecording(), OnvifProfileToken2(), ConnectType(), Mining(), HWAccel(), IPV6() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::Device > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< unsigned int > handler_0;
SAXEventHandler< unsigned int > handler_1;
SAXEventHandler< unsigned int > handler_2;
SAXEventHandler< std::string > handler_3;
SAXEventHandler< std::string > handler_4;
SAXEventHandler< std::string > handler_5;
SAXEventHandler< std::string > handler_6;
SAXEventHandler< std::string > handler_7;
SAXEventHandler< std::string > handler_8;
SAXEventHandler< std::string > handler_9;
SAXEventHandler< std::string > handler_10;
SAXEventHandler< std::string > handler_11;
SAXEventHandler< std::string > handler_12;
SAXEventHandler< unsigned int > handler_13;
SAXEventHandler< std::string > handler_14;
SAXEventHandler< unsigned int > handler_15;
SAXEventHandler< unsigned int > handler_16;
SAXEventHandler< unsigned int > handler_17;
SAXEventHandler< std::string > handler_18;
SAXEventHandler< unsigned int > handler_19;
SAXEventHandler< unsigned int > handler_20;
SAXEventHandler< unsigned int > handler_21;
SAXEventHandler< unsigned int > handler_22;bool has_nId;
bool has_nType;
bool has_nSubType;
bool has_Name;
bool has_Param;
bool has_IP;
bool has_Port;
bool has_User;
bool has_Password;
bool has_RtspLocation;
bool has_FileLocation;
bool has_OnvifAddress;
bool has_CameraIndex;
bool has_UseProfileToken;
bool has_OnvifProfileToken;
bool has_Recording;
bool has_GroupId;
bool has_HdfsRecording;
bool has_OnvifProfileToken2;
bool has_ConnectType;
bool has_Mining;
bool has_HWAccel;
bool has_IPV6;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "nId";
case 1:
    return "nType";
case 2:
    return "nSubType";
case 3:
    return "Name";
case 4:
    return "Param";
case 5:
    return "IP";
case 6:
    return "Port";
case 7:
    return "User";
case 8:
    return "Password";
case 9:
    return "RtspLocation";
case 10:
    return "FileLocation";
case 11:
    return "OnvifAddress";
case 12:
    return "CameraIndex";
case 13:
    return "UseProfileToken";
case 14:
    return "OnvifProfileToken";
case 15:
    return "Recording";
case 16:
    return "GroupId";
case 17:
    return "HdfsRecording";
case 18:
    return "OnvifProfileToken2";
case 19:
    return "ConnectType";
case 20:
    return "Mining";
case 21:
    return "HWAccel";
case 22:
    return "IPV6";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_nId = false;
has_nType = false;
has_nSubType = false;
has_Name = false;
has_Param = false;
has_IP = false;
has_Port = false;
has_User = false;
has_Password = false;
has_RtspLocation = false;
has_FileLocation = false;
has_OnvifAddress = false;
has_CameraIndex = false;
has_UseProfileToken = false;
has_OnvifProfileToken = false;
has_Recording = false;
has_GroupId = false;
has_HdfsRecording = false;
has_OnvifProfileToken2 = false;
has_ConnectType = false;
has_Mining = false;
has_HWAccel = false;
has_IPV6 = false;
    }

public:
    explicit SAXEventHandler( ::oapi::Device * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->nId)
, handler_1(&obj->nType)
, handler_2(&obj->nSubType)
, handler_3(&obj->Name)
, handler_4(&obj->Param)
, handler_5(&obj->IP)
, handler_6(&obj->Port)
, handler_7(&obj->User)
, handler_8(&obj->Password)
, handler_9(&obj->RtspLocation)
, handler_10(&obj->FileLocation)
, handler_11(&obj->OnvifAddress)
, handler_12(&obj->CameraIndex)
, handler_13(&obj->UseProfileToken)
, handler_14(&obj->OnvifProfileToken)
, handler_15(&obj->Recording)
, handler_16(&obj->GroupId)
, handler_17(&obj->HdfsRecording)
, handler_18(&obj->OnvifProfileToken2)
, handler_19(&obj->ConnectType)
, handler_20(&obj->Mining)
, handler_21(&obj->HWAccel)
, handler_22(&obj->IPV6)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

case 1:
    return checked_event_forwarding(handler_1.Null());

case 2:
    return checked_event_forwarding(handler_2.Null());

case 3:
    return checked_event_forwarding(handler_3.Null());

case 4:
    return checked_event_forwarding(handler_4.Null());

case 5:
    return checked_event_forwarding(handler_5.Null());

case 6:
    return checked_event_forwarding(handler_6.Null());

case 7:
    return checked_event_forwarding(handler_7.Null());

case 8:
    return checked_event_forwarding(handler_8.Null());

case 9:
    return checked_event_forwarding(handler_9.Null());

case 10:
    return checked_event_forwarding(handler_10.Null());

case 11:
    return checked_event_forwarding(handler_11.Null());

case 12:
    return checked_event_forwarding(handler_12.Null());

case 13:
    return checked_event_forwarding(handler_13.Null());

case 14:
    return checked_event_forwarding(handler_14.Null());

case 15:
    return checked_event_forwarding(handler_15.Null());

case 16:
    return checked_event_forwarding(handler_16.Null());

case 17:
    return checked_event_forwarding(handler_17.Null());

case 18:
    return checked_event_forwarding(handler_18.Null());

case 19:
    return checked_event_forwarding(handler_19.Null());

case 20:
    return checked_event_forwarding(handler_20.Null());

case 21:
    return checked_event_forwarding(handler_21.Null());

case 22:
    return checked_event_forwarding(handler_22.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

case 1:
    return checked_event_forwarding(handler_1.Bool(b));

case 2:
    return checked_event_forwarding(handler_2.Bool(b));

case 3:
    return checked_event_forwarding(handler_3.Bool(b));

case 4:
    return checked_event_forwarding(handler_4.Bool(b));

case 5:
    return checked_event_forwarding(handler_5.Bool(b));

case 6:
    return checked_event_forwarding(handler_6.Bool(b));

case 7:
    return checked_event_forwarding(handler_7.Bool(b));

case 8:
    return checked_event_forwarding(handler_8.Bool(b));

case 9:
    return checked_event_forwarding(handler_9.Bool(b));

case 10:
    return checked_event_forwarding(handler_10.Bool(b));

case 11:
    return checked_event_forwarding(handler_11.Bool(b));

case 12:
    return checked_event_forwarding(handler_12.Bool(b));

case 13:
    return checked_event_forwarding(handler_13.Bool(b));

case 14:
    return checked_event_forwarding(handler_14.Bool(b));

case 15:
    return checked_event_forwarding(handler_15.Bool(b));

case 16:
    return checked_event_forwarding(handler_16.Bool(b));

case 17:
    return checked_event_forwarding(handler_17.Bool(b));

case 18:
    return checked_event_forwarding(handler_18.Bool(b));

case 19:
    return checked_event_forwarding(handler_19.Bool(b));

case 20:
    return checked_event_forwarding(handler_20.Bool(b));

case 21:
    return checked_event_forwarding(handler_21.Bool(b));

case 22:
    return checked_event_forwarding(handler_22.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

case 1:
    return checked_event_forwarding(handler_1.Int(i));

case 2:
    return checked_event_forwarding(handler_2.Int(i));

case 3:
    return checked_event_forwarding(handler_3.Int(i));

case 4:
    return checked_event_forwarding(handler_4.Int(i));

case 5:
    return checked_event_forwarding(handler_5.Int(i));

case 6:
    return checked_event_forwarding(handler_6.Int(i));

case 7:
    return checked_event_forwarding(handler_7.Int(i));

case 8:
    return checked_event_forwarding(handler_8.Int(i));

case 9:
    return checked_event_forwarding(handler_9.Int(i));

case 10:
    return checked_event_forwarding(handler_10.Int(i));

case 11:
    return checked_event_forwarding(handler_11.Int(i));

case 12:
    return checked_event_forwarding(handler_12.Int(i));

case 13:
    return checked_event_forwarding(handler_13.Int(i));

case 14:
    return checked_event_forwarding(handler_14.Int(i));

case 15:
    return checked_event_forwarding(handler_15.Int(i));

case 16:
    return checked_event_forwarding(handler_16.Int(i));

case 17:
    return checked_event_forwarding(handler_17.Int(i));

case 18:
    return checked_event_forwarding(handler_18.Int(i));

case 19:
    return checked_event_forwarding(handler_19.Int(i));

case 20:
    return checked_event_forwarding(handler_20.Int(i));

case 21:
    return checked_event_forwarding(handler_21.Int(i));

case 22:
    return checked_event_forwarding(handler_22.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

case 1:
    return checked_event_forwarding(handler_1.Uint(i));

case 2:
    return checked_event_forwarding(handler_2.Uint(i));

case 3:
    return checked_event_forwarding(handler_3.Uint(i));

case 4:
    return checked_event_forwarding(handler_4.Uint(i));

case 5:
    return checked_event_forwarding(handler_5.Uint(i));

case 6:
    return checked_event_forwarding(handler_6.Uint(i));

case 7:
    return checked_event_forwarding(handler_7.Uint(i));

case 8:
    return checked_event_forwarding(handler_8.Uint(i));

case 9:
    return checked_event_forwarding(handler_9.Uint(i));

case 10:
    return checked_event_forwarding(handler_10.Uint(i));

case 11:
    return checked_event_forwarding(handler_11.Uint(i));

case 12:
    return checked_event_forwarding(handler_12.Uint(i));

case 13:
    return checked_event_forwarding(handler_13.Uint(i));

case 14:
    return checked_event_forwarding(handler_14.Uint(i));

case 15:
    return checked_event_forwarding(handler_15.Uint(i));

case 16:
    return checked_event_forwarding(handler_16.Uint(i));

case 17:
    return checked_event_forwarding(handler_17.Uint(i));

case 18:
    return checked_event_forwarding(handler_18.Uint(i));

case 19:
    return checked_event_forwarding(handler_19.Uint(i));

case 20:
    return checked_event_forwarding(handler_20.Uint(i));

case 21:
    return checked_event_forwarding(handler_21.Uint(i));

case 22:
    return checked_event_forwarding(handler_22.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

case 1:
    return checked_event_forwarding(handler_1.Int64(i));

case 2:
    return checked_event_forwarding(handler_2.Int64(i));

case 3:
    return checked_event_forwarding(handler_3.Int64(i));

case 4:
    return checked_event_forwarding(handler_4.Int64(i));

case 5:
    return checked_event_forwarding(handler_5.Int64(i));

case 6:
    return checked_event_forwarding(handler_6.Int64(i));

case 7:
    return checked_event_forwarding(handler_7.Int64(i));

case 8:
    return checked_event_forwarding(handler_8.Int64(i));

case 9:
    return checked_event_forwarding(handler_9.Int64(i));

case 10:
    return checked_event_forwarding(handler_10.Int64(i));

case 11:
    return checked_event_forwarding(handler_11.Int64(i));

case 12:
    return checked_event_forwarding(handler_12.Int64(i));

case 13:
    return checked_event_forwarding(handler_13.Int64(i));

case 14:
    return checked_event_forwarding(handler_14.Int64(i));

case 15:
    return checked_event_forwarding(handler_15.Int64(i));

case 16:
    return checked_event_forwarding(handler_16.Int64(i));

case 17:
    return checked_event_forwarding(handler_17.Int64(i));

case 18:
    return checked_event_forwarding(handler_18.Int64(i));

case 19:
    return checked_event_forwarding(handler_19.Int64(i));

case 20:
    return checked_event_forwarding(handler_20.Int64(i));

case 21:
    return checked_event_forwarding(handler_21.Int64(i));

case 22:
    return checked_event_forwarding(handler_22.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

case 1:
    return checked_event_forwarding(handler_1.Uint64(i));

case 2:
    return checked_event_forwarding(handler_2.Uint64(i));

case 3:
    return checked_event_forwarding(handler_3.Uint64(i));

case 4:
    return checked_event_forwarding(handler_4.Uint64(i));

case 5:
    return checked_event_forwarding(handler_5.Uint64(i));

case 6:
    return checked_event_forwarding(handler_6.Uint64(i));

case 7:
    return checked_event_forwarding(handler_7.Uint64(i));

case 8:
    return checked_event_forwarding(handler_8.Uint64(i));

case 9:
    return checked_event_forwarding(handler_9.Uint64(i));

case 10:
    return checked_event_forwarding(handler_10.Uint64(i));

case 11:
    return checked_event_forwarding(handler_11.Uint64(i));

case 12:
    return checked_event_forwarding(handler_12.Uint64(i));

case 13:
    return checked_event_forwarding(handler_13.Uint64(i));

case 14:
    return checked_event_forwarding(handler_14.Uint64(i));

case 15:
    return checked_event_forwarding(handler_15.Uint64(i));

case 16:
    return checked_event_forwarding(handler_16.Uint64(i));

case 17:
    return checked_event_forwarding(handler_17.Uint64(i));

case 18:
    return checked_event_forwarding(handler_18.Uint64(i));

case 19:
    return checked_event_forwarding(handler_19.Uint64(i));

case 20:
    return checked_event_forwarding(handler_20.Uint64(i));

case 21:
    return checked_event_forwarding(handler_21.Uint64(i));

case 22:
    return checked_event_forwarding(handler_22.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

case 1:
    return checked_event_forwarding(handler_1.Double(d));

case 2:
    return checked_event_forwarding(handler_2.Double(d));

case 3:
    return checked_event_forwarding(handler_3.Double(d));

case 4:
    return checked_event_forwarding(handler_4.Double(d));

case 5:
    return checked_event_forwarding(handler_5.Double(d));

case 6:
    return checked_event_forwarding(handler_6.Double(d));

case 7:
    return checked_event_forwarding(handler_7.Double(d));

case 8:
    return checked_event_forwarding(handler_8.Double(d));

case 9:
    return checked_event_forwarding(handler_9.Double(d));

case 10:
    return checked_event_forwarding(handler_10.Double(d));

case 11:
    return checked_event_forwarding(handler_11.Double(d));

case 12:
    return checked_event_forwarding(handler_12.Double(d));

case 13:
    return checked_event_forwarding(handler_13.Double(d));

case 14:
    return checked_event_forwarding(handler_14.Double(d));

case 15:
    return checked_event_forwarding(handler_15.Double(d));

case 16:
    return checked_event_forwarding(handler_16.Double(d));

case 17:
    return checked_event_forwarding(handler_17.Double(d));

case 18:
    return checked_event_forwarding(handler_18.Double(d));

case 19:
    return checked_event_forwarding(handler_19.Double(d));

case 20:
    return checked_event_forwarding(handler_20.Double(d));

case 21:
    return checked_event_forwarding(handler_21.Double(d));

case 22:
    return checked_event_forwarding(handler_22.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.String(str, length, copy));

case 2:
    return checked_event_forwarding(handler_2.String(str, length, copy));

case 3:
    return checked_event_forwarding(handler_3.String(str, length, copy));

case 4:
    return checked_event_forwarding(handler_4.String(str, length, copy));

case 5:
    return checked_event_forwarding(handler_5.String(str, length, copy));

case 6:
    return checked_event_forwarding(handler_6.String(str, length, copy));

case 7:
    return checked_event_forwarding(handler_7.String(str, length, copy));

case 8:
    return checked_event_forwarding(handler_8.String(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.String(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.String(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.String(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.String(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.String(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.String(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.String(str, length, copy));

case 16:
    return checked_event_forwarding(handler_16.String(str, length, copy));

case 17:
    return checked_event_forwarding(handler_17.String(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.String(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.String(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.String(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.String(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x6e\x49\x64", 3))
                         { state=0; has_nId = true; }
else if (utility::string_equal(str, length, "\x6e\x54\x79\x70\x65", 5))
                         { state=1; has_nType = true; }
else if (utility::string_equal(str, length, "\x6e\x53\x75\x62\x54\x79\x70\x65", 8))
                         { state=2; has_nSubType = true; }
else if (utility::string_equal(str, length, "\x4e\x61\x6d\x65", 4))
                         { state=3; has_Name = true; }
else if (utility::string_equal(str, length, "\x50\x61\x72\x61\x6d", 5))
                         { state=4; has_Param = true; }
else if (utility::string_equal(str, length, "\x49\x50", 2))
                         { state=5; has_IP = true; }
else if (utility::string_equal(str, length, "\x50\x6f\x72\x74", 4))
                         { state=6; has_Port = true; }
else if (utility::string_equal(str, length, "\x55\x73\x65\x72", 4))
                         { state=7; has_User = true; }
else if (utility::string_equal(str, length, "\x50\x61\x73\x73\x77\x6f\x72\x64", 8))
                         { state=8; has_Password = true; }
else if (utility::string_equal(str, length, "\x52\x74\x73\x70\x4c\x6f\x63\x61\x74\x69\x6f\x6e", 12))
                         { state=9; has_RtspLocation = true; }
else if (utility::string_equal(str, length, "\x46\x69\x6c\x65\x4c\x6f\x63\x61\x74\x69\x6f\x6e", 12))
                         { state=10; has_FileLocation = true; }
else if (utility::string_equal(str, length, "\x4f\x6e\x76\x69\x66\x41\x64\x64\x72\x65\x73\x73", 12))
                         { state=11; has_OnvifAddress = true; }
else if (utility::string_equal(str, length, "\x43\x61\x6d\x65\x72\x61\x49\x6e\x64\x65\x78", 11))
                         { state=12; has_CameraIndex = true; }
else if (utility::string_equal(str, length, "\x55\x73\x65\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e", 15))
                         { state=13; has_UseProfileToken = true; }
else if (utility::string_equal(str, length, "\x4f\x6e\x76\x69\x66\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e", 17))
                         { state=14; has_OnvifProfileToken = true; }
else if (utility::string_equal(str, length, "\x52\x65\x63\x6f\x72\x64\x69\x6e\x67", 9))
                         { state=15; has_Recording = true; }
else if (utility::string_equal(str, length, "\x47\x72\x6f\x75\x70\x49\x64", 7))
                         { state=16; has_GroupId = true; }
else if (utility::string_equal(str, length, "\x48\x64\x66\x73\x52\x65\x63\x6f\x72\x64\x69\x6e\x67", 13))
                         { state=17; has_HdfsRecording = true; }
else if (utility::string_equal(str, length, "\x4f\x6e\x76\x69\x66\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e\x32", 18))
                         { state=18; has_OnvifProfileToken2 = true; }
else if (utility::string_equal(str, length, "\x43\x6f\x6e\x6e\x65\x63\x74\x54\x79\x70\x65", 11))
                         { state=19; has_ConnectType = true; }
else if (utility::string_equal(str, length, "\x4d\x69\x6e\x69\x6e\x67", 6))
                         { state=20; has_Mining = true; }
else if (utility::string_equal(str, length, "\x48\x57\x41\x63\x63\x65\x6c", 7))
                         { state=21; has_HWAccel = true; }
else if (utility::string_equal(str, length, "\x49\x50\x56\x36", 4))
                         { state=22; has_IPV6 = true; }
            else {
                state = -1;
                the_error.reset(new error::UnknownFieldError(str, length)); return false;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.Key(str, length, copy));

case 2:
    return checked_event_forwarding(handler_2.Key(str, length, copy));

case 3:
    return checked_event_forwarding(handler_3.Key(str, length, copy));

case 4:
    return checked_event_forwarding(handler_4.Key(str, length, copy));

case 5:
    return checked_event_forwarding(handler_5.Key(str, length, copy));

case 6:
    return checked_event_forwarding(handler_6.Key(str, length, copy));

case 7:
    return checked_event_forwarding(handler_7.Key(str, length, copy));

case 8:
    return checked_event_forwarding(handler_8.Key(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.Key(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.Key(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.Key(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.Key(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.Key(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.Key(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.Key(str, length, copy));

case 16:
    return checked_event_forwarding(handler_16.Key(str, length, copy));

case 17:
    return checked_event_forwarding(handler_17.Key(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.Key(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.Key(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.Key(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.Key(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

case 1:
    return checked_event_forwarding(handler_1.StartArray());

case 2:
    return checked_event_forwarding(handler_2.StartArray());

case 3:
    return checked_event_forwarding(handler_3.StartArray());

case 4:
    return checked_event_forwarding(handler_4.StartArray());

case 5:
    return checked_event_forwarding(handler_5.StartArray());

case 6:
    return checked_event_forwarding(handler_6.StartArray());

case 7:
    return checked_event_forwarding(handler_7.StartArray());

case 8:
    return checked_event_forwarding(handler_8.StartArray());

case 9:
    return checked_event_forwarding(handler_9.StartArray());

case 10:
    return checked_event_forwarding(handler_10.StartArray());

case 11:
    return checked_event_forwarding(handler_11.StartArray());

case 12:
    return checked_event_forwarding(handler_12.StartArray());

case 13:
    return checked_event_forwarding(handler_13.StartArray());

case 14:
    return checked_event_forwarding(handler_14.StartArray());

case 15:
    return checked_event_forwarding(handler_15.StartArray());

case 16:
    return checked_event_forwarding(handler_16.StartArray());

case 17:
    return checked_event_forwarding(handler_17.StartArray());

case 18:
    return checked_event_forwarding(handler_18.StartArray());

case 19:
    return checked_event_forwarding(handler_19.StartArray());

case 20:
    return checked_event_forwarding(handler_20.StartArray());

case 21:
    return checked_event_forwarding(handler_21.StartArray());

case 22:
    return checked_event_forwarding(handler_22.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

case 1:
    return checked_event_forwarding(handler_1.EndArray(length));

case 2:
    return checked_event_forwarding(handler_2.EndArray(length));

case 3:
    return checked_event_forwarding(handler_3.EndArray(length));

case 4:
    return checked_event_forwarding(handler_4.EndArray(length));

case 5:
    return checked_event_forwarding(handler_5.EndArray(length));

case 6:
    return checked_event_forwarding(handler_6.EndArray(length));

case 7:
    return checked_event_forwarding(handler_7.EndArray(length));

case 8:
    return checked_event_forwarding(handler_8.EndArray(length));

case 9:
    return checked_event_forwarding(handler_9.EndArray(length));

case 10:
    return checked_event_forwarding(handler_10.EndArray(length));

case 11:
    return checked_event_forwarding(handler_11.EndArray(length));

case 12:
    return checked_event_forwarding(handler_12.EndArray(length));

case 13:
    return checked_event_forwarding(handler_13.EndArray(length));

case 14:
    return checked_event_forwarding(handler_14.EndArray(length));

case 15:
    return checked_event_forwarding(handler_15.EndArray(length));

case 16:
    return checked_event_forwarding(handler_16.EndArray(length));

case 17:
    return checked_event_forwarding(handler_17.EndArray(length));

case 18:
    return checked_event_forwarding(handler_18.EndArray(length));

case 19:
    return checked_event_forwarding(handler_19.EndArray(length));

case 20:
    return checked_event_forwarding(handler_20.EndArray(length));

case 21:
    return checked_event_forwarding(handler_21.EndArray(length));

case 22:
    return checked_event_forwarding(handler_22.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

case 1:
    return checked_event_forwarding(handler_1.StartObject());

case 2:
    return checked_event_forwarding(handler_2.StartObject());

case 3:
    return checked_event_forwarding(handler_3.StartObject());

case 4:
    return checked_event_forwarding(handler_4.StartObject());

case 5:
    return checked_event_forwarding(handler_5.StartObject());

case 6:
    return checked_event_forwarding(handler_6.StartObject());

case 7:
    return checked_event_forwarding(handler_7.StartObject());

case 8:
    return checked_event_forwarding(handler_8.StartObject());

case 9:
    return checked_event_forwarding(handler_9.StartObject());

case 10:
    return checked_event_forwarding(handler_10.StartObject());

case 11:
    return checked_event_forwarding(handler_11.StartObject());

case 12:
    return checked_event_forwarding(handler_12.StartObject());

case 13:
    return checked_event_forwarding(handler_13.StartObject());

case 14:
    return checked_event_forwarding(handler_14.StartObject());

case 15:
    return checked_event_forwarding(handler_15.StartObject());

case 16:
    return checked_event_forwarding(handler_16.StartObject());

case 17:
    return checked_event_forwarding(handler_17.StartObject());

case 18:
    return checked_event_forwarding(handler_18.StartObject());

case 19:
    return checked_event_forwarding(handler_19.StartObject());

case 20:
    return checked_event_forwarding(handler_20.StartObject());

case 21:
    return checked_event_forwarding(handler_21.StartObject());

case 22:
    return checked_event_forwarding(handler_22.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

case 1:
    return checked_event_forwarding(handler_1.EndObject(length));

case 2:
    return checked_event_forwarding(handler_2.EndObject(length));

case 3:
    return checked_event_forwarding(handler_3.EndObject(length));

case 4:
    return checked_event_forwarding(handler_4.EndObject(length));

case 5:
    return checked_event_forwarding(handler_5.EndObject(length));

case 6:
    return checked_event_forwarding(handler_6.EndObject(length));

case 7:
    return checked_event_forwarding(handler_7.EndObject(length));

case 8:
    return checked_event_forwarding(handler_8.EndObject(length));

case 9:
    return checked_event_forwarding(handler_9.EndObject(length));

case 10:
    return checked_event_forwarding(handler_10.EndObject(length));

case 11:
    return checked_event_forwarding(handler_11.EndObject(length));

case 12:
    return checked_event_forwarding(handler_12.EndObject(length));

case 13:
    return checked_event_forwarding(handler_13.EndObject(length));

case 14:
    return checked_event_forwarding(handler_14.EndObject(length));

case 15:
    return checked_event_forwarding(handler_15.EndObject(length));

case 16:
    return checked_event_forwarding(handler_16.EndObject(length));

case 17:
    return checked_event_forwarding(handler_17.EndObject(length));

case 18:
    return checked_event_forwarding(handler_18.EndObject(length));

case 19:
    return checked_event_forwarding(handler_19.EndObject(length));

case 20:
    return checked_event_forwarding(handler_20.EndObject(length));

case 21:
    return checked_event_forwarding(handler_21.EndObject(length));

case 22:
    return checked_event_forwarding(handler_22.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_nId) set_missing_required("nId");
if (!has_nType) set_missing_required("nType");
if (!has_nSubType) set_missing_required("nSubType");
if (!has_Name) set_missing_required("Name");
if (!has_Param) set_missing_required("Param");
if (!has_IP) set_missing_required("IP");
if (!has_Port) set_missing_required("Port");
if (!has_User) set_missing_required("User");
if (!has_Password) set_missing_required("Password");
if (!has_RtspLocation) set_missing_required("RtspLocation");
if (!has_FileLocation) set_missing_required("FileLocation");
if (!has_OnvifAddress) set_missing_required("OnvifAddress");
if (!has_CameraIndex) set_missing_required("CameraIndex");
if (!has_UseProfileToken) set_missing_required("UseProfileToken");
if (!has_OnvifProfileToken) set_missing_required("OnvifProfileToken");
if (!has_Recording) set_missing_required("Recording");
if (!has_GroupId) set_missing_required("GroupId");
if (!has_HdfsRecording) set_missing_required("HdfsRecording");
if (!has_OnvifProfileToken2) set_missing_required("OnvifProfileToken2");
if (!has_ConnectType) set_missing_required("ConnectType");
if (!has_Mining) set_missing_required("Mining");
if (!has_HWAccel) set_missing_required("HWAccel");
if (!has_IPV6) set_missing_required("IPV6");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;
case 1:
     handler_1.ReapError(errs); break;
case 2:
     handler_2.ReapError(errs); break;
case 3:
     handler_3.ReapError(errs); break;
case 4:
     handler_4.ReapError(errs); break;
case 5:
     handler_5.ReapError(errs); break;
case 6:
     handler_6.ReapError(errs); break;
case 7:
     handler_7.ReapError(errs); break;
case 8:
     handler_8.ReapError(errs); break;
case 9:
     handler_9.ReapError(errs); break;
case 10:
     handler_10.ReapError(errs); break;
case 11:
     handler_11.ReapError(errs); break;
case 12:
     handler_12.ReapError(errs); break;
case 13:
     handler_13.ReapError(errs); break;
case 14:
     handler_14.ReapError(errs); break;
case 15:
     handler_15.ReapError(errs); break;
case 16:
     handler_16.ReapError(errs); break;
case 17:
     handler_17.ReapError(errs); break;
case 18:
     handler_18.ReapError(errs); break;
case 19:
     handler_19.ReapError(errs); break;
case 20:
     handler_20.ReapError(errs); break;
case 21:
     handler_21.ReapError(errs); break;
case 22:
     handler_22.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();
handler_2.PrepareForReuse();
handler_3.PrepareForReuse();
handler_4.PrepareForReuse();
handler_5.PrepareForReuse();
handler_6.PrepareForReuse();
handler_7.PrepareForReuse();
handler_8.PrepareForReuse();
handler_9.PrepareForReuse();
handler_10.PrepareForReuse();
handler_11.PrepareForReuse();
handler_12.PrepareForReuse();
handler_13.PrepareForReuse();
handler_14.PrepareForReuse();
handler_15.PrepareForReuse();
handler_16.PrepareForReuse();
handler_17.PrepareForReuse();
handler_18.PrepareForReuse();
handler_19.PrepareForReuse();
handler_20.PrepareForReuse();
handler_21.PrepareForReuse();
handler_22.PrepareForReuse();

    }
};

template < class Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8 >
struct Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, ::oapi::Device > {

    void operator()( Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8& w, const ::oapi::Device& value) const
    {
        w.StartObject();

        w.Key("\x6e\x49\x64", 3, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.nId);
w.Key("\x6e\x54\x79\x70\x65", 5, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.nType);
w.Key("\x6e\x53\x75\x62\x54\x79\x70\x65", 8, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.nSubType);
w.Key("\x4e\x61\x6d\x65", 4, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.Name);
w.Key("\x50\x61\x72\x61\x6d", 5, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.Param);
w.Key("\x49\x50", 2, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.IP);
w.Key("\x50\x6f\x72\x74", 4, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.Port);
w.Key("\x55\x73\x65\x72", 4, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.User);
w.Key("\x50\x61\x73\x73\x77\x6f\x72\x64", 8, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.Password);
w.Key("\x52\x74\x73\x70\x4c\x6f\x63\x61\x74\x69\x6f\x6e", 12, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.RtspLocation);
w.Key("\x46\x69\x6c\x65\x4c\x6f\x63\x61\x74\x69\x6f\x6e", 12, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.FileLocation);
w.Key("\x4f\x6e\x76\x69\x66\x41\x64\x64\x72\x65\x73\x73", 12, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.OnvifAddress);
w.Key("\x43\x61\x6d\x65\x72\x61\x49\x6e\x64\x65\x78", 11, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.CameraIndex);
w.Key("\x55\x73\x65\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e", 15, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.UseProfileToken);
w.Key("\x4f\x6e\x76\x69\x66\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e", 17, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.OnvifProfileToken);
w.Key("\x52\x65\x63\x6f\x72\x64\x69\x6e\x67", 9, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.Recording);
w.Key("\x47\x72\x6f\x75\x70\x49\x64", 7, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.GroupId);
w.Key("\x48\x64\x66\x73\x52\x65\x63\x6f\x72\x64\x69\x6e\x67", 13, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.HdfsRecording);
w.Key("\x4f\x6e\x76\x69\x66\x50\x72\x6f\x66\x69\x6c\x65\x54\x6f\x6b\x65\x6e\x32", 18, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, std::string >()(w, value.OnvifProfileToken2);
w.Key("\x43\x6f\x6e\x6e\x65\x63\x74\x54\x79\x70\x65", 11, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.ConnectType);
w.Key("\x4d\x69\x6e\x69\x6e\x67", 6, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.Mining);
w.Key("\x48\x57\x41\x63\x63\x65\x6c", 7, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.HWAccel);
w.Key("\x49\x50\x56\x36", 4, false); Serializer< Writer81f9667a09efaccfd42bab9c4422f49db45bc06881e43cbce9906ee37c0091d8, unsigned int >()(w, value.IPV6);

        w.EndObject(23);
    }

};
}


// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

namespace oapi { struct DeviceList {
 unsigned int Num;
std::vector<oapi::Device> list;

explicit DeviceList():Num(), list() {  }


 
}; }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::oapi::DeviceList > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< unsigned int > handler_0;
SAXEventHandler< std::vector<oapi::Device> > handler_1;bool has_Num;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "Num";
case 1:
    return "list";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_Num = false;

    }

public:
    explicit SAXEventHandler( ::oapi::DeviceList * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->Num)
, handler_1(&obj->list)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

case 1:
    return checked_event_forwarding(handler_1.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

case 1:
    return checked_event_forwarding(handler_1.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

case 1:
    return checked_event_forwarding(handler_1.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

case 1:
    return checked_event_forwarding(handler_1.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

case 1:
    return checked_event_forwarding(handler_1.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

case 1:
    return checked_event_forwarding(handler_1.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

case 1:
    return checked_event_forwarding(handler_1.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x4e\x75\x6d", 3))
                         { state=0; has_Num = true; }
else if (utility::string_equal(str, length, "\x6c\x69\x73\x74", 4))
                         { state=1;  }
            else {
                state = -1;
                return true;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

case 1:
    return checked_event_forwarding(handler_1.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

case 1:
    return checked_event_forwarding(handler_1.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

case 1:
    return checked_event_forwarding(handler_1.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

case 1:
    return checked_event_forwarding(handler_1.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_Num) set_missing_required("Num");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;
case 1:
     handler_1.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();

    }
};

template < class Writer2f45fe8d27bc21dd98aeb3ee45efe9e571b84454b0e47abd731bcf99ee9a64f8 >
struct Serializer< Writer2f45fe8d27bc21dd98aeb3ee45efe9e571b84454b0e47abd731bcf99ee9a64f8, ::oapi::DeviceList > {

    void operator()( Writer2f45fe8d27bc21dd98aeb3ee45efe9e571b84454b0e47abd731bcf99ee9a64f8& w, const ::oapi::DeviceList& value) const
    {
        w.StartObject();

        w.Key("\x4e\x75\x6d", 3, false); Serializer< Writer2f45fe8d27bc21dd98aeb3ee45efe9e571b84454b0e47abd731bcf99ee9a64f8, unsigned int >()(w, value.Num);
w.Key("\x6c\x69\x73\x74", 4, false); Serializer< Writer2f45fe8d27bc21dd98aeb3ee45efe9e571b84454b0e47abd731bcf99ee9a64f8, std::vector<oapi::Device> >()(w, value.list);

        w.EndObject(2);
    }

};
}


