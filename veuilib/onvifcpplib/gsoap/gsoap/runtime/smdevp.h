/*
	smdevp.h

	gSOAP interface for (signed) message digest

gSOAP XML Web services tools
Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc., All Rights Reserved.
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
Copyright (C) 2000-2005, Robert van Engelen, Genivia, Inc., All Rights Reserved.
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

#ifndef SMDEVP_H
#define SMDEVP_H

#include "stdsoap2.h"

#ifdef WITH_OPENSSL
#include <openssl/evp.h>
#include <openssl/hmac.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Expose EVP_PKEY in a portable representation */
#define SOAP_SMD_KEY_TYPE	EVP_PKEY

/** Expose EVP_MAX_MD_SIZE in a portable representation */
#define SOAP_SMD_MAX_SIZE	EVP_MAX_MD_SIZE

/** MD5 digest size in octets */
#define SOAP_SMD_MD5_SIZE	(16)

/** SHA1 digest size in octets */
#define SOAP_SMD_SHA1_SIZE	(20)

/******************************************************************************\
 *
 * Supported algorithms
 *
\******************************************************************************/

#define SOAP_SMD_NONE		(0)
/** MD5 digest algorithm */
#define SOAP_SMD_DGST_MD5	(1)
/** SHA1 digest algorithm */
#define SOAP_SMD_DGST_SHA1	(2)
/** HMAC-SHA1 shared key signature algorithm */
#define SOAP_SMD_HMAC_SHA1	(3)
/** DSA-SHA1 secret key signature algorithm */
#define SOAP_SMD_SIGN_DSA_SHA1	(4)
/** RSA-SHA1 secret key signature algorithm */
#define SOAP_SMD_SIGN_RSA_SHA1	(5)
/** DSA-SHA1 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_DSA_SHA1	(6)
/** RSA-SHA1 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_RSA_SHA1	(7)

/**
@struct soap_smd_data
@brief The smdevp engine context data
*/
struct soap_smd_data
{ int alg;		/**< The digest or signature algorithm used */
  void *ctx;		/**< EVP_MD_CTX or HMAC_CTX */
  const void *key;	/**< EVP_PKEY */
  int (*fsend)(struct soap*, const char*, size_t);
  size_t (*frecv)(struct soap*, char*, size_t);
  soap_mode mode;
  void *user;
};

/******************************************************************************\
 *
 * soap_smd API functions
 *
\******************************************************************************/

size_t soap_smd_size(int alg, const void *key);

int soap_smd_begin(struct soap *soap, int alg, const void *key, int keylen);
int soap_smd_end(struct soap *soap, char *buf, int *len);

int soap_smd_init(struct soap *soap, struct soap_smd_data *data, int alg, const void *key, int keylen);
int soap_smd_update(struct soap *soap, struct soap_smd_data *data, const char *buf, size_t len);
int soap_smd_final(struct soap *soap, struct soap_smd_data *data, char *buf, int *len);

#ifdef __cplusplus
}
#endif

#endif
