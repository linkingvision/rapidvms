
#ifndef cppkit_udp_receiver_h
#define cppkit_udp_receiver_h

#include "cppkit/os/ck_platform.h"
#include "cppkit/ck_socket.h"
#include "cppkit/ck_string.h"
#include "cppkit/ck_memory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_socket_address.h"
#include <memory>
#include <list>

namespace cppkit
{

// 0 here means do not set.
const int DEFAULT_UDP_RECV_BUF_SIZE = 0;

class ck_udp_receiver
{
public:
    /// @note If destinationAddress is empty, then it defaults to IPv4
    CK_API ck_udp_receiver( int destinationPort = 0,
                            int recvBufferSize = DEFAULT_UDP_RECV_BUF_SIZE,
                            const ck_string& destinationAddress = ip4_addr_any );

    CK_API ck_udp_receiver( const ck_udp_receiver& ) = delete;

    CK_API virtual ~ck_udp_receiver() throw();

    CK_API ck_udp_receiver& operator = ( const ck_udp_receiver& ) = delete;

    CK_API void connect( const ck_string& ipAddress, int port );

    CK_API virtual bool receive( int& port, std::shared_ptr<ck_memory> buffer );

    //Minumum timeout is 250ms.
    CK_API virtual bool receive( int& port, std::shared_ptr<ck_memory> buffer, int waitMillis );

    CK_API virtual bool non_blocking_receive( int& port, std::shared_ptr<ck_memory> buffer );

    CK_API virtual bool raw_receive( int& port, std::shared_ptr<ck_memory> buffer );

    CK_API virtual void shutdown();

    CK_API virtual void close();

    CK_API virtual void associate( std::shared_ptr<ck_udp_receiver> receiver );

    CK_API virtual void clear_associations();

    CK_API virtual int get_bound_port();

    CK_API virtual size_t get_recv_buffer_size();

    CK_API virtual void set_recv_buffer_size( size_t size );

private:

    bool _receive( int& port, std::shared_ptr<ck_memory> buffer, bool block, int waitMillis );

    SOCKET _sok;
    ck_socket_address _addr;
    std::list<std::shared_ptr<ck_udp_receiver> > _associatedReceivers;
};

}


#endif
