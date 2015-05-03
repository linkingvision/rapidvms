
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

#ifndef cppkit_udp_sender_h
#define cppkit_udp_sender_h

#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_socket_address.h"

#ifdef WIN32
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #include <BaseTsd.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>

    typedef int SOCKET;
#endif

namespace cppkit
{

class ck_udp_sender
{
public:
    CK_API ck_udp_sender() = delete;
    CK_API ck_udp_sender( const ck_udp_sender& ) = delete;
    CK_API ck_udp_sender( const ck_string& targetIP,
                          int targetPort,
                          const ck_string& localInterfaceIP = "",
                          int localPort = 0 );

    CK_API virtual ~ck_udp_sender() throw();

    CK_API ck_udp_sender& operator = ( const ck_udp_sender& ) = delete;

    CK_API void aim( const ck_string& targetIP, int targetPort );

    CK_API int32_t send( void* buffer, size_t length );

    CK_API virtual size_t get_send_buffer_size();

    CK_API virtual void set_send_buffer_size(size_t size);

private:
    void _configure();
    void _close() throw();

    SOCKET _sok;
    ck_socket_address _addr;
    ck_string _localInterfaceIP;
    int _localPort;
};

}

#endif
