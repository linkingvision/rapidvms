
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XStreamIO_h
#define XSDK_XStreamIO_h

#include "XSDK/XDuration.h"
#include "XSDK/XRef.h"

#ifdef WIN32
    #include <BaseTsd.h>

    #define ssize_t SSIZE_T
#endif

namespace XSDK
{
    /// This typedef describes the interface to a function that will be called when
    /// their is either a send or recv timeout. A return value of true means "try again".
    typedef bool (*TimeoutHandler)( void* opaque );

    class XStreamIO
    {
    public:
        X_API XStreamIO(){}
        X_API virtual ~XStreamIO(){}

        /// Whether this XStreamIO is valid to use (i.e. is open and ready to be used).
        X_API virtual bool Valid()=0;

        /// Send data to server. May generate send timeout callbacks.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        X_API virtual size_t Send( const void* msg, size_t msgLen )=0;

        /// Send data to server. May generate send timeout callbacks.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        /// \param sendTimeout Override send timeout for this call only.
        X_API virtual size_t Send( const void* msg, size_t msgLen, XDuration sendTimeout )=0;
        X_API virtual size_t Send( const void* msg, size_t msgLen, int sendTimeoutMillis )
        { return Send(msg, msgLen, XDuration(MSECS, sendTimeoutMillis)); }

        /// Recieve data from server. May generate recv timeout callbacks.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to recieve.
        X_API virtual size_t Recv( void* buf, size_t msgLen )=0;

        /// Recieve data from server. May generate recv timeout callbacks.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to recieve.
        /// \param recvTimeout Override recv timeout for this call only.
        X_API virtual size_t Recv( void* buf, size_t msgLen, XDuration recvTimeout )=0;
        X_API virtual size_t Recv( void* buf, size_t msgLen, int recvTimeoutMillis )
        { return Recv(buf, msgLen, XDuration(MSECS, recvTimeoutMillis)); }

            /// Attempts to pre buffer some data to be received.
            /// Returns true if some data was buffered.
        X_API virtual bool BufferedRecv()=0;

        /// Send data to server. May write less bytes than asked for.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        X_API virtual ssize_t RawSend( const void* msg, size_t msgLen )=0;

        /// Recieve data from server. May read less bytes than asked for.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to recieve.
        X_API virtual ssize_t RawRecv( void* buf, size_t msgLen )=0;

        /// Attach a handler function pointer to send notifications about recv timeouts.
        /// \param rtcb A pointer to a function to call when recv times out.
        /// \param opaque User data to be passed to rtcb.
        X_API virtual void AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque )=0;

        /// Explicitly set a recv timeout value.
        /// \param timeout The length of time to attempt a recv before failing.
        X_API virtual void SetRecvTimeout( XDuration timeout )=0;
        X_API virtual void SetRecvTimeout( int milliseconds )
        { SetRecvTimeout(XDuration(MSECS, milliseconds)); }

        /// Attach a handler function pointer to send notifications about send timeouts.
        /// \param stcb A pointer to a function to call when send times out.
        /// \param opaque User data to be passed to stcb.
        X_API virtual void AttachSendTimeoutHandler( TimeoutHandler stcb, void* opaque )=0;

        /// Explicitly set a send timeout value.
        /// \param timeout The length of time to attempt a send before failing.
        X_API virtual void SetSendTimeout( XDuration timeout )=0;
        X_API virtual void SetSendTimeout( int milliseconds )
        { SetSendTimeout(XDuration(MSECS, milliseconds)); }

        /// Wait for data to be available to be recieved on the socket.
        X_API virtual bool WaitRecv( XDuration& waitMillis )=0;
        X_API virtual bool WaitRecv( int& waitMillis )
        {
            XDuration waitTime = XDuration(MSECS, waitMillis);
            try
            {
                const bool result = WaitRecv(waitTime);
                waitMillis = (int)waitTime.Total(MSECS);
                return result;
            }
            catch(...)
            {
                waitMillis = (int)waitTime.Total(MSECS);
                throw;
            }
        }

        /// Wait for socket clear to send.
        X_API virtual bool WaitSend( XDuration& wait )=0;
        X_API virtual bool WaitSend( int& waitMillis )
        {
            XDuration waitTime = XDuration(MSECS, waitMillis);
            try
            {
                const bool result = WaitSend(waitTime);
                waitMillis = (int)waitTime.Total(MSECS);
                return result;
            }
            catch(...)
            {
                waitMillis = (int)waitTime.Total(MSECS);
                throw;
            }
        }

        /// Returns true if a RawRecv() would not block.
        X_API virtual bool ReadyToRecv()=0;

        /// Returns true if a RawSend() would not block.
        X_API virtual bool ReadyToSend()=0;

        /// Get Host used for Connect
        X_API virtual XSDK::XString GetHost() const=0;
        X_API virtual uint32_t GetHostPort() const=0;


        X_API virtual XSDK::XString GetPeerIp() const=0;

        X_API virtual void Close()=0;
    };

};



#endif//XStreamIO
