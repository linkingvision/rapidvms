
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

#ifndef cppkit_stream_io_h
#define cppkit_stream_io_h

#include "cppkit/ck_string.h"

#include <functional>

#ifdef IS_WINDOWS
  #include <BaseTsd.h>
  #define ssize_t SSIZE_T
#endif

namespace cppkit
{

typedef std::function<bool()> timeout_callback;

class ck_stream_io
{
public:
    /// Whether this ck_stream_io is valid to use (i.e. is open and ready to be used).
    CK_API virtual bool valid()=0;

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    CK_API virtual size_t send(const void* msg, size_t msgLen)=0;

    /// Send data to server. May generate send timeout callbacks.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    /// \param sendTimeoutMillis Override send timeout for this call only.
    CK_API virtual size_t send(const void* msg, size_t msgLen, int sendTimeoutMillis)=0;

    /// Recieve data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
    CK_API virtual size_t recv(void* buf, size_t msgLen)=0;

    /// Recieve data from server. May generate recv timeout callbacks.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
    /// \param recvTimeoutMillis Override recv timeout for this call only.
    CK_API virtual size_t recv(void* buf, size_t msgLen, int recvTimeoutMillis)=0;

    /// Attempts to pre buffer some data to be received.
    /// Returns true if some data was buffered.
    CK_API virtual bool buffered_recv()=0;

    /// Send data to server. May write less bytes than asked for.
    /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
    /// \param msgLen The length of the data to send.
    CK_API virtual ssize_t raw_send(const void* msg, size_t msgLen)=0;

    /// Recieve data from server. May read less bytes than asked for.
    /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
    /// \param msgLen The length of the data to recieve.
    CK_API virtual ssize_t raw_recv(void* buf, size_t msgLen)=0;

    /// Attach a handler function pointer to send notifications about recv timeouts.
    /// \param rtcb A pointer to a function to call when recv times out.
    CK_API virtual void register_recv_timeout_callback( timeout_callback cb ) = 0;

    /// Explicitly set a recv timeout value.
    /// \param milliseconds The milliseconds to attempt a recv before failing.
    CK_API virtual void set_recv_timeout(int milliseconds)=0;

    /// Attach a handler function pointer to send notifications about send timeouts.
    /// \param stcb A pointer to a function to call when send times out.
    CK_API virtual void register_send_timeout_callback( timeout_callback cb ) = 0;

    /// Explicitly set a send timeout value.
    /// \param milliseconds The milliseconds to attempt a send before failing.
    CK_API virtual void set_send_timeout(int milliseconds)=0;

    /// Wait for data to be available to be recieved on the socket.
    CK_API virtual bool wait_recv(int& waitMillis)=0;

    /// Wait for socket clear to send.
    CK_API virtual bool wait_send(int& waitMillis)=0;

    /// Returns true if a raw_recv() would not block.
    CK_API virtual bool ready_to_recv()=0;

    /// Returns true if a raw_send() would not block.
    CK_API virtual bool ready_to_send()=0;

    CK_API virtual void close()=0;
};

}

#endif
