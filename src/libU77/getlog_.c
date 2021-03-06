/*
 * Copyright 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/*

  Copyright (C) 1999-2001, Silicon Graphics, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2.1 of the GNU Lesser General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  Further, any
  license provided herein, whether implied or otherwise, is limited to 
  this program in accordance with the express provisions of the 
  GNU Lesser General Public License.  

  Patent licenses, if any, provided herein do not apply to combinations 
  of this program with other product or programs, or any other product 
  whatsoever.  This program is distributed without any warranty that the 
  program is delivered free of the rightful claim of any third person by 
  way of infringement or the like.  

  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston MA 02111-1307, USA.

*/

/* $Header: libU77/getlog_.c 1.6 05/08/16 00:09:56-07:00 scorrell@soapstone.internal.keyresearch.com $ */
/*
 *
 * get login name of user
 *
 * calling sequence:
 *	character*8 getlog, name
 *	name = getlog()
 * or
 *	call getlog (name)
 * where:
 *	name will receive the login name of the user, or all blanks if
 *	this is a detached process.
 */

#include <unistd.h>
#include "externals.h"

#ifdef KEY /* Bug 1683, 5019 */

#if HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <stdlib.h>

#include "pathf90_libU_intrin.h"

void
pathf90_getlog(char *name, int len)
{
	char *l = alloca(len + 1);
#ifdef __sun
    char *res = getlogin_r(l, len + 1);
	int err = res == NULL;
#else
	int err = getlogin_r(l, len + 1);
#endif

	b_char((err == 0) ?l:" ", name, len);
}

#else

extern void
getlog_(char *name, int len)
{
	char *l = getlogin();

	b_char(l?l:" ", name, len);
}

#endif /* KEY Bug 1683, 5019 */
