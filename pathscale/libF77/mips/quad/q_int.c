/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

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


#include "quad.h"

extern	long double __qint(double, double);

	/* intrinsic QINT */

	/* by address version */

	/* truncates a long double, i.e. computes the integral part of
	   a long double
	*/

long double
__q_int( u )
long double *u;
{
ldquad	x;

#include "msg.h"

	x.ld = *u;

	return ( __qint(x.q.hi, x.q.lo) );
}

