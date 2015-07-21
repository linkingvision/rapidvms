/*
    This file is part of libmicrospdy
    Copyright Copyright (C) 2012 Andrey Uzunov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file compression.h
 * @brief  zlib handling functions
 * @author Andrey Uzunov
 */

#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "platform.h"

/* size of buffers used by zlib on (de)compressing */
#define SPDYF_ZLIB_CHUNK 16384


/**
 * Initializes the zlib stream for compression. Must be called once
 * for a session on initialization.
 *
 * @param strm Zlib stream on which we work
 * @return SPDY_NO if zlib failed. SPDY_YES otherwise
 */		
int
SPDYF_zlib_deflate_init(z_stream *strm);


/**
 * Deinitializes the zlib stream for compression. Should be called once
 * for a session on cleaning up.
 *
 * @param strm Zlib stream on which we work
 */	
void
SPDYF_zlib_deflate_end(z_stream *strm);


/**
 * Compressing stream with zlib.
 *
 * @param strm Zlib stream on which we work
 * @param src stream of the data to be compressed
 * @param src_size size of the data
 * @param data_used the number of bytes from src_stream that were used
 * 					TODO do we need
 * @param dest the resulting compressed stream. Should be NULL. Must be
 * 					freed later manually.
 * @param dest_size size of the data after compression
 * @return SPDY_NO if malloc or zlib failed. SPDY_YES otherwise
 */
int
SPDYF_zlib_deflate(z_stream *strm,
					const void *src,
					size_t src_size,
					size_t *data_used,
					void **dest,
					size_t *dest_size);
     

/**
 * Initializes the zlib stream for decompression. Must be called once
 * for a session.
 *
 * @param strm Zlib stream on which we work
 * @return SPDY_NO if zlib failed. SPDY_YES otherwise
 */	                 
int
SPDYF_zlib_inflate_init(z_stream *strm);


/**
 * Deinitializes the zlib stream for decompression. Should be called once
 * for a session on cleaning up.
 *
 * @param strm Zlib stream on which we work
 */	
void
SPDYF_zlib_inflate_end(z_stream *strm);


/**
 * Decompressing stream with zlib.
 *
 * @param strm Zlib stream on which we work
 * @param src stream of the data to be decompressed
 * @param src_size size of the data
 * @param dest the resulting decompressed stream. Should be NULL. Must
 * 				be freed manually.
 * @param dest_size size of the data after decompression
 * @return SPDY_NO if malloc or zlib failed. SPDY_YES otherwise. If the
 * 			function fails, the SPDY session must be closed
 */
int
SPDYF_zlib_inflate(z_stream *strm,
					const void *src,
					size_t src_size,
					void **dest,
					size_t *dest_size);

#endif
