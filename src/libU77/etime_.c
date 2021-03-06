/*
 * Copyright (C) 2007. PathScale, LLC. All Rights Reserved.
 */
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

/* $Header: /home/bos/bk/kpro64-pending/libU77/etime_.c 1.5 04/12/21 14:58:06-08:00 bos@eng-25.internal.keyresearch.com $ */
/*
 *
 * Return the elapsed execution time for this process.
 *
 * calling sequence:
 * 	real time(2)
 * 	call etime (time)
 * where:
 * 	the 2 element array, time, will receive the user and system
 * 	elapsed time since the start of execution.
 *
 *	This routine can be called as function, and returns the sum of
 *	user and system times. The time array argument must always be given.
 *
 *	The resolution for all timing is system dependent.
 */

#include <sys/time.h>
#include <sys/resource.h>

#include "pathf90_libU_intrin.h"

/* Bug 3018 */
float
pathf90_etime (float tarray[2])
{	struct rusage ru;

	getrusage (RUSAGE_SELF, &ru);
	tarray[0] = (float)ru.ru_utime.tv_sec
		    + (float)ru.ru_utime.tv_usec * 1e-6;
	tarray[1] = (float)ru.ru_stime.tv_sec
		    + (float)ru.ru_stime.tv_usec * 1e-6;
	return(tarray[0] + tarray[1]);
}

/* Alternate G77 subroutine form */
void
pathf90_subr_etime (float tarray[2], float *result)
{
  *result = pathf90_etime(tarray);
}
