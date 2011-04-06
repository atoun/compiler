/*
 * Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/*

  Copyright (C) 2000, 2001, Silicon Graphics, Inc.  All Rights Reserved.

   Path64 is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation version 2.1

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



#pragma ident "@(#) libf/fio/fcleanup.c	92.1	06/18/99 16:08:47"

#include <inttypes.h>
#include <errno.h>
#include <foreign.h>
#include <stdlib.h>
#include <liberrno.h>
#include "fio.h"
#include <inttypes.h>
int _print_statistics;

/*
 * 	_fcleanup - closes all connected Fortran units except 100, 101,
 *	and 102.
 *
 *	This routine aborts when errors are detected after printing error 
 *	messages for each error encountered.
 *
 *	This routine is called from exit() after all other active tasks have
 *	been terminated.
 */

void
_fcleanup(void)
{
	register int	ret;
	register short	errflag;
	static short	pass = 0; /* incremented when _fcleanup is called */
	unit		*uptr;

	if (pass++ >= 1) return;

	errflag	= 0; 

/*
 *	Find all open Fortran units not connected by WOPEN/OPENMS/OPENDR/AQOPEN
 *	and close them.
 */
	uptr	= _get_next_unit(NULL, 0, 0);

	while (uptr != NULL) {     /* while more open units */
		register unum_t	unum;
		
		unum	= uptr->uid;
		
		if (OPEN_UPTR(uptr) && uptr->ufs != FS_AUX) {
			ret	= _unit_close(uptr, CLST_UNSPEC, NULL);
			if (ret != 0) {
				char	msgbuf[80]; 

				if (!_is_file_name(uptr->uid)) {
					sprintf(msgbuf,
		    "FATAL error closing unit %"PRId64" during program termination",
						unum);
				}
				else {
					sprintf(msgbuf,
	     "FATAL error closing a Hollerith unit during program termination");
				}
				_lmessage(ret, msgbuf, NULL);
				errflag	= 1;
			}
		}
		uptr	= _get_next_unit(uptr, 0, 0);
	}

/*
 *	Flush C files here for two reasons:
 *		1) On Solaris, the C cleanup routine will not be executed if
 *		   the code is loaded using the f90 command.   So Fortran 
 *		   termination processing must flush stdout and any user C 
 *		   files in addition to the Fortran files.
 *		2) When job-end "mtused" statistics are requested, the accuracy
 *		   of the statistics is improved by flushing as many files 
 *		   as possible.
 */	 
	if (fflush(NULL) == EOF)
		errflag	= 1;

	if (errflag)
		(void) abort();

#ifdef	_CRAY1
	if (_print_statistics)
                _mtcpu();
#endif

	return;
}

#if	defined(__mips) || defined(_LITTLE_ENDIAN)
/* On Irix 6.2 and 6.5, we see that _cleanup, the libc stdio cleanup routine,
 * is being called by each thread as it exits.  But, it is unlocked. 
 * Also, _cleanup is called before any of the routines registered through
 * atexit() are called.
 * So, we can have 2 or more threads in there at once, and we can get
 * duplicated output.  This routine, which is called at each thread's
 * exit before _cleanup, does an fflush(NULL).
 * Note that there are still possible problems:
 *  It's possible that another thread could already be in a stdio routine
 *  when this routine is called.
 */

plock_t	_fclock;
void
_fortclean(void)
{
	static volatile int beenhere = 0;

	MEM_LOCK(&_fclock);

	if (beenhere) {
		MEM_UNLOCK(&_fclock);
		return;
	}

	beenhere++;
	fflush(NULL);
	MEM_UNLOCK(&_fclock);

	return;
}
#endif
