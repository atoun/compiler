/*
 * Copyright 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

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


/*  $Header: /home/bos/bk/kpro64-pending/libF77/bittab.c 1.5 04/12/21 14:58:01-08:00 bos@eng-25.internal.keyresearch.com $ */
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	actual or intended publication of such source code.	*/

/* mask[k] asserts right most k bits of a long. When used as a mask
 * (i.e. A & mask[k]) the bits to the left of the k bits are cleared.
 */

#include <cmplrs/host.h>

int32_t F77mask[33] = {
	0x0,
	0x1, 0x3, 0x7, 0xf,
	0x1f, 0x3f, 0x7f, 0xff,
	0x1ff, 0x3ff, 0x7ff, 0xfff,
	0x1fff, 0x3fff, 0x7fff, 0xffff,
	0x1ffff, 0x3ffff, 0x7ffff, 0xfffff,
	0x1fffff, 0x3fffff, 0x7fffff, 0xffffff,
	0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
	0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
	};

int64_t F77llmask[65] = {
	0x0LL,
	0x1LL, 0x3LL, 0x7LL, 0xfLL,
	0x1fLL, 0x3fLL, 0x7fLL, 0xffLL,
	0x1ffLL, 0x3ffLL, 0x7ffLL, 0xfffLL,
	0x1fffLL, 0x3fffLL, 0x7fffLL, 0xffffLL,
	0x1ffffLL, 0x3ffffLL, 0x7ffffLL, 0xfffffLL,
	0x1fffffLL, 0x3fffffLL, 0x7fffffLL, 0xffffffLL,
	0x1ffffffLL, 0x3ffffffLL, 0x7ffffffLL, 0xfffffffLL,
	0x1fffffffLL, 0x3fffffffLL, 0x7fffffffLL, 0xffffffffLL,
	0x1ffffffffLL, 0x3ffffffffLL, 0x7ffffffffLL, 0xfffffffffLL,
	0x1fffffffffLL, 0x3fffffffffLL, 0x7fffffffffLL, 0xffffffffffLL,
	0x1ffffffffffLL, 0x3ffffffffffLL, 0x7ffffffffffLL, 0xfffffffffffLL,
	0x1fffffffffffLL, 0x3fffffffffffLL, 0x7fffffffffffLL, 0xffffffffffffLL,
	0x1ffffffffffffLL, 0x3ffffffffffffLL, 0x7ffffffffffffLL, 0xfffffffffffffLL,
	0x1fffffffffffffLL, 0x3fffffffffffffLL, 0x7fffffffffffffLL, 0xffffffffffffffLL,
	0x1ffffffffffffffLL, 0x3ffffffffffffffLL, 0x7ffffffffffffffLL, 0xfffffffffffffffLL,
	0x1fffffffffffffffLL, 0x3fffffffffffffffLL, 0x7fffffffffffffffLL, 0xffffffffffffffffLL
	};


/* zmask[k] asserts k+1 right most bits. */

int32_t *F77zmask = &F77mask[1];
int64_t *F77llzmask = &F77llmask[1];
