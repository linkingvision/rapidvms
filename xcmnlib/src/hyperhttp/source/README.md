hyperhttp
=====

<b>A C++11 HTTP library dependent on cppkit.</b>

The main idea in hyperhttp is that you have 4 types of objects that encapsulate a complete HTTP round trip.

<pre>
  <b>client_request</b> - Used by a client to represent a request to be sent to a server.
  <b>server_request</b> - Used by a server to represent a request sent to it from a client.
  <b>server_response</b> - Used by a server to represent a response to a request to be sent to a client.
  <b>client_response</b> - Used by a client to represent a response from a server.
</pre>

Instances of these objects implement methods like "write_request()", "read_request()", "write_response()" and
"read_response()" and know how to serialize themselves to and from sockets. Using hyperhttp essentially involves creating
and populating instances of these objects, connecting to a server with a ck_socket and then serializing and
deserializing these object to and from the socket.

Here is a simple example (adapted from the unit tests):

    thread t([&](){
        // server
        //
        ck_socket serverSok;
        serverSok.bind( 8080 );
        serverSok.listen();
        auto connected = serverSok.accept();

        server_request serverRequest;
        serverRequest.read_request( connected );

        server_response serverResponse;
        serverResponse.set_body( "<html><head></head><body><h1>Hello, World!</h1></body></html>" );
        serverResponse.write_response( connected );
    });
    t.detach();

    // client
    //
    shared_ptr<ck_socket> clientSok = make_shared<ck_socket>();
    clientSok->query_connect( "127.0.0.1", 8080 );

    client_request clientRequest( clientSok->get_host(), clientSok->get_host_port() );
    clientRequest.set_uri( "/ping" );
    clientRequest.write_request( clientSok );

    client_response clientResponse;
    clientResponse.read_response( clientSok );

    // at this point, clientResponse.get_body_as_string() contains the HTML returned from the above
    // server.

The above code implements both a server and a client. The code in the above "thread" lambda implements the server and the remainder of the code implements the client and connects to the server.

<b>What is hyperhttp good for?</b>

hyperhttp is not designed to be a general purpose web server. hyperhttp is strictly focused on allowing C++ code to implement
restful web services. There is a lot more involved with implementing a web server than these 4 objects. That said, these
4 objects are very simple and easy to use and can be used to implement a useful subset of the HTTP spec focused on
restful web services.

<b>hyperhttp is part of the cppkit ecosystem.</b>

hyperhttp depends upon cppkit for many things (sockets, nullable types, etc...) and depends on the cppkit build ecosystem.
Building hyperhttp at this point first requires checking out and building cppkit. cppkit and hyperhttp also depend on cmake.

<b>Building and installing hyperhttp and cppkit on Linux.</b>

<pre>
git clone http://github.com/dicroce/cppkit
cd cppkit
mkdir build
cd build
cmake ..
make
make install

cd ../..
git clone http://github.com/dicroce/hyperhttp
cd hyperhttp
mkdir build
cd build
cmake ..
make
make install
</pre>

<b>Building and installing hyperhttp and cppkit on Windows.</b>

<pre>
git clone http://github.com/dicroce/cppkit
cd cppkit
mkdir build
cd build
cmake ..
cmake --build . --clean-first
cmake --build . --target install

cd ../..
git clone http://github.com/dicroce/hyperhttp
cd hyperhttp
mkdir build
cd build
cmake ..
cmake --build . --clean-first
cmake --build . --target install
</pre>
