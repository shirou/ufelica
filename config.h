/*	$AoiMoe: config.h,v 1.6 2004/01/04 14:30:31 tshiozak Exp $	*/

/*-
 * Copyright (c)2003 Takuya SHIOZAKI <tshiozak@bsdclub.org>,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#if defined(__NetBSD__)
#include <sys/endian.h>
#define O_BINARY	(0)

#elif defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/mchain.h>
#define htole16 htoles
#define htole32 htolel
#define le16toh letohs
#define le32toh letohl
#define __packed __attribute__((__packed__))
#define O_BINARY	(0)

#elif defined(linux)
#include <asm/byteorder.h>
#define htole16 __cpu_to_le16
#define htole32 __cpu_to_le32
#define le16toh __le16_to_cpu
#define le32toh __le32_to_cpu
#define __packed __attribute__((__packed__))
#define getprogname() "ugbabtc"
#define O_BINARY	(0)

#elif defined(_WIN32) && defined(_MSC_VER)
#define EOPNOTSUPP	EINVAL	/* XXX */
#define __packed __declspec(align(1))
#define getprogname() "ugbabtc"
#define htole16(x)	((uint16_t)(x))
#define htole32(x)	((uint32_t)(x))
#define le16toh(x)	((uint16_t)(x))
#define le32toh(x)	((uint32_t)(x))

#elif defined(__APPLE_CC__)
#include <architecture/byte_order.h>
#define htole16 NXSwapHostShortToLittle
#define htole32 NXSwapHostLongToLittle
#define le16toh NXSwapLittleShortToHost
#define le32toh NXSwapLittleLongToHost
#define __packed __attribute__((__packed__))
#define O_BINARY        (0)

#endif

/* cdecl alternative */
#ifndef BEGIN_DECLS_
#ifdef __cplusplus
#define BEGIN_DECLS_	extern "C" {
#define END_DECLS_	}
#else
#define BEGIN_DECLS_
#define END_DECLS_
#endif
#endif

#endif
