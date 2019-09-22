/*
 * Copyright (c) 2011 Petr Koupy
 * Copyright (c) 2011 Jiri Zarevucky
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

/** @addtogroup libposix
 * @{
 */
/** @file Standard library definitions.
 */

#ifndef POSIX_STDLIB_H_
#define POSIX_STDLIB_H_

#include <libc/stdlib.h>
#include <sys/types.h>

#include <stddef.h>

__C_DECLS_BEGIN;

/* Environment Access */
extern int putenv(char *string);

/* Symbolic Links */
extern char *realpath(const char *__restrict__ name, char *__restrict__ resolved);

/* Floating Point Conversion */
extern double atof(const char *nptr);
extern float strtof(const char *__restrict__ nptr, char **__restrict__ endptr);
extern double strtod(const char *__restrict__ nptr, char **__restrict__ endptr);

/* Temporary Files */
extern int mkstemp(char *tmpl);

/* Legacy Declarations */
extern char *mktemp(char *tmpl) __attribute__((deprecated));
extern int bsd_getloadavg(double loadavg[], int nelem);

__C_DECLS_END;

#endif  // POSIX_STDLIB_H_

/** @}
 */
