/*
 *
 * Copyright (c) 2012 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file mhd2spdy_spdy.c
 * @brief  SPDY part of the proxy. libspdylay is used for the client side.
 *         The example spdycli.c from spdylay was used as basis;
 *         however, multiple changes were made.
 * @author Tatsuhiro Tsujikawa
 * @author Andrey Uzunov
 */

#include "mhd2spdy_structures.h"
#include "mhd2spdy_spdy.h"
#include "mhd2spdy_http.h"


/*
 * Prints error containing the function name |func| and message |msg|
 * and exit.
 */
static void
spdy_dief(const char *func,
          const char *msg)
{
  fprintf(stderr, "FATAL: %s: %s\n", func, msg);
  exit(EXIT_FAILURE);
}


/*
 * Prints error containing the function name |func| and error code
 * |error_code| and exit.
 */
void
spdy_diec(const char *func,
          int error_code)
{
  fprintf(stderr, "FATAL: %s: error_code=%d, msg=%s\n", func, error_code,
          spdylay_strerror(error_code));
  exit(EXIT_FAILURE);
}


static ssize_t
spdy_cb_data_source_read(spdylay_session *session, int32_t stream_id, uint8_t *buf, size_t length, int *eof, spdylay_data_source *source, void *user_data)
{
  (void)session;
  (void)stream_id;
  (void)user_data;
  
  ssize_t ret;
  assert(NULL != source);
  assert(NULL != source->ptr);
	struct Proxy *proxy = (struct Proxy *)(source->ptr);
	void *newbody;
  
 
  if(length < 1)
  {
    PRINT_INFO("spdy_cb_data_source_read: length is 0");
    return 0;
	}
  
	if(!proxy->received_body_size)//nothing to write now
  {
    if(proxy->receiving_done)
    {
      PRINT_INFO("POST spdy EOF");
      *eof = 1;
    }
      PRINT_INFO("POST SPDYLAY_ERR_DEFERRED");
		return SPDYLAY_ERR_DEFERRED;//TODO SPDYLAY_ERR_DEFERRED should be used
  }
	
	if(length >= proxy->received_body_size)
	{
		ret = proxy->received_body_size;
		newbody = NULL;
	}
	else
	{
		ret = length;
		if(NULL == (newbody = malloc(proxy->received_body_size - length)))
		{
			PRINT_INFO("no memory");
			return SPDYLAY_ERR_TEMPORAL_CALLBACK_FAILURE;
		}
		memcpy(newbody, proxy->received_body + length, proxy->received_body_size - length);
	}
	memcpy(buf, proxy->received_body, ret);
	free(proxy->received_body);
	proxy->received_body = newbody;
	proxy->received_body_size -= ret;
  
  if(0 == proxy->received_body_size && proxy->receiving_done)
    {
      PRINT_INFO("POST spdy EOF");
    *eof = 1;
  }
  
  PRINT_INFO2("given POST bytes to spdylay: %zd", ret);
  
  return ret;
}


/*
 * The implementation of spdylay_send_callback type. Here we write
 * |data| with size |length| to the network and return the number of
 * bytes actually written. See the documentation of
 * spdylay_send_callback for the details.
 */
static ssize_t
spdy_cb_send(spdylay_session *session,
             const uint8_t *data,
             size_t length,
             int flags,
             void *user_data)
{
  (void)session;
  (void)flags;
  
  //PRINT_INFO("spdy_cb_send called");
  struct SPDY_Connection *connection;
  ssize_t rv;
  connection = (struct SPDY_Connection*)user_data;
  connection->want_io = IO_NONE;
  
  if(glob_opt.ignore_rst_stream
    && 16 == length
    && 0x80 == data[0]
    && 0x00 == data[2]
    && 0x03 == data[3]
    )
  {
    PRINT_INFO2("ignoring RST_STREAM for stream_id %i %i %i %i", data[8], data[9], data[10], data[11]);
    glob_opt.ignore_rst_stream = false;
    return 16;
  }
  glob_opt.ignore_rst_stream = false;
  
  if(connection->is_tls)
  {
    ERR_clear_error();
    rv = SSL_write(connection->ssl, data, length);
    if(rv < 0) {
      int err = SSL_get_error(connection->ssl, rv);
      if(err == SSL_ERROR_WANT_WRITE || err == SSL_ERROR_WANT_READ) {
        connection->want_io |= (err == SSL_ERROR_WANT_READ ?
                               WANT_READ : WANT_WRITE);
        rv = SPDYLAY_ERR_WOULDBLOCK;
      } else {
        rv = SPDYLAY_ERR_CALLBACK_FAILURE;
      }
    }
  }
  else
  {
    rv = write(connection->fd, 
            data,
            length);
            
    if (rv < 0)
    {
      switch(errno)
      {				
        case EAGAIN:
  #if EAGAIN != EWOULDBLOCK
        case EWOULDBLOCK:
  #endif
          connection->want_io |= WANT_WRITE;
          rv = SPDYLAY_ERR_WOULDBLOCK;
          break;
          
        default:
          rv = SPDYLAY_ERR_CALLBACK_FAILURE;
      }
    }
  }
  
  PRINT_INFO2("%zd bytes written by spdy", rv);
  
  if(rv > 0)
    UPDATE_STAT(glob_stat.spdy_bytes_sent, rv);
  
  return rv;
}


/*
 * The implementation of spdylay_recv_callback type. Here we read data
 * from the network and write them in |buf|. The capacity of |buf| is
 * |length| bytes. Returns the number of bytes stored in |buf|. See
 * the documentation of spdylay_recv_callback for the details.
 */
static ssize_t
spdy_cb_recv(spdylay_session *session,
             uint8_t *buf,
             size_t length, 
             int flags,
             void *user_data)
{
  (void)session;
  (void)flags;
  
  struct SPDY_Connection *connection;
  ssize_t rv;
  
  connection = (struct SPDY_Connection*)user_data;
  //prevent monopolizing everything
  if(!(++connection->counter % 10)) return SPDYLAY_ERR_WOULDBLOCK;
  connection->want_io = IO_NONE;
  if(connection->is_tls)
  {
    ERR_clear_error();
    rv = SSL_read(connection->ssl, buf, length);
    if(rv < 0) {
      int err = SSL_get_error(connection->ssl, rv);
      if(err == SSL_ERROR_WANT_WRITE || err == SSL_ERROR_WANT_READ) {
        connection->want_io |= (err == SSL_ERROR_WANT_READ ?
                               WANT_READ : WANT_WRITE);
        rv = SPDYLAY_ERR_WOULDBLOCK;
      } else {
        rv = SPDYLAY_ERR_CALLBACK_FAILURE;
      }
    } else if(rv == 0) {
      rv = SPDYLAY_ERR_EOF;
    }
  }
  else
  {
    rv = read(connection->fd, 
            buf,
            length);
            
    if (rv < 0)
    {
      switch(errno)
      {				
        case EAGAIN:
  #if EAGAIN != EWOULDBLOCK
        case EWOULDBLOCK:
  #endif
          connection->want_io |= WANT_READ;
          rv = SPDYLAY_ERR_WOULDBLOCK;
          break;
          
        default:
          rv = SPDYLAY_ERR_CALLBACK_FAILURE;
      }
    }
    else if(rv == 0)
      rv = SPDYLAY_ERR_EOF;
  }
  
  if(rv > 0)
    UPDATE_STAT(glob_stat.spdy_bytes_received, rv);
  
  return rv;
}


static void
spdy_cb_before_ctrl_send(spdylay_session *session,
                    spdylay_frame_type type,
                    spdylay_frame *frame,
                    void *user_data)
{
  (void)user_data;
  
  int32_t stream_id;
  struct Proxy *proxy;
  
  switch(type) {
    case SPDYLAY_SYN_STREAM:
      stream_id = frame->syn_stream.stream_id;
      proxy = spdylay_session_get_stream_user_data(session, stream_id);
      proxy->stream_id = stream_id;
      ++glob_opt.streams_opened;
      ++proxy->spdy_connection->streams_opened;
      PRINT_INFO2("opening stream: str open %i; %s", glob_opt.streams_opened, proxy->url);
      break;
    case SPDYLAY_RST_STREAM:
      //try to ignore duplicate RST_STREAMs
      //TODO this will ignore RST_STREAMs also for bogus data
      glob_opt.ignore_rst_stream = NULL==spdylay_session_get_stream_user_data(session, frame->rst_stream.stream_id);
      PRINT_INFO2("sending RST_STREAM for %i; ignore %i; status %i",
        frame->rst_stream.stream_id,
        glob_opt.ignore_rst_stream,
        frame->rst_stream.status_code);
    break;
    default:
      break;
  }
}


void
spdy_cb_on_ctrl_recv(spdylay_session *session,
                    spdylay_frame_type type,
                    spdylay_frame *frame,
                    void *user_data)
{
  (void)user_data;
  
  char **nv;
  int32_t stream_id;
  struct Proxy * proxy;

  switch(type) {
    case SPDYLAY_SYN_REPLY:
      nv = frame->syn_reply.nv;
      stream_id = frame->syn_reply.stream_id;
    break;
    case SPDYLAY_RST_STREAM:
      stream_id = frame->rst_stream.stream_id;
    break;
    case SPDYLAY_HEADERS:
      nv = frame->headers.nv;
      stream_id = frame->headers.stream_id;
    break;
    default:
      return;
    break;
  }

  proxy = spdylay_session_get_stream_user_data(session, stream_id);
  if(NULL == proxy)
  {
    PRINT_INFO2("received frame type %i for unkonwn stream id %i", type, stream_id);
    return;
    //DIE("no proxy obj");
  }

  switch(type) {
    case SPDYLAY_SYN_REPLY:
      PRINT_INFO2("received headers for %s", proxy->url);
      http_create_response(proxy, nv);
    break;
    case SPDYLAY_RST_STREAM:
      PRINT_INFO2("received reset stream for %s", proxy->url);
      proxy->spdy_error = true;
    break;
    case SPDYLAY_HEADERS:
      PRINT_INFO2("received headers for %s", proxy->url);
      http_create_response(proxy, nv);
    break;
    default:
      return;
    break;
  }
  
  glob_opt.spdy_data_received = true;
}


/*
 * The implementation of spdylay_on_stream_close_callback type. We use
 * this function to know the response is fully received. Since we just
 * fetch 1 resource in this program, after reception of the response,
 * we submit GOAWAY and close the session.
 */
static void
spdy_cb_on_stream_close(spdylay_session *session,
                       int32_t stream_id,
                       spdylay_status_code status_code,
                       void *user_data)
{
  (void)status_code;
  (void)user_data;
  
  struct Proxy * proxy = spdylay_session_get_stream_user_data(session, stream_id);
  
  assert(NULL != proxy);
  
  --glob_opt.streams_opened;
  --proxy->spdy_connection->streams_opened;
  PRINT_INFO2("closing stream: str opened %i; remove proxy %i", glob_opt.streams_opened, proxy->id);
   
  DLL_remove(proxy->spdy_connection->proxies_head, proxy->spdy_connection->proxies_tail, proxy); 
  if(proxy->http_active)
  {
    proxy->spdy_active = false;
  }
  else
  {
    free_proxy(proxy);
  }
}


/*
 * The implementation of spdylay_on_data_chunk_recv_callback type. We
 * use this function to print the received response body.
 */
static void
spdy_cb_on_data_chunk_recv(spdylay_session *session,
                          uint8_t flags,
                          int32_t stream_id,
                          const uint8_t *data,
                          size_t len,
                          void *user_data)
{
  (void)flags;
  (void)user_data;
  
  struct Proxy *proxy;
  proxy = spdylay_session_get_stream_user_data(session, stream_id);
  
  if(NULL == proxy)
  {
    PRINT_INFO("proxy in spdy_cb_on_data_chunk_recv is NULL)");
    return;
	}
  
  if(!copy_buffer(data, len, &proxy->http_body, &proxy->http_body_size))
  {
    //TODO handle it better?
    PRINT_INFO("not enough memory (malloc/realloc returned NULL)");
    return;
  }
  /*
	if(NULL == proxy->http_body)
		proxy->http_body = au_malloc(len);
  else
		proxy->http_body = realloc(proxy->http_body, proxy->http_body_size + len);
	if(NULL == proxy->http_body)
	{
		PRINT_INFO("not enough memory (realloc returned NULL)");
		return ;
	}

	memcpy(proxy->http_body + proxy->http_body_size, data, len);
	proxy->http_body_size += len;
  */
  PRINT_INFO2("received data for %s; %zu bytes", proxy->url, len);
  glob_opt.spdy_data_received = true;
}


static void
spdy_cb_on_data_recv(spdylay_session *session,
		                 uint8_t flags,
                     int32_t stream_id,
                     int32_t length,
                     void *user_data)
{
  (void)length;
  (void)user_data;
  
	if(flags & SPDYLAY_DATA_FLAG_FIN)
	{
    struct Proxy *proxy;
    proxy = spdylay_session_get_stream_user_data(session, stream_id);
    proxy->done = true;
    PRINT_INFO2("last data frame received for %s", proxy->url);
	}
}


/*
 * Setup callback functions. Spdylay API offers many callback
 * functions, but most of them are optional. The send_callback is
 * always required. Since we use spdylay_session_recv(), the
 * recv_callback is also required.
 */
static void
spdy_setup_spdylay_callbacks(spdylay_session_callbacks *callbacks)
{
  memset(callbacks, 0, sizeof(spdylay_session_callbacks));
  callbacks->send_callback = spdy_cb_send;
  callbacks->recv_callback = spdy_cb_recv;
  callbacks->before_ctrl_send_callback = spdy_cb_before_ctrl_send;
  callbacks->on_ctrl_recv_callback = spdy_cb_on_ctrl_recv;
  callbacks->on_stream_close_callback = spdy_cb_on_stream_close;
  callbacks->on_data_chunk_recv_callback = spdy_cb_on_data_chunk_recv;
  callbacks->on_data_recv_callback = spdy_cb_on_data_recv;
}


/*
 * Callback function for SSL/TLS NPN. Since this program only supports
 * SPDY protocol, if server does not offer SPDY protocol the Spdylay
 * library supports, we terminate program.
 */
static int
spdy_cb_ssl_select_next_proto(SSL* ssl,
                                unsigned char **out,
                                unsigned char *outlen,
                                const unsigned char *in,
                                unsigned int inlen,
                                void *arg)
{
  (void)ssl;
  
  int rv;
  uint16_t *spdy_proto_version;
  
  /* spdylay_select_next_protocol() selects SPDY protocol version the
     Spdylay library supports. */
  rv = spdylay_select_next_protocol(out, outlen, in, inlen);
  if(rv <= 0) {
    PRINT_INFO("Server did not advertise spdy/2 or spdy/3 protocol.");
    return rv;
  }
  spdy_proto_version = (uint16_t*)arg;
  *spdy_proto_version = rv;
  return SSL_TLSEXT_ERR_OK;
}


/*
 * Setup SSL context. We pass |spdy_proto_version| to get negotiated
 * SPDY protocol version in NPN callback.
 */
void
spdy_ssl_init_ssl_ctx(SSL_CTX *ssl_ctx,
                      uint16_t *spdy_proto_version)
{
  /* Disable SSLv2 and enable all workarounds for buggy servers */
  SSL_CTX_set_options(ssl_ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2 | SSL_OP_NO_COMPRESSION);
  SSL_CTX_set_mode(ssl_ctx, SSL_MODE_AUTO_RETRY);
  SSL_CTX_set_mode(ssl_ctx, SSL_MODE_RELEASE_BUFFERS);
  /* Set NPN callback */
  SSL_CTX_set_next_proto_select_cb(ssl_ctx, spdy_cb_ssl_select_next_proto,
                                   spdy_proto_version);
}


static int
spdy_ssl_handshake(SSL *ssl,
                   int fd)
{
  int rv;
  
  if(SSL_set_fd(ssl, fd) == 0)
    spdy_dief("SSL_set_fd", ERR_error_string(ERR_get_error(), NULL));

  ERR_clear_error();
  rv = SSL_connect(ssl);
  if(rv <= 0)
    PRINT_INFO2("SSL_connect %s", ERR_error_string(ERR_get_error(), NULL));
  
  return rv;
}


/*
 * Connects to the host |host| and port |port|.  This function returns
 * the file descriptor of the client socket.
 */
static int
spdy_socket_connect_to(const char *host,
                       uint16_t port)
{
  struct addrinfo hints;
  int fd = -1;
  int rv;
  char service[NI_MAXSERV];
  struct addrinfo *res, *rp;
  
  //TODO checks
  snprintf(service, sizeof(service), "%u", port);
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  rv = getaddrinfo(host, service, &hints, &res);
  if(rv != 0)
  {
	  printf("%s\n",host);
    spdy_dief("getaddrinfo", gai_strerror(rv));
  }
  for(rp = res; rp; rp = rp->ai_next)
  {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if(fd == -1)
      continue;
    while((rv = connect(fd, rp->ai_addr, rp->ai_addrlen)) == -1 &&
          errno == EINTR);
    if(rv == 0)
      break;
    close(fd);
    fd = -1;
  }
  freeaddrinfo(res);
  
  return fd;
}


static void
spdy_socket_make_non_block(int fd)
{
  int flags;
  int rv;
  
  while((flags = fcntl(fd, F_GETFL, 0)) == -1 && errno == EINTR);
  
  if(flags == -1)
    spdy_dief("fcntl", strerror(errno));
    
  while((rv = fcntl(fd, F_SETFL, flags | O_NONBLOCK)) == -1 && errno == EINTR);
 
  if(rv == -1)
    spdy_dief("fcntl", strerror(errno));
}


/*
 * Setting TCP_NODELAY is not mandatory for the SPDY protocol.
 */
static void
spdy_socket_set_tcp_nodelay(int fd)
{
  int val = 1;
  int rv;
  
  rv = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, (socklen_t)sizeof(val));
  if(rv == -1)
    spdy_dief("setsockopt", strerror(errno));
}

/*
 * Update |pollfd| based on the state of |connection|.
 */
 /*
void
spdy_ctl_poll(struct pollfd *pollfd,
              struct SPDY_Connection *connection)
{
  pollfd->events = 0;
  if(spdylay_session_want_read(connection->session) ||
     connection->want_io & WANT_READ)
  {
    pollfd->events |= POLLIN;
  }
  if(spdylay_session_want_write(connection->session) ||
     connection->want_io & WANT_WRITE)
  {
    pollfd->events |= POLLOUT;
  }
}*/


/*
 * Update |selectfd| based on the state of |connection|.
 */
bool
spdy_ctl_select(fd_set * read_fd_set,
                fd_set * write_fd_set, 
                fd_set * except_fd_set,
                struct SPDY_Connection *connection)
{
  (void)except_fd_set;
  
  bool ret = false;
  
  if(spdylay_session_want_read(connection->session) ||
     connection->want_io & WANT_READ)
  {
    FD_SET(connection->fd, read_fd_set);
    ret = true;
  }
  if(spdylay_session_want_write(connection->session) ||
     connection->want_io & WANT_WRITE)
  {
    FD_SET(connection->fd, write_fd_set);
    ret = true;
  }
  
  return ret;
}


/*
 * Performs the network I/O.
 */
int
spdy_exec_io(struct SPDY_Connection *connection)
{
  int rv;
  
  rv = spdylay_session_recv(connection->session);
  if(rv != 0)
  {
    PRINT_INFO2("spdylay_session_recv %i", rv);
    return rv;
  }
  rv = spdylay_session_send(connection->session);
  if(rv != 0)
    PRINT_INFO2("spdylay_session_send %i", rv);
    
  return rv;
}


/*
 * Fetches the resource denoted by |uri|.
 */
struct SPDY_Connection *
spdy_connect(const struct URI *uri,
             uint16_t port,
             bool is_tls)
{
  spdylay_session_callbacks callbacks;
  int fd;
  SSL *ssl=NULL;
  struct SPDY_Connection * connection = NULL;
  int rv;

  spdy_setup_spdylay_callbacks(&callbacks);

  /* Establish connection and setup SSL */
  PRINT_INFO2("connecting to %s:%i", uri->host, port);
  fd = spdy_socket_connect_to(uri->host, port);
  if(fd == -1)
  {
    PRINT_INFO("Could not open file descriptor");
    return NULL;
  }
  
  if(is_tls)
  {
    ssl = SSL_new(glob_opt.ssl_ctx);
    if(ssl == NULL) {
      spdy_dief("SSL_new", ERR_error_string(ERR_get_error(), NULL));
    }
    
    //TODO non-blocking
    /* To simplify the program, we perform SSL/TLS handshake in blocking
       I/O. */
    glob_opt.spdy_proto_version = 0;
    rv = spdy_ssl_handshake(ssl, fd);
    if(rv <= 0 || (glob_opt.spdy_proto_version != 3 && glob_opt.spdy_proto_version != 2))
    {
      PRINT_INFO("Closing SSL");
      //no spdy on the other side
      goto free_and_fail;
    }
  }
  else
  {
    glob_opt.spdy_proto_version = 3;
  }

  if(NULL == (connection = au_malloc(sizeof(struct SPDY_Connection))))
    goto free_and_fail;
  
  connection->is_tls = is_tls;
  connection->ssl = ssl;
  connection->want_io = IO_NONE;
  if(NULL == (connection->host = strdup(uri->host)))
    goto free_and_fail;

  /* Here make file descriptor non-block */
  spdy_socket_make_non_block(fd);
  spdy_socket_set_tcp_nodelay(fd);

  PRINT_INFO2("[INFO] SPDY protocol version = %d\n", glob_opt.spdy_proto_version);
  rv = spdylay_session_client_new(&(connection->session), glob_opt.spdy_proto_version,
                                  &callbacks, connection);
  if(rv != 0) {
    spdy_diec("spdylay_session_client_new", rv);
  }
  
  connection->fd = fd;

	return connection;
  
	//for GOTO
	free_and_fail:
  if(NULL != connection)
  {
    free(connection->host);
    free(connection);
  }
  
  if(is_tls)
    SSL_shutdown(ssl);
    
  close(fd);
  
  if(is_tls)
    SSL_free(ssl);
  
  return NULL;
}


void
spdy_free_connection(struct SPDY_Connection * connection)
{
  struct Proxy *proxy;
  struct Proxy *proxy_next;
  
  if(NULL != connection)
  {
    for(proxy = connection->proxies_head; NULL != proxy; proxy=proxy_next)
    {
      proxy_next = proxy->next;
      DLL_remove(connection->proxies_head, connection->proxies_tail, proxy);
      proxy->spdy_active = false;
      proxy->spdy_error = true;
      PRINT_INFO2("spdy_free_connection for id %i", proxy->id);
      if(!proxy->http_active)
      {
        free_proxy(proxy);
      }
    }
    spdylay_session_del(connection->session);
    SSL_free(connection->ssl);
    free(connection->host);
    free(connection);
    //connection->session = NULL;
  }
}


int
spdy_request(const char **nv,
             struct Proxy *proxy,
             bool with_body)
{
  int ret;
  uint16_t port;
  struct SPDY_Connection *connection;
  spdylay_data_provider post_data;
  
  if(glob_opt.only_proxy)
  {
    connection = glob_opt.spdy_connection;
  }
  else
  {
    connection = glob_opt.spdy_connections_head;
    while(NULL != connection)
    {
      if(0 == strcasecmp(proxy->uri->host, connection->host))
        break;
      connection = connection->next;
    }
  
    if(NULL == connection)
    {
      //connect to host
      port = proxy->uri->port;
      if(0 == port) port = 443;
      connection = spdy_connect(proxy->uri, port, true);
      if(NULL != connection)
      {
        DLL_insert(glob_opt.spdy_connections_head, glob_opt.spdy_connections_tail, connection);
        glob_opt.total_spdy_connections++;
      }
      else
        connection = glob_opt.spdy_connection;
    }
  }
  
  if(NULL == connection)
  {
    PRINT_INFO("there is no proxy!");
    return -1;
  }
  
  proxy->spdy_connection = connection;
  if(with_body)
  {
    post_data.source.ptr = proxy;
    post_data.read_callback = &spdy_cb_data_source_read;
    ret = spdylay_submit_request(connection->session, 0, nv, &post_data, proxy);
  }
  else
    ret = spdylay_submit_request(connection->session, 0, nv, NULL, proxy);
  
  if(ret != 0) {
    spdy_diec("spdylay_spdy_submit_request", ret);
  }
  PRINT_INFO2("adding proxy %i", proxy->id);
  if(NULL != connection->proxies_head)
    PRINT_INFO2("before proxy %i", connection->proxies_head->id);
  DLL_insert(connection->proxies_head, connection->proxies_tail, proxy);
  
  return ret;
}

/*
void
spdy_get_pollfdset(struct pollfd fds[],
                   struct SPDY_Connection *connections[],
                   unsigned int max_size,
                   nfds_t *real_size)
{
  struct SPDY_Connection *connection;
  struct Proxy *proxy;
  
  *real_size = 0;
  if(max_size<1)
    return;
    
  if(NULL != glob_opt.spdy_connection)
  {
    spdy_ctl_poll(&(fds[*real_size]), glob_opt.spdy_connection);
    if(!fds[*real_size].events)
    {
      //PRINT_INFO("TODO drop connection");
      glob_opt.streams_opened -= glob_opt.spdy_connection->streams_opened;
      
      for(proxy = glob_opt.spdy_connection->proxies_head; NULL != proxy; proxy=proxy->next)
      {
        abort();
        DLL_remove(glob_opt.spdy_connection->proxies_head, glob_opt.spdy_connection->proxies_tail, proxy);
        proxy->spdy_active = false;
      }
      spdy_free_connection(glob_opt.spdy_connection);
      glob_opt.spdy_connection = NULL;
    }
    else
    {
      fds[*real_size].fd = glob_opt.spdy_connection->fd;
      connections[*real_size] = glob_opt.spdy_connection;
      ++(*real_size);
    }
  }
  
  connection = glob_opt.spdy_connections_head;
  
  while(NULL != connection && *real_size < max_size)
  {
    assert(!glob_opt.only_proxy);
    spdy_ctl_poll(&(fds[*real_size]), connection);
    if(!fds[*real_size].events)
    {
      //PRINT_INFO("TODO drop connection");
      glob_opt.streams_opened -= connection->streams_opened;
      DLL_remove(glob_opt.spdy_connections_head, glob_opt.spdy_connections_tail, connection);
      glob_opt.total_spdy_connections--;
      
      for(proxy = connection->proxies_head; NULL != proxy; proxy=proxy->next)
      {
        abort();
        DLL_remove(connection->proxies_head, connection->proxies_tail, proxy);
        proxy->spdy_active = false;
      }
      spdy_free_connection(connection);
    }
    else
    {
      fds[*real_size].fd = connection->fd;
      connections[*real_size] = connection;
      ++(*real_size);
    }
    connection = connection->next;
  }
  
  //, "TODO max num of conn reached; close something"
  assert(NULL == connection);
}
*/

int
spdy_get_selectfdset(fd_set * read_fd_set,
                      fd_set * write_fd_set, 
                      fd_set * except_fd_set,
                      struct SPDY_Connection *connections[],
                      unsigned int max_size,
                      nfds_t *real_size)
{
  struct SPDY_Connection *connection;
  struct SPDY_Connection *next_connection;
  bool ret;
  int maxfd = 0;
  
  *real_size = 0;
  if(max_size<1)
    return 0;
    
  if(NULL != glob_opt.spdy_connection)
  {
    ret = spdy_ctl_select(read_fd_set,
				 write_fd_set, 
				 except_fd_set, glob_opt.spdy_connection);
    if(!ret)
    {
      glob_opt.streams_opened -= glob_opt.spdy_connection->streams_opened;
      
      PRINT_INFO("spdy_free_connection in spdy_get_selectfdset");
      spdy_free_connection(glob_opt.spdy_connection);
      glob_opt.spdy_connection = NULL;
    }
    else
    {
      connections[*real_size] = glob_opt.spdy_connection;
      ++(*real_size);
      if(maxfd < glob_opt.spdy_connection->fd) maxfd = glob_opt.spdy_connection->fd;
    }
  }
  
  connection = glob_opt.spdy_connections_head;
  
  while(NULL != connection && *real_size < max_size)
  {
    assert(!glob_opt.only_proxy);
    ret = spdy_ctl_select(read_fd_set,
				 write_fd_set, 
				 except_fd_set, connection);
         
    next_connection = connection->next;
    if(!ret)
    {
      glob_opt.streams_opened -= connection->streams_opened;
      DLL_remove(glob_opt.spdy_connections_head, glob_opt.spdy_connections_tail, connection);
      glob_opt.total_spdy_connections--;
      
      PRINT_INFO("spdy_free_connection in spdy_get_selectfdset");
      spdy_free_connection(connection);
    }
    else
    {
      connections[*real_size] = connection;
      ++(*real_size);
      if(maxfd < connection->fd) maxfd = connection->fd;
    }
    connection = next_connection;
  }
  
  //, "TODO max num of conn reached; close something"
  assert(NULL == connection);
  
  return maxfd;
}

/*
void
spdy_run(struct pollfd fds[],
         struct SPDY_Connection *connections[],
         int size)
{
  int i;
  int ret;
  struct Proxy *proxy;
  
  for(i=0; i<size; ++i)
  {
    //  PRINT_INFO2("exec about to be called for %s", connections[i]->host);
    if(fds[i].revents & (POLLIN | POLLOUT))
    {
      ret = spdy_exec_io(connections[i]);
      //PRINT_INFO2("%i",ret);
      //if((spdy_pollfds[i].revents & POLLHUP) || (spdy_pollfds[0].revents & POLLERR))
      //  PRINT_INFO("SPDY SPDY_Connection error");
      
      //TODO POLLRDHUP
      // always close on ret != 0?
        
      if(0 != ret)
      {
        glob_opt.streams_opened -= connections[i]->streams_opened;
        if(connections[i] == glob_opt.spdy_connection)
        {
          glob_opt.spdy_connection = NULL;
        }
        else
        {
          DLL_remove(glob_opt.spdy_connections_head, glob_opt.spdy_connections_tail, connections[i]);
          glob_opt.total_spdy_connections--;
        }
        for(proxy = connections[i]->proxies_head; NULL != proxy; proxy=proxy->next)
        {
        abort();
          DLL_remove(connections[i]->proxies_head, connections[i]->proxies_tail, proxy);
          proxy->spdy_active = false;
          proxy->spdy_error = true;
          PRINT_INFO2("spdy_free_connection for id %i", proxy->id);
        }
        PRINT_INFO("spdy_free_connection in loop");
        spdy_free_connection(connections[i]);
      }
    }
    else
      PRINT_INFO("not called");
  }
}
*/

void
spdy_run_select(fd_set * read_fd_set,
                fd_set * write_fd_set, 
                fd_set * except_fd_set,
                struct SPDY_Connection *connections[],
                int size)
{
  int i;
  int ret;
  
  for(i=0; i<size; ++i)
  {
    //  PRINT_INFO2("exec about to be called for %s", connections[i]->host);
    if(FD_ISSET(connections[i]->fd, read_fd_set) || FD_ISSET(connections[i]->fd, write_fd_set) || FD_ISSET(connections[i]->fd, except_fd_set))
    {
      //raise(SIGINT);
      ret = spdy_exec_io(connections[i]);
        
      if(0 != ret)
      {
        glob_opt.streams_opened -= connections[i]->streams_opened;
        if(connections[i] == glob_opt.spdy_connection)
        {
          glob_opt.spdy_connection = NULL;
        }
        else
        {
          DLL_remove(glob_opt.spdy_connections_head, glob_opt.spdy_connections_tail, connections[i]);
          glob_opt.total_spdy_connections--;
        }
        PRINT_INFO("in spdy_run_select");
        spdy_free_connection(connections[i]);
      }
    }
    else
    {
      PRINT_INFO("not called");
      //PRINT_INFO2("connection->want_io %i",connections[i]->want_io);
      //PRINT_INFO2("read %i",spdylay_session_want_read(connections[i]->session));
      //PRINT_INFO2("write %i",spdylay_session_want_write(connections[i]->session));
      //raise(SIGINT);
    }
  }
}
