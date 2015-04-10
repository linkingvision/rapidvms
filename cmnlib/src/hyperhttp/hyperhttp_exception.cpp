
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// hyperhttp - http://www.cppkit.org
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

#include "hyperhttp/hyperhttp_exception.h"

using namespace hyperhttp;
using namespace cppkit;

hyperhttp_exception::hyperhttp_exception() :
    ck_exception()
{
}

hyperhttp_exception::hyperhttp_exception( const char* msg, ... ) :
    ck_exception()
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

hyperhttp_exception::hyperhttp_exception( const ck_string& msg ) :
    ck_exception( msg )
{
}

http_exception::http_exception( int statusCode ) :
    hyperhttp_exception(),
    _statusCode( statusCode )
{
}

http_exception::http_exception( int statusCode, const char* msg, ... ) :
    hyperhttp_exception(),
    _statusCode( statusCode )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_exception::http_exception( int statusCode, const ck_string& msg ) :
    hyperhttp_exception( msg ),
    _statusCode( statusCode )
{
}

http_400_exception::http_400_exception() :
    http_exception( 400 )
{
}

http_400_exception::http_400_exception( const char* msg, ... ) :
    http_exception( 400 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_400_exception::http_400_exception( const ck_string& msg ) :
    http_exception( 400, msg )
{
}

http_401_exception::http_401_exception() :
    http_exception( 401 )
{
}

http_401_exception::http_401_exception( const char* msg, ... ) :
    http_exception( 401 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_401_exception::http_401_exception( const ck_string& msg ) :
    http_exception( 401, msg )
{
}

http_403_exception::http_403_exception() :
    http_exception( 403 )
{
}

http_403_exception::http_403_exception( const char* msg, ... ) :
    http_exception( 403 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_403_exception::http_403_exception( const ck_string& msg ) :
    http_exception( 403, msg )
{
}

http_404_exception::http_404_exception() :
    http_exception( 404 )
{
}

http_404_exception::http_404_exception( const char* msg, ... ) :
    http_exception( 404 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_404_exception::http_404_exception( const ck_string& msg ) :
    http_exception( 404, msg )
{
}

http_415_exception::http_415_exception() :
    http_exception( 415 )
{
}

http_415_exception::http_415_exception( const char* msg, ... ) :
    http_exception( 415 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_415_exception::http_415_exception( const ck_string& msg ) :
    http_exception( 415, msg )
{
}

http_453_exception::http_453_exception() :
    http_exception( 453 )
{
}

http_453_exception::http_453_exception( const char* msg, ... ) :
    http_exception( 453 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_453_exception::http_453_exception( const ck_string& msg ) :
    http_exception( 453, msg )
{
}

http_500_exception::http_500_exception() :
    http_exception( 500 )
{
}

http_500_exception::http_500_exception( const char* msg, ... ) :
    http_exception( 500 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_500_exception::http_500_exception( const ck_string& msg ) :
    http_exception( 500, msg )
{
}

http_501_exception::http_501_exception() :
    http_exception( 501 )
{
}

http_501_exception::http_501_exception( const char* msg, ... ) :
    http_exception( 501 )
{
    va_list args;
    va_start( args, msg );
    set_msg( ck_string::format( msg, args ) );
    va_end( args );
}

http_501_exception::http_501_exception( const ck_string& msg ) :
    http_exception( 501, msg )
{
}

void hyperhttp::throw_http_exception( int statusCode, const char* msg, ... )
{
    va_list args;
    va_start( args, msg );
    const ck_string message = ck_string::format( msg, args );
    va_end( args );

    throw_http_exception( statusCode, message );
}

void hyperhttp::throw_http_exception( int statusCode, const ck_string& msg )
{
    switch( statusCode )
    {
        case 400:
        {
            http_400_exception e( msg );
            e.set_type_name("http_400_exception");
            throw e;
        }
        case 401:
        {
            http_401_exception e( msg );
            e.set_type_name("http_401_exception");
            throw e;
        }
        case 403:
        {
            http_403_exception e( msg );
            e.set_type_name("http_403_exception");
            throw e;
        }
        case 404:
        {
            http_404_exception e( msg );
            e.set_type_name("http_404_exception");
            throw e;
        }
        case 415:
        {
            http_415_exception e( msg );
            e.set_type_name("http_415_exception");
            throw e;
        }
        case 453:
        {
            http_453_exception e( msg );
            e.set_type_name("http_453_exception");
            throw e;
        }
        case 500:
        {
            http_500_exception e( msg );
            e.set_type_name("http_500_exception");
            throw e;
        }
        case 501:
        {
            http_501_exception e( msg );
            e.set_type_name("http_501_exception");
            throw e;
        }
        default:
        {
            http_exception e( statusCode, msg );
            e.set_type_name("http_exception");
            throw e;
        }
    }
}

