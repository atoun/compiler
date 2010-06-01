/*
Copyright 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.

   Path64 is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   Path64 is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with Path64; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   Special thanks goes to SGI for their continued support to open source
*/
#ifdef KEY /* Bug 1683 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#if defined(BUILD_OS_DARWIN)
#include <stdlib.h>
#else /* defined(BUILD_OS_DARWIN) */
#include <malloc.h>
#endif /* defined(BUILD_OS_DARWIN) */
#include "cmplrs/f_errno.h"
#include <sys/param.h>
#include "externals.h"

#include "pathf90_libU_intrin.h"

/* Provide this for backward compatibility, and in case somebody declares
 * it "external" in Fortran but expects to get it from the library instead
 * of defining it themselves.
 */
extern int32_t
chmod_ (char *name, char *mode, int32_t namlen, int32_t modlen) {
  return pathf90_chmod(name, mode, 0, namlen, modlen);
  }

#endif /* KEY Bug 1683 */
