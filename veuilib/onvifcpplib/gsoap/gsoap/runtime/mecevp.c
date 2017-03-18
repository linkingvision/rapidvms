/*
	mecevp.c

	gSOAP interface for streaming message encryption and decryption

gSOAP XML Web services tools
Copyright (C) 2000-2012, Robert van Engelen, Genivia Inc., All Rights Reserved.
This part of the software is released under one of the following licenses:
GPL, the gSOAP public license, or Genivia's license for commercial use.
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2012, Robert van Engelen, Genivia, Inc., All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

/**

@page mecevp The mecevp streaming message encryption and decryption engine

The gSOAP mecevp engine encrypts and decrypts messages using the EVP interface
of OpenSSL. It supports envelope encryption/decryption with public and private
RSA keys and symmetric encryption with shared secret keys. Streaming and
buffered message encryption modes are supported.

An encryption and decryption algorithm and mode is selected with one of the
following:

- @ref SOAP_MEC_ENV_ENC_DES_CBC	envelope encryption with triple DES CBC
- @ref SOAP_MEC_ENV_ENC_DES_CBC	envelope decryption with triple DES CBC
- @ref SOAP_MEC_ENC_DES_CBC		symmetric encryption with triple DES CBC
- @ref SOAP_MEC_ENC_DES_CBC		symmetric decryption with triple DES CBC
- @ref SOAP_MEC_ENV_ENC_AES256_CBC	envelope encryption with triple DES CBC
- @ref SOAP_MEC_ENV_ENC_AES256_CBC	envelope decryption with triple DES CBC
- @ref SOAP_MEC_ENC_AES256_CBC		symmetric encryption with triple DES CBC
- @ref SOAP_MEC_ENC_AES256_CBC		symmetric decryption with triple DES CBC

where AES256 can be replaced with any one of AES128, AES192, AES512.

Algorithm options:

- @ref SOAP_MEC_STORE		buffer all output in memory
- @ref SOAP_MEC_OAEP		use OAEP padding

The mecevp engine wraps the EVP API with four new functions:

- @ref soap_mec_init	to initialize the engine
- @ref soap_mec_update	to encrypt/decrypt a message part
- @ref soap_mec_final	to finalize encryption/decryption
- @ref soap_mec_cleanup	to deallocate the engine and buffers

All cipher data is written and read in base64 format.

A higher-level interface for message encryption/decryption in parts (such as
individual XML elements) is defined by two new functions:

- @ref soap_mec_begin	to begin a streaming sequence of encryptions/decryptions
- @ref soap_mec_start	to start encryption/decryption of a message part
- @ref soap_mec_stop	to stop encryption/decryption of a message part
- @ref soap_mec_end	to end the sequence and deallocate the engine buffers

Compile all source codes with -DWITH_OPENSSL and link with ssl and crypto
libraries.

Here is an example to encrypt a message while streaming it to the output. The
example uses the public key of the recipient/reader of the message. The
recipient/reader uses its private key to decrypt. Envelope encryption is used
with SOAP_MEC_ENV_ENC_DES_CBC, which means an ephemeral secret key is generated
and encrypted with the public key. This encrypted secret key should be
communicated to the recipient/reader with the message to decrypt:

@code
    #include "mecevp.h"
    soap_mec_data mec;
    ns__Object object;
    int alg = SOAP_MEC_ENV_ENC_DES_CBC;
    FILE *fd = fopen("key.pem", "r");
    EVP_PKEY *pubk;
    unsigned char *key;
    int keylen;
    if (...) // key file contains public key?
      pubk = PEM_read_PUBKEY(fd, NULL, NULL, NULL);
    else // key file contains certificate
    { X509 *cert = PEM_read_X509(fd, NULL, NULL, NULL);
      pubk = X509_get_pubkey(cert);
      X509_free(cert);
    }
    fclose(fd);
    key = soap_malloc(soap, soap_mec_size(alg, pubk));
    if (soap_begin_send(soap)
     || soap_mec_begin(soap, &mec, alg, pubk, key, &keylen)
     || soap_mec_start(soap, NULL)
     || soap_out_ns__Object(soap, "ns:Object", 0, &object, NULL)
     || soap_mec_stop(soap)
     || soap_mec_end(soap, &mec)
     || soap_end_send(soap))
    { soap_mec_cleanup(soap, &mec); // clean up when error
      soap_print_fault(soap, stderr);
    }
    EVP_PKEY_free(pubk);
@endcode

The decryption by the recipient/reader requires the ephemeral encrypted secret
key generated by soap_mec_begin by the sender (as set above) to decrypt the
message using envelope decryption with SOAP_MEC_ENV_DEC_DES_CBC.

@code
    #include "mecevp.h"
    soap_mec_data mec;
    ns__Object object;
    int alg = SOAP_MEC_ENV_DEC_DES_CBC;
    FILE *fd = fopen("key.pem", "r");
    EVP_PKEY *privk = PEM_read_PrivateKey(fd, NULL, NULL, "password");
    unsigned char *key;
    int keylen;
    fclose(fd);
    key = ... // value set as above by sender
    keylen = ... // value set as above by sender
    if (soap_begin_recv(soap)
     || soap_mec_begin(soap, &mec, alg, privk, key, &keylen)
     || soap_mec_start(soap)
     || soap_in_ns__Object(soap, "ns:Object", &object, NULL) == NULL
     || soap_mec_stop(soap)
     || soap_mec_end(soap, &mec)
     || soap_end_recv(soap))
    { soap_mec_cleanup(soap, &mec); // clean up when error
      soap_print_fault(soap, stderr);
    }
    EVP_PKEY_free(privk);
@endcode 

Note that the encrypted secret key can be send in the clear or stored openly,
since only the recipient/reader will be able to decode it for use in message
decryption.

Symmetric encryption and decryption can be used if both parties can safely
share a secret symmetric key that no other party has access to. We use
SOAP_MEC_ENC_DES_CBC for encryption and SOAP_MEC_DEC_DES_CBC for decryption
using a 160-bit triple DES key.

Here is an example to encrypt a message using a shared secret key while
streaming it to the output.

@code
    #include "mecevp.h"
    soap_mec_data mec;
    ns__Object object;
    int alg = SOAP_MEC_ENC_DES_CBC;
    unsigned char key[20] = { ... }; // shared secret triple DES key
    int keylen = 20;
    if (soap_begin_send(soap)
     || soap_mec_begin(soap, &mec, alg, NULL, key, &keylen)
     || soap_mec_start(soap, NULL)
     || soap_out_ns__Object(soap, "ns:Object", 0, &object, NULL)
     || soap_mec_stop(soap)
     || soap_mec_end(soap, &mec)
     || soap_end_send(soap))
    { soap_mec_cleanup(soap, &mec); // clean up when error
      soap_print_fault(soap, stderr);
    }
@endcode

The decryption by the recipient/reader requires the same shared secret key to
decrypt the message using envelope decryption with SOAP_MEC_DEC_DES_CBC. This
key is secret and unencrypted, so it should never be shared with any other
party besides the sender/writer and recipient/reader.

@code
    #include "mecevp.h"
    soap_mec_data mec;
    ns__Object object;
    int alg = SOAP_MEC_DEC_DES_CBC;
    unsigned char key[20] = { ... }; // shared secret triple DES key
    int keylen = 20;
    if (soap_begin_recv(soap)
     || soap_mec_begin(soap, &mec, alg, NULL, key, &keylen)
     || soap_mec_start(soap)
     || soap_in_ns__Object(soap, "ns:Object", &object, NULL) == NULL
     || soap_mec_stop(soap)
     || soap_mec_end(soap, &mec)
     || soap_end_recv(soap))
    { soap_mec_cleanup(soap, &mec); // clean up when error
      soap_print_fault(soap, stderr);
    }
@endcode

Note: the mecevp engine uses callbacks of the gSOAP engine that were introduced
in version 2.8.1. Earlier gSOAP version releases are not compatible with the
mecevp plugin and engine.

*/

#include "mecevp.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
 *
 * Static local functions used
 *
\******************************************************************************/

static int soap_mec_upd(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final);
static int soap_mec_upd_enc(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final);
static int soap_mec_upd_dec(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final);

static int soap_mec_check(struct soap *soap, struct soap_mec_data *data, int err, const char *msg);

static void soap_mec_put_base64(struct soap *soap, struct soap_mec_data *data, const unsigned char *s, int n);
static void soap_mec_end_base64(struct soap *soap, struct soap_mec_data *data);
static int soap_mec_get_base64(struct soap *soap, struct soap_mec_data *data, char *t, size_t *l, const char *s, size_t n, const char **r, size_t *k);

static int soap_mec_filtersend(struct soap *soap, const char **s, size_t *n);
static int soap_mec_filterrecv(struct soap *soap, char *buf, size_t *len, size_t maxlen);

/******************************************************************************\
 *
 * soap_mec API functions
 *
\******************************************************************************/

/**
@fn int soap_mec_init(struct soap *soap, struct soap_mec_data *data, int alg, SOAP_MEC_KEY_TYPE *pkey, unsigned char *key, int *keylen)
@brief Initialize mecevp engine state and create context for
encryption/decryption algorithm using a private/public key or symmetric secret
key.
@param soap context
@param[in,out] data mecevp engine context
@param[in] alg encryption/decryption algorithm
@param[in] pkey public/private key or NULL
@param[in,out] key secret key or encrypted ephemeral secret key set with envelope encryption, or NULL
@param[in,out] keylen secret key length
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_mec_init(struct soap *soap, struct soap_mec_data *data, int alg, SOAP_MEC_KEY_TYPE *pkey, unsigned char *key, int *keylen)
{ int ok = 1;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_mec_init()\n"));
  soap_ssl_init();
  data->ctx = (EVP_CIPHER_CTX*)SOAP_MALLOC(soap, sizeof(EVP_CIPHER_CTX));
  if (!data->ctx)
    return soap->error = SOAP_EOM;
  EVP_CIPHER_CTX_init(data->ctx);
  data->alg = alg;
  data->state = SOAP_MEC_STATE_NONE;
  if (alg & SOAP_MEC_DES_CBC)
    data->type = EVP_des_ede3_cbc(); /* triple DES CBC */
  else if (alg & SOAP_MEC_AES128_CBC)
    data->type = EVP_get_cipherbyname("AES128");
  else if (alg & SOAP_MEC_AES192_CBC)
    data->type = EVP_get_cipherbyname("AES192");
  else if (alg & SOAP_MEC_AES256_CBC)
    data->type = EVP_get_cipherbyname("AES256");
  else if (alg & SOAP_MEC_AES512_CBC)
    data->type = EVP_get_cipherbyname("AES512");
  else
    data->type = EVP_enc_null();
  data->buf = NULL;
  data->rest = NULL;
  data->restlen = 0;
  if (alg & SOAP_MEC_ENC)
  { if (!data->type)
      return soap_mec_check(soap, data, 0, "soap_mec_init() failed: cannot load cipher");
    EVP_EncryptInit_ex(data->ctx, data->type, NULL, NULL, NULL);
  }
  if (alg & SOAP_MEC_OAEP)
    EVP_CIPHER_CTX_set_padding(data->ctx, RSA_PKCS1_OAEP_PADDING);
  else
    EVP_CIPHER_CTX_set_padding(data->ctx, RSA_PKCS1_PADDING);
  switch (alg & SOAP_MEC_MASK)
  { case SOAP_MEC_ENV_ENC_AES128_CBC:
    case SOAP_MEC_ENV_ENC_AES192_CBC:
    case SOAP_MEC_ENV_ENC_AES256_CBC:
    case SOAP_MEC_ENV_ENC_AES512_CBC:
    case SOAP_MEC_ENV_ENC_DES_CBC:
      ok = EVP_CIPHER_CTX_rand_key(data->ctx, data->ekey);
      /* generate ephemeral secret key */
#if (OPENSSL_VERSION_NUMBER >= 0x01000000L)
      *keylen = EVP_PKEY_encrypt_old(key, data->ekey, EVP_CIPHER_CTX_key_length(data->ctx), pkey);
#else
      *keylen = EVP_PKEY_encrypt(key, data->ekey, EVP_CIPHER_CTX_key_length(data->ctx), pkey);
#endif
      key = data->ekey;
      /* fall through to next arm */
    case SOAP_MEC_ENC_DES_CBC:
    case SOAP_MEC_ENC_AES128_CBC:
    case SOAP_MEC_ENC_AES192_CBC:
    case SOAP_MEC_ENC_AES256_CBC:
    case SOAP_MEC_ENC_AES512_CBC:
      data->bufidx = 0;
      data->buflen = 1024; /* > iv in base64 must fit */
      data->buf = (char*)SOAP_MALLOC(soap, data->buflen);
      data->key = key;
      break;
    case SOAP_MEC_ENV_DEC_AES128_CBC:
    case SOAP_MEC_ENV_DEC_AES192_CBC:
    case SOAP_MEC_ENV_DEC_AES256_CBC:
    case SOAP_MEC_ENV_DEC_AES512_CBC:
    case SOAP_MEC_ENV_DEC_DES_CBC:
    case SOAP_MEC_DEC_DES_CBC:
    case SOAP_MEC_DEC_AES128_CBC:
    case SOAP_MEC_DEC_AES192_CBC:
    case SOAP_MEC_DEC_AES256_CBC:
    case SOAP_MEC_DEC_AES512_CBC:
      data->pkey = pkey;
      data->key = key;
      data->keylen = *keylen;
      break;
    default:
      return soap_set_receiver_error(soap, "Unsupported encryption algorithm", NULL, SOAP_SSL_ERROR);
  }
  return soap_mec_check(soap, data, ok, "soap_mec_init() failed");
}

/**
@fn int soap_mec_update(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n)
@brief Update mecevp engine state: encrypts plain text (or raw data) or
decrypts cipher data in base64 format.
@param soap context
@param[in,out] data mecevp engine context
@param[in,out] s input data to convert, afterwards points to converted data (original content is unchanged)
@param[in,out] n size of input, afterwards size of output
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_mec_update(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n)
{ return soap_mec_upd(soap, data, s, n, 0);
}

/**
@fn int soap_mec_final(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n)
@brief Ends mecevp engine state: encrypt/decrypt remainder from buffers.
@param soap context
@param[in,out] data mecevp engine context
@param[out] s afterwards points to converted remaining data in streaming mode, or entire converted data in buffer mode (SOAP_MEC_STORE option)
@param[out] n afterwards size of remaining data
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_mec_final(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n)
{ DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_mec_final()\n"));
  *n = 0;
  if (!data->ctx)
    return SOAP_OK;
  if (soap_mec_upd(soap, data, s, n, 1))
    return soap->error;
  return SOAP_OK;
}

/**
@fn void soap_mec_cleanup(struct soap *soap, struct soap_mec_data *data)
@brief Clean up mecevp engine and deallocate cipher context and buffers.
@param soap context
@param[in,out] data mecevp engine context
@return SOAP_OK or SOAP_SSL_ERROR
*/
void
soap_mec_cleanup(struct soap *soap, struct soap_mec_data *data)
{ DBGLOG(TEST, SOAP_MESSAGE(fdebug, "soap_mec_cleanup()\n"));
  if (data->ctx)
  { EVP_CIPHER_CTX_cleanup(data->ctx);
    SOAP_FREE(soap, data->ctx);
    data->ctx = NULL;
  }
  if (data->buf)
  { SOAP_FREE(soap, data->buf);
    data->buf = NULL;
  }
  if (data->rest)
  { SOAP_FREE(soap, data->rest);
    data->rest = NULL;
  }
}

/**
@fn int soap_mec_begin(struct soap *soap, struct soap_mec_data *data, int alg, SOAP_MEC_KEY_TYPE *pkey, unsigned char *key, int *keylen)
@brief Initialize the mecevp engine data and begin encryption or decryption
message sequence using a private/public key or symmetric secret key.
@param soap context
@param[in,out] data mecevp engine context
@param[in] alg encryption/decryption algorithm
@param[in] pkey public/private key or NULL
@param[in,out] key secret key or encrypted ephemeral secret key set with envelope encryption, or NULL
@param[in,out] keylen secret key length
@return SOAP_OK or error code
*/
int
soap_mec_begin(struct soap *soap, struct soap_mec_data *data, int alg, SOAP_MEC_KEY_TYPE *pkey, unsigned char *key, int *keylen)
{ DBGLOG(TEST, SOAP_MESSAGE(fdebug, "MEC Begin alg=%x\n", alg));
  /* save and set the engine's 'data' field to pass data to the callbacks */
  soap->data[1] = (void*)data;
  data->ctx = NULL;
  data->type = EVP_enc_null();
  data->pkey = NULL;
  data->key = NULL;
  data->buf = NULL;
  /* save the mode flag */
  data->mode = soap->mode;
  if (alg & SOAP_MEC_ENC)
  { /* clear the IO flags and DOM flag */
    soap->mode &= ~(SOAP_IO | SOAP_IO_LENGTH | SOAP_ENC_ZLIB | SOAP_XML_DOM);
    /* clear the XML attribute store */
    soap_clr_attr(soap);
    /* load the local XML namespaces store */
    soap_set_local_namespaces(soap);
    if (soap->mode & SOAP_XML_CANONICAL)
      soap->ns = 0; /* for in c14n, we must have all xmlns bindings available */
  }
  else
  { /* save and override the callbacks */
    data->ffilterrecv = soap->ffilterrecv;
    soap->ffilterrecv = soap_mec_filterrecv;
  }
  /* init the soap_mec engine */
  return soap_mec_init(soap, data, alg, pkey, key, keylen);
}

/**
@fn int soap_mec_start_alg(struct soap *soap, int alg, const unsigned char *key)
@brief Start encryption or decryption of current message. If key is non-NULL,
use the symmetric triple DES key. Use soap_mec_start only after soap_mec_begin.
The soap_mec_start should be followed by a soap_mec_stop call.
@param soap context
@param[in] alg algorithm
@param[in] key secret triple DES key or NULL
@return SOAP_OK or error code
*/
int
soap_mec_start_alg(struct soap *soap, int alg, const unsigned char *key)
{ struct soap_mec_data *data;
  int ok = 1;
  data = (struct soap_mec_data*)soap->data[1];
  if (!data)
    return soap->error = SOAP_USER_ERROR;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "MEC Start alg=%x\n", data->alg));
  if (key)
    data->key = key;
  if (alg != SOAP_MEC_NONE)
    data->alg = alg;
  if (data->alg & SOAP_MEC_ENC)
  { unsigned char iv[EVP_MAX_IV_LENGTH];
    int ivlen;
    /* save and override the callbacks */
    data->ffiltersend = soap->ffiltersend;
    soap->ffiltersend = soap_mec_filtersend;
    data->bufidx = 0;
    data->i = 0;
    data->m = 0;
    ivlen = EVP_CIPHER_iv_length(data->type);
    if (ivlen)
    { RAND_pseudo_bytes(iv, ivlen);
      soap_mec_put_base64(soap, data, (unsigned char*)iv, ivlen);
    }
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "IV = "));
    DBGHEX(TEST, iv, ivlen);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--\n"));
    ok = EVP_EncryptInit_ex(data->ctx, NULL, NULL, data->key, iv);
  }
  else
  { size_t len;
    /* algorithm */
    if (data->alg & SOAP_MEC_DES_CBC)
      data->type = EVP_des_ede3_cbc(); /* triple DES CBC */
    else if (data->alg & SOAP_MEC_AES128_CBC)
      data->type = EVP_get_cipherbyname("AES128");
    else if (data->alg & SOAP_MEC_AES192_CBC)
      data->type = EVP_get_cipherbyname("AES192");
    else if (data->alg & SOAP_MEC_AES256_CBC)
      data->type = EVP_get_cipherbyname("AES256");
    else if (data->alg & SOAP_MEC_AES512_CBC)
      data->type = EVP_get_cipherbyname("AES512");
    else
      data->type = EVP_enc_null();
    len = 2 * sizeof(soap->buf) + EVP_CIPHER_block_size(data->type);
    if (!data->buf || data->buflen < len)
    { if (data->buf)
        SOAP_FREE(soap, data->buf);
      data->buflen = len;
      data->buf = (char*)SOAP_MALLOC(soap, data->buflen);
    }
    data->bufidx = soap->buflen - soap->bufidx;
    /* copy buf[bufidx..buflen-1] to data buf */
    memcpy(data->buf, soap->buf + soap->bufidx, data->bufidx);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Alloc buf=%lu, copy %lu message bytes\n", (unsigned long)data->buflen, (unsigned long)data->bufidx));
    /* trigger ffilterrecv() */
    soap->bufidx = soap->buflen;
    /* INIT state */
    data->i = 0;
    data->m = 0;
    data->state = SOAP_MEC_STATE_INIT;
  }
  return soap_mec_check(soap, data, ok, "soap_mec_start() failed");
}

/**
@fn int soap_mec_start(struct soap *soap, const unsigned char *key)
@brief Start encryption or decryption of current message. If key is non-NULL,
use the symmetric triple DES key. Use soap_mec_start only after soap_mec_begin.
The soap_mec_start should be followed by a soap_mec_stop call.
@param soap context
@param[in] key secret triple DES key or NULL
@return SOAP_OK or error code
*/
int
soap_mec_start(struct soap *soap, const unsigned char *key)
{ return soap_mec_start_alg(soap, SOAP_MEC_NONE, key);
}

/**
@fn int soap_mec_stop(struct soap *soap)
@brief Stops encryption or decryption of current message. Use after
soap_mec_start.
@param soap context
@return SOAP_OK or error code
*/
int
soap_mec_stop(struct soap *soap)
{ struct soap_mec_data *data;
  int err = SOAP_OK;
  const char *s = NULL;
  size_t n = 0;
  data = (struct soap_mec_data*)soap->data[1];
  if (!data)
    return soap->error = SOAP_USER_ERROR;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "MEC Stop alg=%x\n", data->alg));
  err = soap_mec_final(soap, data, &s, &n);
  if (data->alg & SOAP_MEC_ENC)
  { /* reset callbacks */
    if (soap->ffiltersend == soap_mec_filtersend)
      soap->ffiltersend = data->ffiltersend;
    /* send remaining cipher data */
    if (!err && n)
      if (soap_send_raw(soap, s, n))
        return soap->error;
  }
  return err;
}

/**
@fn int soap_mec_end(struct soap *soap, struct soap_mec_data *data)
@brief Ends encryption or decryption of a sequence of message parts that began
with soap_mec_begin.
@param soap context
@param[in,out] data mecevp engine context
@return SOAP_OK or error code
*/
int
soap_mec_end(struct soap *soap, struct soap_mec_data *data)
{ DBGLOG(TEST, SOAP_MESSAGE(fdebug, "MEC End alg=%x\n", data->alg));
  /* reset callbacks */
  if (soap->ffiltersend == soap_mec_filtersend)
    soap->ffiltersend = data->ffiltersend;
  if (soap->ffilterrecv == soap_mec_filterrecv)
    soap->ffilterrecv = data->ffilterrecv;
  /* restore the mode flag */
  soap->mode = data->mode;
  /* cleanup and reset mecevp engine */
  soap_mec_cleanup(soap, data);
  soap->data[1] = NULL;
  return SOAP_OK;
}

/**
@fn size_t soap_mec_size(int alg, SOAP_MEC_KEY_TYPE *pkey)
@brief Returns the number of octets needed to store the public/private key or
the symmetric key, depending on the algorithm.
@param[in] alg is the algorithm to be used
@param[in] pkey is a pointer to an EVP_PKEY object or NULL for symmetric keys
@return size_t number of octets that is needed to hold the key.
*/
size_t
soap_mec_size(int alg, SOAP_MEC_KEY_TYPE *pkey)
{ if (alg & SOAP_MEC_ENV)
    return EVP_PKEY_size(pkey);
  switch (alg & SOAP_MEC_MASK & ~SOAP_MEC_ENC)
  { case SOAP_MEC_DES_CBC:
      return 20; /* triple DES 160 bits */
    case SOAP_MEC_AES128_CBC:
      return 16;
    case SOAP_MEC_AES192_CBC:
      return 24;
    case SOAP_MEC_AES256_CBC:
      return 32;
    case SOAP_MEC_AES512_CBC:
      return 64;
  }
  return 0;
}

/******************************************************************************\
 *
 * Static local functions
 *
\******************************************************************************/

/**
@fn int soap_mec_upd(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
@brief Update encryption/decryption state depending on the current algorithm
@param soap context
@param[in,out] data mecevp engine context
@param[in,out] s input data to convert, afterwards points to converted data (original content is unchanged)
@param[in,out] n size of input, afterwards size of output
@param[in] final flag to indicate no more input, output is flushed to s
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_upd(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
{ if (!data || !data->ctx)
    return soap->error = SOAP_USER_ERROR;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MEC Update alg=%x n=%lu final=%d (%p) --\n", data->alg, (unsigned long)*n, final, data->ctx));
  DBGMSG(TEST, *s, *n);                                     
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--\n"));
  if (data->alg & SOAP_MEC_ENC)
  { if (soap_mec_upd_enc(soap, data, s, n, final))
      return soap->error;
  }
  else
  { if (soap_mec_upd_dec(soap, data, s, n, final))
      return soap->error;
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--\n"));
  DBGMSG(TEST, *s, *n);                                     
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--\n"));
  return SOAP_OK;
}

/**
@fn int soap_mec_upd_enc(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
@brief Update encryption state with input plain text (or raw) data and output
in base64 format.
@param soap context
@param[in,out] data mecevp engine context
@param[in,out] s input plain text, afterwards points to output cipher data
@param[in,out] n size of input text, afterwards size of cipher data
@param[in] final flag to indicate no more input, output is flushed to s
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_upd_enc(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
{ size_t k;
  int m;
  int ok = 0;
  /* cipher size */
  k = *n + EVP_CIPHER_block_size(data->type);
  /* scale by base64 size + in-use part + 8 margin */
  m = data->bufidx + 8 + (k + 2) / 3 * 4 + 1;
  /* fits in buf after bufidx? */
  if (m > (int)data->buflen)
  { char *t = data->buf;
    data->buflen = m; /* + slack? */
    data->buf = (char*)SOAP_MALLOC(soap, data->buflen);
    if (t)
    { memcpy(data->buf, t, data->bufidx); /* copy in-use part */
      SOAP_FREE(soap, t);
    }
  }
  if (!final)
  { /* envelope encryption or with shared key? */
    if (data->alg & SOAP_MEC_ENV)
      ok = EVP_SealUpdate(data->ctx, (unsigned char*)data->buf + data->buflen - k, &m, (unsigned char*)*s, (int)*n);
    else
      ok = EVP_EncryptUpdate(data->ctx, (unsigned char*)data->buf + data->buflen - k, &m, (unsigned char*)*s, (int)*n);
    DBGHEX(TEST, (unsigned char*)(data->buf + data->buflen - k), m);
    /* convert to base64 */
    soap_mec_put_base64(soap, data, (unsigned char*)(data->buf + data->buflen - k), m);
    *s = data->buf;
    *n = data->bufidx;
    if (!(data->alg & SOAP_MEC_STORE))
      data->bufidx = 0;
  }
  else
  { /* envelope encryption or with shared key? */
    if (data->alg & SOAP_MEC_ENV)
      ok = EVP_SealFinal(data->ctx, (unsigned char*)data->buf + data->buflen - k, &m);
    else
      ok = EVP_EncryptFinal(data->ctx, (unsigned char*)data->buf + data->buflen - k, &m);
    DBGHEX(TEST, (unsigned char*)(data->buf + data->buflen - k), m);
    /* convert to base64 */
    soap_mec_put_base64(soap, data, (unsigned char*)(data->buf + data->buflen - k), m);
    soap_mec_end_base64(soap, data);
    *s = data->buf;
    *n = data->bufidx;
    if (!(data->alg & SOAP_MEC_STORE))
      data->bufidx = 0;
  }
  if (m > (int)k)
  { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Assertion m<=k failed k=%lu m=%lu\n", (unsigned long)k, (unsigned long)m));
    return soap->error = SOAP_USER_ERROR;
  }
  return soap_mec_check(soap, data, ok, "soap_mec_upd_enc() failed");
}

/**
@fn int soap_mec_upd_dec(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
@brief Update decryption state with input cipher data in base64 format and output in plain text (or raw) format
@param soap context
@param[in,out] data mecevp engine context
@param[in,out] s input cipher data, afterwards points to output plain text
@param[in,out] n size of input cipher data, afterwards size of plain text
@param[in] final flag to indicate no more input, output is flushed to s
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_upd_dec(struct soap *soap, struct soap_mec_data *data, const char **s, size_t *n, int final)
{ const char *r = NULL;
  size_t k = 0, l = 0, m = 0;
  int len = 0;
  int ok = 1;
  enum SOAP_MEC_STATE state = data->state;
  if (final && state == SOAP_MEC_STATE_DECRYPT)
    data->state = SOAP_MEC_STATE_FINAL;
  /* if flushing the buf, no base64-decode or decryption to do */
  if (state == SOAP_MEC_STATE_FLUSH)
  { /* old + new fit in buf? */
    if (data->bufidx + *n > data->buflen)
    { char *t = data->buf;
      do
        data->buflen += sizeof(soap->buf);
      while (data->buflen < data->bufidx + *n);
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Enlarging buffer n=%lu\n", (unsigned long)data->buflen));
      data->buf = (char*)SOAP_MALLOC(soap, data->buflen);
      if (t)
      { memcpy(data->buf, t, data->bufidx); /* copy old */
        SOAP_FREE(soap, t);
      }
    }
    /* concat old + new */
    memcpy(data->buf + data->bufidx, *s, *n);
    *s = data->buf;
    *n += data->bufidx;
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Flush state n=%lu\n", (unsigned long)*n));
    /* release old + new for next round (assuming consumer fetches all) */
    if (!(data->alg & SOAP_MEC_STORE))
      data->bufidx = 0;
    return SOAP_OK;
  }
  if (state == SOAP_MEC_STATE_INIT)
  { /* at init, base64 is in data->buf[bufidx] copied from buf[] */
    data->i = 0;
    data->m = 0;
    k = (data->bufidx + *n + 3) / 4 * 3; /* decoded size from old + new */
  }
  else
    k = (*n + 3) / 4 * 3; /* base64-decoded size */
  m = k + EVP_CIPHER_block_size(data->type); /* decrypted data size */
  /* decrypted + base64-decoded will fit in current buf? */
  if (data->bufidx + m > data->buflen - k)
  { /* no, need to enlarge */
    char *t = data->buf;
    do
      data->buflen += sizeof(soap->buf);
    while (data->buflen < data->bufidx + m + k);
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Enlarging buffer n=%lu\n", (unsigned long)data->buflen));
    data->buf = (char*)SOAP_MALLOC(soap, data->buflen);
    if (t)
    { memcpy(data->buf, t, data->bufidx); /* copy old part */
      SOAP_FREE(soap, t);
    }
  }
  /* base64 decode */
  if (state == SOAP_MEC_STATE_INIT)
  { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Init state stage 1 n=%lu\n", (unsigned long)data->bufidx));
    /* base64 is in data buf[0..bufidx-1] and *s */
    if (soap_mec_get_base64(soap, data, data->buf + data->buflen - k, &m, data->buf, data->bufidx, &r, &l))
      return soap->error;
    /* position 'r' is at a spot that gets overwritten, copy to rest */
    if (r && l)
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Init state stage 2 rest=%lu bytes\n", (unsigned long)l));
    }
    else
    { size_t j;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Init state stage 3 n=%lu\n", (unsigned long)*n));
      /* base64-decode *s */
      if (soap_mec_get_base64(soap, data, data->buf + data->buflen - k + m, &j, *s, *n, &r, &l))
        return soap->error;
      m += j;
    }
    data->bufidx = 0;
  }
  else if (state != SOAP_MEC_STATE_FINAL && state != SOAP_MEC_STATE_FLUSH)
  { /* base64-decode *s */
    if (soap_mec_get_base64(soap, data, data->buf + data->buflen - k, &m, *s, *n, &r, &l))
      return soap->error;
  }
  if (r && l)
  { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Rest=%lu bytes\n", (unsigned long)l));
    if (data->restlen < l)
    { if (data->rest)
        SOAP_FREE(soap, data->rest);
      data->rest = (char*)SOAP_MALLOC(soap, l);
    }
    data->restlen = l;
    memcpy(data->rest, r, l);
  }
  /* debug */
  DBGHEX(TEST, (unsigned char*)(data->buf + data->buflen - k), m);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--\n"));
  /* decryption of data buf[buflen-k] */
  switch (data->state)
  { case SOAP_MEC_STATE_INIT:
      /* move to next state */
      state = SOAP_MEC_STATE_IV;
    case SOAP_MEC_STATE_IV:
      /* get the IV data from buf[buflen-k] */
      memmove(data->buf + data->bufidx, data->buf + data->buflen - k, m);
      /* add to IV */
      data->bufidx += m;
      /* got all IV data? */
      if (data->bufidx >= (size_t)EVP_CIPHER_iv_length(data->type))
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Get IV = "));
        DBGHEX(TEST, (unsigned char*)data->buf, EVP_CIPHER_iv_length(data->type));
        DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\nInitializing alg=%x\n", data->alg));
        switch (data->alg & SOAP_MEC_MASK)
        { case SOAP_MEC_ENV_DEC_DES_CBC:
          case SOAP_MEC_ENV_DEC_AES128_CBC:
          case SOAP_MEC_ENV_DEC_AES192_CBC:
          case SOAP_MEC_ENV_DEC_AES256_CBC:
          case SOAP_MEC_ENV_DEC_AES512_CBC:
            ok = EVP_OpenInit(data->ctx, data->type, data->key, data->keylen, (unsigned char*)data->buf, (EVP_PKEY*)data->pkey);
            DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EVP_OpenInit ok=%d\n", ok));
	    break;
          case SOAP_MEC_DEC_DES_CBC:
          case SOAP_MEC_DEC_AES128_CBC:
          case SOAP_MEC_DEC_AES192_CBC:
          case SOAP_MEC_DEC_AES256_CBC:
          case SOAP_MEC_DEC_AES512_CBC:
            ok = EVP_DecryptInit_ex(data->ctx, data->type, NULL, data->key, (unsigned char*)data->buf);
            DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EVP_DecryptInit_ex ok=%d\n", ok));
	    break;
	}
	if (ok)
	{ /* shift rest of data to cipher section */
          k = data->bufidx - EVP_CIPHER_iv_length(data->type);
	  memmove(data->buf + data->buflen - k, data->buf + EVP_CIPHER_iv_length(data->type), k);
          DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Decrypt %lu bytes\n", (unsigned long)k));
	  /* decrypt to buf */
	  len = 0;
          switch (data->alg & SOAP_MEC_MASK)
          { case SOAP_MEC_ENV_DEC_DES_CBC:
            case SOAP_MEC_ENV_DEC_AES128_CBC:
            case SOAP_MEC_ENV_DEC_AES192_CBC:
            case SOAP_MEC_ENV_DEC_AES256_CBC:
            case SOAP_MEC_ENV_DEC_AES512_CBC:
              ok = EVP_OpenUpdate(data->ctx, (unsigned char*)data->buf, &len, (unsigned char*)(data->buf + data->buflen - k), k);
	      break;
            case SOAP_MEC_DEC_DES_CBC:
            case SOAP_MEC_DEC_AES128_CBC:
            case SOAP_MEC_DEC_AES192_CBC:
            case SOAP_MEC_DEC_AES256_CBC:
            case SOAP_MEC_DEC_AES512_CBC:
              ok = EVP_DecryptUpdate(data->ctx, (unsigned char*)data->buf, &len, (unsigned char*)(data->buf + data->buflen - k), k);
	      break;
          }
	  *s = data->buf;
	  *n = (size_t)len;
	}
        if (!(data->alg & SOAP_MEC_STORE))
	  data->bufidx = 0; /* next decoded goes to start of buf */
	else
          data->bufidx = *n;
        /* next state */
        state = SOAP_MEC_STATE_DECRYPT;
      }
      else
      { /* nothing to return yet, need more data */
        *n = 0;
      }
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "IV/decrypt state n=%lu\n", (unsigned long)*n));
      break;
    case SOAP_MEC_STATE_DECRYPT:
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Decrypt %lu bytes\n", (unsigned long)m));
      len = 0;
      switch (data->alg & SOAP_MEC_MASK)
      { case SOAP_MEC_ENV_DEC_DES_CBC:
        case SOAP_MEC_ENV_DEC_AES128_CBC:
        case SOAP_MEC_ENV_DEC_AES192_CBC:
        case SOAP_MEC_ENV_DEC_AES256_CBC:
        case SOAP_MEC_ENV_DEC_AES512_CBC:
          ok = EVP_OpenUpdate(data->ctx, (unsigned char*)(data->buf + data->bufidx), &len, (unsigned char*)(data->buf + data->buflen - k), m);
	  break;
        case SOAP_MEC_DEC_DES_CBC:
        case SOAP_MEC_DEC_AES128_CBC:
        case SOAP_MEC_DEC_AES192_CBC:
        case SOAP_MEC_DEC_AES256_CBC:
        case SOAP_MEC_DEC_AES512_CBC:
          ok = EVP_DecryptUpdate(data->ctx, (unsigned char*)(data->buf + data->bufidx), &len, (unsigned char*)data->buf + data->buflen - k, m);
	  break;
      }
      *s = data->buf;
      *n = data->bufidx + (size_t)len;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Decrypt state n=%lu\n", (unsigned long)*n));
      if (!(data->alg & SOAP_MEC_STORE))
        data->bufidx = 0; /* next decoded goes to start of buf */
      break;
    case SOAP_MEC_STATE_FINAL:
    { /* we know there is enough space to flush *s and *n through the buf */
      const char *t = *s;
      k = *n;
      len = 0;
      switch (data->alg & SOAP_MEC_MASK)
      { case SOAP_MEC_ENV_DEC_DES_CBC:
        case SOAP_MEC_ENV_DEC_AES128_CBC:
        case SOAP_MEC_ENV_DEC_AES192_CBC:
        case SOAP_MEC_ENV_DEC_AES256_CBC:
        case SOAP_MEC_ENV_DEC_AES512_CBC:
          ok = EVP_OpenFinal(data->ctx, (unsigned char*)(data->buf + data->bufidx), &len);
          break;
        case SOAP_MEC_DEC_DES_CBC:
        case SOAP_MEC_DEC_AES128_CBC:
        case SOAP_MEC_DEC_AES192_CBC:
        case SOAP_MEC_DEC_AES256_CBC:
        case SOAP_MEC_DEC_AES512_CBC:
          ok = EVP_DecryptFinal(data->ctx, (unsigned char*)(data->buf + data->bufidx), &len);
          break;
      }
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Append %d bytes from decrypted\n", len));
      *s = data->buf;
      *n = data->bufidx + (size_t)len;
      if (data->restlen)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Append %lu bytes from rest\n", data->restlen));
        memcpy(data->buf + *n, data->rest, data->restlen);
        *n += data->restlen;
      }
      if (k)
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Append %lu bytes from input\n", k));
        memmove(data->buf + *n, t, k);
        *n += k;
      }
      if (!(data->alg & SOAP_MEC_STORE))
        data->bufidx = 0; /* next decoded goes to start of buf */
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Final len=%lu\n", (unsigned long)*n));
      state = SOAP_MEC_STATE_FLUSH; /* flush data buf, if needed */
      break;
    }
    default:
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Error in decryption state\n"));
      return soap->error = SOAP_SSL_ERROR;
  }
  if (l)
  { if (state == SOAP_MEC_STATE_DECRYPT)
    { state = SOAP_MEC_STATE_FINAL;
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Append rest of stream %lu (%lu <= %lu)\n", data->restlen, *n, data->buflen));
    }
    else
    { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Error in decryption state\n"));
      return soap->error = SOAP_SSL_ERROR;
    }
  }
  data->state = state;
  return soap_mec_check(soap, data, ok, "soap_mec_upd_dec() failed");
}

/**
@fn int soap_mec_check(struct soap *soap, struct soap_mec_data *data, int ok, const char *msg)
@brief Check result of init/update/final mecevp engine operations.
@param soap context
@param[in,out] data mecevp engine context
@param[in] ok EVP error value
@param[in] msg error message
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_check(struct soap *soap, struct soap_mec_data *data, int ok, const char *msg)
{ if (ok <= 0)
  { unsigned long r;
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- MEC Error (%d)", ok));
    while ((r = ERR_get_error()))
    { ERR_error_string_n(r, soap->msgbuf, sizeof(soap->msgbuf));
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, " %s: \"%s\";", msg, soap->msgbuf));
    }
    DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n"));
    /* cleanup and reset mecevp engine */
    soap_mec_cleanup(soap, data);
    return soap_set_receiver_error(soap, msg, soap->msgbuf, SOAP_SSL_ERROR);
  }
  return SOAP_OK;
}

/**
@fn void soap_mec_put_base64(struct soap *soap, struct soap_mec_data *data, const unsigned char *s, int n)
@brief Write base64 formatted data stored in s of length n to internal buffer
@param soap context
@param[in,out] data mecevp engine context
@param[in] s data to convert
@param[in] n length of data to convert
*/
static void
soap_mec_put_base64(struct soap *soap, struct soap_mec_data *data, const unsigned char *s, int n)
{ char *t;
  int i;
  unsigned long m;
  if (!s || !n)
    return;
  t = data->buf + data->bufidx;
  i = data->i;
  m = data->m;
  while (n--)
  { m = (m << 8) | *s++;
    if (i++ == 2)
    { for (i = 4; i > 0; m >>= 6)
        t[--i] = soap_base64o[m & 0x3F];
      t += 4;
      data->bufidx += 4;
    }
  }
  data->i = i;
  data->m = m;
}

/**
@fn void soap_mec_end_base64(struct soap *soap, struct soap_mec_data *data)
@brief End writing base64 formatted data to internal buffer
@param soap context
@param[in,out] data mecevp engine context
*/
static void
soap_mec_end_base64(struct soap *soap, struct soap_mec_data *data)
{ if (data->i)
  { char *t;
    int i;
    unsigned long m;
    t = data->buf + data->bufidx;
    i = data->i;
    m = data->m;
    for (; i < 3; i++)
      m <<= 8;
    for (i++; i > 0; m >>= 6)
      t[--i] = soap_base64o[m & 0x3F];
    for (i = 3; i > data->i; i--)
      t[i] = '=';
    data->bufidx += 4;
  }
  data->i = 0;
  data->m = 0;
}

/**
@fn int soap_mec_get_base64(struct soap *soap, struct soap_mec_data *data, char *t, size_t *l, const char *s, size_t n, const char **r, size_t *k)
@brief Convert base64-formatted data from s[0..n-1] into raw data in t[0..l-1]
where l is the max size and set equal or lower if data fits in t. If data does
not fit r points to remainder in s[0..n-1] of size k.
@param soap context
@param[in,out] data mecevp engine context
@param[in] t raw data (converted from base64)
@param[in,out] l max size of t[], afterwards actual size of data written to t[]
@param[in] s data in base64 format
@param[in] n size of base64 data
@param[out] r if data does not fit in t[], points to s[] remainder to convert
@param[out] k if data does not fit in t[], size of remainder in r[]
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_get_base64(struct soap *soap, struct soap_mec_data *data, char *t, size_t *l, const char *s, size_t n, const char **r, size_t *k)
{ int i;
  unsigned long m;
  size_t j;
  int c;
  i = data->i;
  m = data->m;
  j = 0;
  for (;;)
  { do
    { if (!n--)
      { *l = j;
        data->i = i;
        data->m = m;
	*r = NULL;
	*k = 0;
	return SOAP_OK;
      }
      c = *s++;
      if (c == '=' || c == '<')
      { switch (i)
        { case 2:
            *t++ = (char)((m >> 4) & 0xFF);
            j++;
            break;
          case 3:
            *t++ = (char)((m >> 10) & 0xFF);
            *t++ = (char)((m >> 2) & 0xFF);
            j += 2;
        }
	if (c == '<')
        { s--;
	  n++;
	}
	else if (n && *s == '=')
        { s++;
	  n--;
	}
        *l = j;
	*k = n;
	if (n)
	  *r = s;
	else
	  *r = NULL;
        return SOAP_OK;
      }
      if (c >= '+' && c <= '+' + 79)
      { register int b = soap_base64i[c - '+'];
        if (b >= 64)
          return soap->error = SOAP_SSL_ERROR;
        m = (m << 6) + b;
        i++;
      }
      else if (c < 0 || c > 32)
        return soap->error = SOAP_SSL_ERROR;
    } while (i < 4);
    *t++ = (char)((m >> 16) & 0xFF);
    *t++ = (char)((m >> 8) & 0xFF);
    *t++ = (char)(m & 0xFF);
    j += 3;
    i = 0;
    m = 0;
  }
}

/******************************************************************************\
 *
 * Callbacks registered by plugin
 *
\******************************************************************************/

/**
@fn int soap_mec_filtersend(struct soap *soap, const char **s, size_t *n)
@brief Callback to modify outbound messages by encrypting through the engine.
@param soap context
@param[in,out] s plain text message, afterwards set to encrypted message
@param[in,out] n plain text message size, afterwards set to encrypted message size
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_filtersend(struct soap *soap, const char **s, size_t *n)
{ struct soap_mec_data *data = (struct soap_mec_data*)soap->data[1];
  if (!data)
     return SOAP_OK;
  /* encrypt to base64 */
  return soap_mec_upd(soap, data, s, n, 0);
}

/**
@fn int soap_mec_filterrecv(struct soap *soap, char *buf, size_t *len, size_t maxlen)
@brief Callback to modify inbound messages by decrypting through the engine.
@param soap context
@param[in,out] buf encrypted message, afterwards contains decrypted content
@param[in,out] len encrypted message size, afterwards set to decrypted content size
@param[in] maxlen max length of allocated buf size to contain decrypted content
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_mec_filterrecv(struct soap *soap, char *buf, size_t *len, size_t maxlen)
{ struct soap_mec_data *data = (struct soap_mec_data*)soap->data[1];
  const char *s = buf;
  if (!data || (data->alg & SOAP_MEC_MASK) == SOAP_MEC_NONE || (data->alg & SOAP_MEC_ENC))
    return SOAP_OK;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Filter recv in=%lu\n", (unsigned long)*len));
  /* convert s[len] to new s with len (new s = data->buf) */
  if (soap_mec_upd(soap, data, &s, len, 0))
    return soap->error;
  /* does the result fit in buf[maxlen]? */
  if (*len <= maxlen)
    memcpy(buf, s, *len); /* yes: copy data to buf[] */
  else
  { memcpy(buf, s, maxlen); /* no: copy first part to buf[maxlen] */
    memmove(data->buf, s + maxlen, *len - maxlen); /* shift rest to the left */
    data->bufidx = *len - maxlen; /* keep rest of the data in s (data->buf) */
    *len = maxlen;
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Filter recv out=%lu\n", (unsigned long)*len));
  return SOAP_OK;
}

#ifdef __cplusplus
}
#endif

