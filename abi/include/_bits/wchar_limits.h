/*
 * Copyright (c) 2017 CZ.NIC, z.s.p.o.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Authors:
 *	Jiří Zárevúcky (jzr) <zarevucky.jiri@gmail.com>
 */

/** @addtogroup bits
 * @{
 */
/** @file
 */

#ifndef _BITS_WCHAR_LIMITS_H_
#define _BITS_WCHAR_LIMITS_H_

/* wchar_t should always be int32_t in HelenOS. */

#include <_bits/wchar_t.h>

#ifndef __cplusplus
_Static_assert(((wchar_t) -1) < 0, "wchar_t is not int32_t");
_Static_assert(sizeof(wchar_t) == 4, "wchar_t is not int32_t");
#endif

#ifndef WCHAR_MAX
#define WCHAR_MAX  0x7fffffff
#endif
#ifndef WCHAR_MIN
#define WCHAR_MIN  (-WCHAR_MIN - 1)
#endif

#endif

/** @}
 */
