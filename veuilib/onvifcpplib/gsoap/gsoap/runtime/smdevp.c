/*
	smdevp.c

	gSOAP interface for (signed) message digest

gSOAP XML Web services tools
Copyright (C) 2000-2008, Robert van Engelen, Genivia Inc., All Rights Reserved.
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
Copyright (C) 2000-2008, Robert van Engelen, Genivia, Inc., All Rights Reserved.
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
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

/**

@page smdevp The smdevp engine

The gSOAP smdevp engine computes (signed) message digests over any type of data
using the EVP interface of OpenSSL. It currently supports MD5, SHA1, HMAC_SHA1,
DSA_SHA1, and RSA_SHA1.

A digest or signature algorithm is selected with one the following constants:

- @ref SOAP_SMD_DGST_MD5	to compute MD5 128-bit digests
- @ref SOAP_SMD_DGST_SHA1	to compute MD5 160-bit digests
- @ref SOAP_SMD_HMAC_SHA1	to compute HMAC-SHA1 message authentication code
- @ref SOAP_SMD_SIGN_DSA_SHA1	to compute DSA-SHA1 signatures
- @ref SOAP_SMD_SIGN_RSA_SHA1	to compute RSA-SHA1 signatures
- @ref SOAP_SMD_VRFY_DSA_SHA1	to verify DSA-SHA1 signatures
- @ref SOAP_SMD_VRFY_RSA_SHA1	to verify RSA-SHA1 signatures

The smdevp engine wraps the EVP API with three new functions:

- @ref soap_smd_init	to initialize the engine
- @ref soap_smd_update	to update the state with a message part
- @ref soap_smd_final	to compute the digest, signature, or verify a signature

A higher-level interface for computing (signed) message digests over
messages produced by the gSOAP enginre is defined by two new functions:

- @ref soap_smd_begin	to start a digest or signature computation/verification
- @ref soap_smd_end	to complete a digest/signature computation/verification

Here is an example to sign an XML serialized C++ object using an RSA private
key applied to the SHA1 digest of the serialized object:

@code
    ns__Object *object = ...;
    int alg = SOAP_SMD_SIGN_RSA_SHA1;
    FILE *fd = fopen("key.pem", "r");
    EVP_PKEY *key = PEM_read_PrivateKey(fd, NULL, NULL, "password");
    char *sig = (char*)soap_malloc(soap, soap_smd_size(alg, key));
    int siglen;
    fclose(fd);
    if (soap_smd_begin(soap, alg, key, 0)
     || soap_out_ns__Object(soap, "ns:Object", 0, object, NULL)
     || soap_smd_end(soap, sig, &siglen))
      soap_print_fault(soap, stderr);
    else
      ... // sig contains RSA-SHA1 signature of length siglen 
@endcode

To verify the signature, we use the RSA public key and re-run the octet stream
(by re-serialization in this example) through the smdevp engine using the
SOAP_SMD_VRFY_RSA_SHA1 algorithm. Note that a PEM file may contain both the
(encrypted) private and public keys.

@code
    char *sig = ...;
    int siglen = ...;
    ns__Object *object = ...;
    int alg = SOAP_SMD_VRFY_RSA_SHA1;
    FILE *fd = fopen("key.pem", "r");
    EVP_PKEY *key = PEM_read_PUBKEY(fd, NULL, NULL, NULL);
    fclose(fd);
    if (soap_smd_begin(soap, alg, key, 0)
     || soap_out_ns__Object(soap, "ns:Object", 0, object, NULL)
     || soap_smd_end(soap, sig, &siglen))
      soap_print_fault(soap, stderr);
    else
      ... // sig verified, i.e. signed object was not changed
@endcode

The HMAC algorithm uses a secret key (which both the sender and receiver must
keep secret) to sign and verify a message:

@code
    ns__Object *object = ...;
    int alg = SOAP_SMD_HMAC_SHA1;
    static char key[16] =
    { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    char *sig = (char*)soap_malloc(soap, soap_smd_size(alg, NULL));
    int siglen;
    if (soap_smd_begin(soap, alg, key, sizeof(key))
     || soap_out_ns__Object(soap, "ns:Object", 0, object, NULL)
     || soap_smd_end(soap, sig, &siglen))
      soap_print_fault(soap, stderr);
    else
      ... // sig holds the signature
@endcode

Note: HMAC signature verification proceeds by recomputing the signature value
for comparison.

A digest is a hash value of an octet stream computed using the MD5 or SHA1
algorithms:

@code
    ns__Object *object = ...;
    int alg = SOAP_SMD_DGST_SHA1;
    char *digest = (char*)soap_malloc(soap, soap_smd_size(alg, NULL));
    int digestlen;
    if (soap_smd_begin(soap, alg, NULL, 0)
     || soap_out_ns__Object(soap, "ns:Object", 0, object, NULL)
     || soap_smd_end(soap, digest, &digestlen))
      soap_print_fault(soap, stderr);
    else
      ... // digest holds hash value of serialized object
@endcode

Note that indentation (SOAP_XML_INDENT) and exc-c14n canonicalization
(SOAP_XML_CANONICAL) affects the XML serialization format and, therefore,
the digest or signature produced.

*/

#include "smdevp.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************\
 *
 * Static local functions used
 *
\******************************************************************************/

static int soap_smd_send(struct soap *soap, const char *buf, size_t len);
static size_t soap_smd_recv(struct soap *soap, char *buf, size_t len);
static int soap_smd_check(struct soap *soap, struct soap_smd_data *data, int err, const char *msg);

/******************************************************************************\
 *
 * soap_smd API functions
 *
\******************************************************************************/

/**
@fn size_t soap_smd_size(int alg, const void *key)
@brief Returns the number of octets needed to store the digest or signature returned by soap_smd_end.
@param[in] alg is the digest or signature algorithm to be used
@param[in] key is a pointer to an EVP_PKEY object for RSA/DSA signatures or NULL for digests and HMAC
@return size_t number of octets that is needed to hold digest or signature
@see soap_smd_end

The values returned for digests are SOAP_SMD_MD5_SIZE and SOAP_SMD_SHA1_SIZE.
*/
size_t
soap_smd_size(int alg, const void *key)
{ switch (alg)
  { case SOAP_SMD_DGST_MD5:
      return SOAP_SMD_MD5_SIZE;
    case SOAP_SMD_DGST_SHA1:
    case SOAP_SMD_HMAC_SHA1:
      return SOAP_SMD_SHA1_SIZE;
    case SOAP_SMD_SIGN_DSA_SHA1:
    case SOAP_SMD_SIGN_RSA_SHA1:
    case SOAP_SMD_VRFY_DSA_SHA1:
    case SOAP_SMD_VRFY_RSA_SHA1:
      /* OpenSSL EVP_PKEY_size returns size of signatures given a key */
      return EVP_PKEY_size((EVP_PKEY*)key);
  }
  return 0;
}

/**
@fn int soap_smd_begin(struct soap *soap, int alg, const void *key, int keylen)
@brief Initiates a digest or signature computation.
@param soap context
@param[in] alg is the digest or signature (sign/verification) algorithm used
@param[in] key is a HMAC key or pointer to EVP_PKEY object or NULL for digests
@param[in] keylen is the length of the HMAC key or 0
@return SOAP_OK, SOAP_EOM, or SOAP_SSL_ERROR
*/
int
soap_smd_begin(struct soap *soap, int alg, const void *key, int keylen)
{ struct soap_smd_data *data;
  data = (struct soap_smd_data*)SOAP_MALLOC(soap, sizeof(struct soap_smd_data));
  if (!data)
    return soap->error = SOAP_EOM;
  /* save and set the 'user' field to pass data to the callbacks */
  data->user = soap->user;
  soap->user = (void*)data;
  /* save and set the send and recv callbacks */
  data->fsend = soap->fsend;
  data->frecv = soap->frecv;
  soap->fsend = soap_smd_send;
  soap->frecv = soap_smd_recv;
  /* save the mode flag */
  data->mode = soap->mode;
  /* clear the IO flags and DOM flag */
  soap->mode &= ~(SOAP_IO | SOAP_IO_LENGTH | SOAP_ENC_ZLIB | SOAP_XML_DOM);
  /* clear the XML attribute store */
  soap_clr_attr(soap);
  /* load the local XML namespaces store */
  soap_set_local_namespaces(soap);
  if (soap->mode & SOAP_XML_CANONICAL)
    soap->ns = 0; /* for in c14n, we must have all xmlns bindings available */
  else
    soap->ns = 2; /* we don't want leading whitespace in serialized XML */
  /* init the soap_smd engine */
  return soap_smd_init(soap, data, alg, key, keylen);
}

/**
@fn int soap_smd_end(struct soap *soap, char *buf, int *len)
@brief Completes a digest or signature computation.
@param soap context
@param[in] buf contains signature for verification (when using a SOAP_SMD_VRFY algorithm)
@param[out] buf is populated with the digest or signature
@param[in] len points to length of signature to verify (when using a SOAP_SMD_VRFY algorithm)
@param[out] len points to length of stored digest or signature (when not NULL)
@return SOAP_OK, SOAP_USER_ERROR, or SOAP_SSL_ERROR
*/
int
soap_smd_end(struct soap *soap, char *buf, int *len)
{ struct soap_smd_data *data;
  int err;
  data = (struct soap_smd_data*)soap->user;
  if (!data)
    return SOAP_USER_ERROR;
  /* finalize the digest/signature computation and store data in buf + len */
  /* for signature verification, buf + len contain the signature */
  err = soap_smd_final(soap, data, buf, len);
  /* restore the callbacks */
  soap->fsend = data->fsend;
  soap->frecv = data->frecv;
  /* restore the mode flag */
  soap->mode = data->mode;
  /* restore the 'user' data */
  soap->user = data->user;
  /* free data */
  SOAP_FREE(soap, data);
  /* return SOAP_OK or error */
  return err;
}

/**
@fn int soap_smd_init(struct soap *soap, struct soap_smd_data *data, int alg, const void *key, int keylen)
@brief Initiates a (signed) digest computation.
@param soap context
@param[in,out] data smdevp engine context
@param[in] alg is algorithm to use
@param[in] key is key to use or NULL for digests
@param[in] keylen is length of HMAC key (when provided)
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_smd_init(struct soap *soap, struct soap_smd_data *data, int alg, const void *key, int keylen)
{ static int done = 0;
  int err = 1;
#ifdef WITH_OPENSSL
  /* OpenSSL: make sure we have the digest algorithms */
  if (!done)
  { done = 1;
    OpenSSL_add_all_digests();
    OpenSSL_add_all_algorithms();
  }
#endif
  /* the algorithm to use */
  data->alg = alg;
  /* the key to use */
  data->key = key;
  /* allocate and init the OpenSSL HMAC or EVP_MD context */
  if (alg == SOAP_SMD_HMAC_SHA1)
  { data->ctx = (void*)SOAP_MALLOC(soap, sizeof(HMAC_CTX));
    HMAC_CTX_init((HMAC_CTX*)data->ctx);
  }
  else
  { data->ctx = (void*)SOAP_MALLOC(soap, sizeof(EVP_MD_CTX));
    EVP_MD_CTX_init((EVP_MD_CTX*)data->ctx);
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- SMD Init alg=%d (%p) --\n", alg, data->ctx));
  /* init the digest or signature computations */
  switch (alg)
  { case SOAP_SMD_DGST_MD5:
      EVP_DigestInit((EVP_MD_CTX*)data->ctx, EVP_md5());
      break;
    case SOAP_SMD_DGST_SHA1:
      EVP_DigestInit((EVP_MD_CTX*)data->ctx, EVP_sha1());
      break;
    case SOAP_SMD_HMAC_SHA1:
      HMAC_Init((HMAC_CTX*)data->ctx, key, keylen, EVP_sha1());
      break;
    case SOAP_SMD_SIGN_DSA_SHA1:
      err = EVP_SignInit((EVP_MD_CTX*)data->ctx, EVP_dss1());
      break;
    case SOAP_SMD_SIGN_RSA_SHA1:
      err = EVP_SignInit((EVP_MD_CTX*)data->ctx, EVP_sha1());
      break;
    case SOAP_SMD_VRFY_DSA_SHA1:
      err = EVP_VerifyInit((EVP_MD_CTX*)data->ctx, EVP_dss1());
      break;
    case SOAP_SMD_VRFY_RSA_SHA1:
      err = EVP_VerifyInit((EVP_MD_CTX*)data->ctx, EVP_sha1());
      break;
  }
  /* check and return */
  return soap_smd_check(soap, data, err, "soap_smd_init() failed");
}

/**
@fn int soap_smd_update(struct soap *soap, struct soap_smd_data *data, const char *buf, size_t len)
@brief Updates (signed) digest computation with message part.
@param soap context
@param[in,out] data smdevp engine context
@param[in] buf contains message part
@param[in] len of message part
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_smd_update(struct soap *soap, struct soap_smd_data *data, const char *buf, size_t len)
{ int err = 1;
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- SMD Update alg=%d (%p) --\n", data->alg, data->ctx));
  switch (data->alg)
  { case SOAP_SMD_DGST_MD5:
    case SOAP_SMD_DGST_SHA1:
      EVP_DigestUpdate((EVP_MD_CTX*)data->ctx, (const void*)buf, (unsigned int)len);
      break;
    case SOAP_SMD_HMAC_SHA1:
      HMAC_Update((HMAC_CTX*)data->ctx, (const unsigned char*)buf, len);
      break;
    case SOAP_SMD_SIGN_DSA_SHA1:
    case SOAP_SMD_SIGN_RSA_SHA1:
      err = EVP_SignUpdate((EVP_MD_CTX*)data->ctx, (const void*)buf, (unsigned int)len);
      break;
    case SOAP_SMD_VRFY_DSA_SHA1:
    case SOAP_SMD_VRFY_RSA_SHA1:
      err = EVP_VerifyUpdate((EVP_MD_CTX*)data->ctx, (const void*)buf, (unsigned int)len);
      break;
  }
  DBGMSG(TEST, buf, len);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--"));
  /* check and return */
  return soap_smd_check(soap, data, err, "soap_smd_update() failed");
}

/**
@fn int soap_smd_final(struct soap *soap, struct soap_smd_data *data, char *buf, int *len)
@brief Finalizes (signed) digest computation and returns digest or signature.
@param soap context
@param[in,out] data smdevp engine context
@param[in] buf contains signature for verification (SOAP_SMD_VRFY algorithms)
@param[out] buf is populated with the digest or signature
@param[in] len points to length of signature to verify (SOAP_SMD_VRFY algorithms)
@param[out] len points to length of stored digest or signature (pass NULL if you are not interested in this value)
@return SOAP_OK or SOAP_SSL_ERROR
*/
int
soap_smd_final(struct soap *soap, struct soap_smd_data *data, char *buf, int *len)
{ unsigned int n = 0;
  int err = 1;
  /* finalize the digest or signature computation */
  switch (data->alg)
  { case SOAP_SMD_DGST_MD5:
    case SOAP_SMD_DGST_SHA1:
      EVP_DigestFinal((EVP_MD_CTX*)data->ctx, (unsigned char*)buf, &n);
      break;
    case SOAP_SMD_HMAC_SHA1:
      HMAC_Final((HMAC_CTX*)data->ctx, (unsigned char*)buf, &n);
      break;
    case SOAP_SMD_SIGN_DSA_SHA1:
    case SOAP_SMD_SIGN_RSA_SHA1:
      err = EVP_SignFinal((EVP_MD_CTX*)data->ctx, (unsigned char*)buf, &n, (EVP_PKEY*)data->key);
      break;
    case SOAP_SMD_VRFY_DSA_SHA1:
    case SOAP_SMD_VRFY_RSA_SHA1:
      if (len)
      { n = (unsigned int)*len;
        err = EVP_VerifyFinal((EVP_MD_CTX*)data->ctx, (unsigned char*)buf, n, (EVP_PKEY*)data->key);
      }
      else
        err = 0;
      break;
  }
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- SMD Final alg=%d (%p) %d bytes--\n", data->alg, data->ctx, n));
  DBGHEX(TEST, buf, n);
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "\n--"));
  /* cleanup and free the HMAC or EVP_MD context */
  if (data->alg == SOAP_SMD_HMAC_SHA1)
    HMAC_CTX_cleanup((HMAC_CTX*)data->ctx);
  else
    EVP_MD_CTX_cleanup((EVP_MD_CTX*)data->ctx);
  SOAP_FREE(soap, data->ctx);
  data->ctx = NULL;
  /* return length of digest or signature produced */
  if (len)
    *len = (int)n;
  /* check and return */
  return soap_smd_check(soap, data, err, "soap_smd_final() failed");
}

/******************************************************************************\
 *
 * Static local functions
 *
\******************************************************************************/

/**
@fn int soap_smd_check(struct soap *soap, struct soap_smd_data *data, int err, const char *msg)
@brief Check result of init/update/final smdevp engine operations.
@param soap context
@param[in,out] data smdevp engine context
@param[in] err OpenSSL error value
@param[in] msg error message
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_smd_check(struct soap *soap, struct soap_smd_data *data, int err, const char *msg)
{ if (err <= 0)
  { unsigned long r;
    while ((r = ERR_get_error()))
    { ERR_error_string_n(r, soap->msgbuf, sizeof(soap->msgbuf));
      DBGLOG(TEST, SOAP_MESSAGE(fdebug, "-- SMD Error (%d) %s: %s\n", err, msg, soap->msgbuf));
    }
    if (data->ctx)
    { if (data->alg == SOAP_SMD_HMAC_SHA1)
        HMAC_CTX_cleanup((HMAC_CTX*)data->ctx);
      else
        EVP_MD_CTX_cleanup((EVP_MD_CTX*)data->ctx);
      SOAP_FREE(soap, data->ctx);
      data->ctx = NULL;
    }
    return soap_set_receiver_error(soap, msg, soap->msgbuf, SOAP_SSL_ERROR);
  }
  return SOAP_OK;
}

/**
@fn int soap_smd_send(struct soap *soap, const char *buf, size_t len)
@brief Callback to intercept messages for digest or signature computation.
@param soap context
@param[in] buf message
@param[in] len message length
@return SOAP_OK or SOAP_SSL_ERROR
*/
static int
soap_smd_send(struct soap *soap, const char *buf, size_t len)
{ return soap_smd_update(soap, (struct soap_smd_data*)soap->user, buf, len);
}

/**
@fn size_t soap_smd_recv(struct soap *soap, char *buf, size_t len)
@brief Dummy callback to avoid read operations from blocking.
@return 0
*/
static size_t
soap_smd_recv(struct soap *soap, char *buf, size_t len)
{ return 0;
}

#ifdef __cplusplus
}
#endif

