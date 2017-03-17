
#include "hyperhttp_client_server_test.h"
#include "hyperhttp/client_request.h"
#include "hyperhttp/server_request.h"
#include "hyperhttp/server_response.h"
#include "hyperhttp/client_response.h"
#include "hyperhttp/methods.h"
#include "cppkit/ck_socket.h"
#include "cppkit/os/ck_time_utils.h"
#include <thread>
#include <memory>

using namespace std;
using namespace std::placeholders;
using namespace cppkit;
using namespace hyperhttp;

REGISTER_TEST_FIXTURE(hyperhttp_client_server_test);

#define TRY_N_TIMES(a,b)     \
{                            \
    int tries = 0;           \
    while(!a && (tries < b)) \
    {                        \
        ck_usleep(100000);   \
        tries++;             \
    }                        \
}

void hyperhttp_client_server_test::test_get()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        ck_string acceptType;
        UT_ASSERT( serverRequest.get_header( "accept", acceptType ) );
        UT_ASSERT( acceptType == "application/json" );

        UT_ASSERT( serverRequest.get_method() == METHOD_GET );

        server_response serverResponse;

        ck_string rawURI = serverRequest.get_uri().get_full_raw_uri();
        if( rawURI.contains( "ping" ) )
            serverResponse.set_body( "pong" );

        serverResponse.write_response( connected );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_accept_type( "application/json" );
    clientRequest.set_uri( "/ping" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    clientResponse.read_response( clientSok );

    UT_ASSERT( clientResponse.get_body_as_string().contains( "pong" ) );
}

void hyperhttp_client_server_test::test_post()
{
    ck_string bodyText = "{ \"glossary\": { \"title\": \"example glossary\", \"GlossDiv\": { \"title\": \"S\", \"GlossList\": { \"GlossEntry\": { \"ID\": \"SGML\", \"SortAs\": \"SGML\", \"GlossTerm\": \"Standard Generalized Markup Language\", \"Acronym\": \"SGML\", \"Abbrev\": \"ISO 8879:1986\", \"GlossDef\": { \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\", \"GlossSeeAlso\": [\"GML\", \"XML\"] }, \"GlossSee\": \"markup\" } } } } }";

    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        UT_ASSERT( serverRequest.get_method() == METHOD_POST );

        ck_string body = serverRequest.get_body_as_string();
        UT_ASSERT( bodyText.length() == body.length() );
        UT_ASSERT( bodyText == body );

        ck_string rawURI = serverRequest.get_uri().get_full_raw_uri();
        UT_ASSERT( rawURI.contains( "ping" ) );

        server_response serverResponse;
        serverResponse.set_body( "pong" );
        serverResponse.write_response( connected );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_uri( "/ping" );
    clientRequest.set_method( METHOD_POST );
    clientRequest.set_content_type( "application/json" );
    clientRequest.set_body( bodyText );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    clientResponse.read_response( clientSok );

    UT_ASSERT( clientResponse.get_body_as_string().contains( "pong" ) );
}

void hyperhttp_client_server_test::test_chunked_response_lambda()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        UT_ASSERT( serverRequest.get_method() == METHOD_GET );

        server_response serverResponse;

        for( size_t i = 0; i < 100; i++ )
        {
            shared_ptr<ck_memory> chunk = make_shared<ck_memory>();
            chunk->extend_data( 1024 );

            serverResponse.write_chunk( connected, chunk );
        }

        serverResponse.write_chunk_finalizer( connected );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_uri( "/ping" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    int numChunks = 0;
    clientResponse.register_chunk_callback( [&]( shared_ptr<ck_memory> chunk, const client_response& resp)->bool{ ++numChunks; return true; } );
    clientResponse.read_response( clientSok );
    UT_ASSERT( numChunks == 100 );
}

class call_backy
{
public:
    bool callback( shared_ptr<ck_memory> chunk, const client_response& resp ) { ++_numCalls; return true; }
    int _numCalls;
};

void hyperhttp_client_server_test::test_chunked_response_bind()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        UT_ASSERT( serverRequest.get_method() == METHOD_GET );

        server_response serverResponse;

        for( size_t i = 0; i < 100; i++ )
        {
            shared_ptr<ck_memory> chunk = make_shared<ck_memory>();
            chunk->extend_data( 1024 );

            serverResponse.write_chunk( connected, chunk );
        }

        serverResponse.write_chunk_finalizer( connected );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_uri( "/ping" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    call_backy cb;
    cb._numCalls = 0;

    // First: note the "using std::placeholders;" weirdness above.
    // std::bind can take a variable number of arguments. In this case, I'm telling std::bind that on a call to
    // call_backy::callback the first argument should be the object (effectively "this") and that beyond that this
    // call takes two more arguments that will be provided at call time. Here I use the std::placeholder values of
    // _1 and _2. Weird, but awesome.
    clientResponse.register_chunk_callback( std::bind( &call_backy::callback, &cb, _1, _2 ) );
    clientResponse.read_response( clientSok );

    UT_ASSERT( cb._numCalls == 100 );
}

void hyperhttp_client_server_test::test_multipart_response_lambda()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        UT_ASSERT( serverRequest.get_method() == METHOD_GET );

        server_response serverResponse;
        serverResponse.set_content_type( "multipart/mixed;" );

        serverResponse.write_response( connected );

        for( size_t i = 0; i < 100; i++ )
        {
            shared_ptr<ck_memory> chunk = make_shared<ck_memory>();
            chunk->extend_data( 1024 );

            map<string,ck_string> partHeaders;
            serverResponse.write_part( connected, "boundary", partHeaders, chunk );
        }

        serverResponse.write_part_finalizer( connected, "boundary" );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_uri( "/ping" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    int numParts = 0;

    clientResponse.register_part_callback( [&]( shared_ptr<ck_memory> chunk, const map<string,ck_string>& partHeaders, const client_response& resp)->bool{ ++numParts; return true; } );

    clientResponse.read_response( clientSok );

    UT_ASSERT( clientResponse.is_success() );

    UT_ASSERT( numParts == 100 );
}

void hyperhttp_client_server_test::test_post_with_vars()
{
    int port = UT_NEXT_PORT();

    thread t([&](){
        ck_socket serverSok;
        serverSok.bind( port );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        UT_ASSERT( serverRequest.get_method() == METHOD_POST );

        auto postVars = serverRequest.get_post_vars();

        UT_ASSERT( postVars.find( "foo" ) != postVars.end() );
        UT_ASSERT( postVars.find( "foo" )->second == "foo_val" );
        UT_ASSERT( postVars.find( "bar" ) != postVars.end() );
        UT_ASSERT( postVars.find( "bar" )->second == "bar_val" );
        UT_ASSERT( postVars.find( "baz" ) != postVars.end() );
        UT_ASSERT( postVars.find( "baz" )->second == "baz_val" );
        UT_ASSERT( postVars.find( "not_found" ) == postVars.end() );

        server_response serverResponse;
        serverResponse.write_response( connected );
    });
    t.detach();

    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    TRY_N_TIMES( clientSok->query_connect( "127.0.0.1", port, 1), 10 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_method( METHOD_POST );
    clientRequest.set_uri( "/ping" );
    clientRequest.add_post_var( "foo", "foo_val" );
    clientRequest.add_post_var( "bar", "bar_val" );
    clientRequest.add_post_var( "baz", "baz_val" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    clientResponse.read_response( clientSok );

    UT_ASSERT( clientResponse.is_success() );
}
