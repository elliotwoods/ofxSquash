/* Copyright (c) 2013 The Squash Authors
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *   Evan Nemerson <evan@coeus-group.com>
 */
/* IWYU pragma: private, include <squash/squash.h> */

#ifndef SQUASH_VERSION_H
#define SQUASH_VERSION_H

#if !defined (SQUASH_H_INSIDE) && !defined (SQUASH_COMPILATION)
#error "Only <squash/squash.h> can be included directly."
#endif

#define SQUASH_VERSION_MAJOR 0
#define SQUASH_VERSION_MINOR 8
#define SQUASH_VERSION_REVISION 0
#define SQUASH_VERSION_API "0.8"

#define SQUASH_VERSION(major,minor,revision) (((major) << 16) | ((minor) << 8) | (revision))
#define SQUASH_VERSION_CURRENT SQUASH_VERSION(SQUASH_VERSION_MAJOR, SQUASH_VERSION_MINOR, SQUASH_VERSION_REVISION)

#define SQUASH_VERSION_EXTRACT_MAJOR(version) (((version) >> 16) & 0xff)
#define SQUASH_VERSION_EXTRACT_MINOR(version) (((version) >> 8) & 0xff)
#define SQUASH_VERSION_EXTRACT_REVISION(version) ((version) & 0xff)

SQUASH_BEGIN_DECLS

SQUASH_API unsigned int squash_version     (void);
SQUASH_API const char*  squash_version_api (void);

SQUASH_END_DECLS

#endif /* SQUASH_VERSION_H */
