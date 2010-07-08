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


/*  $Header$ */
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	actual or intended publication of such source code.	*/

#include <cmplrs/host.h>
#include "bit.h"

/* Extract a subfield of len bits from m starting with bit position i
 * and extending left for len bits. The right most bit is bit 0.
 */

int64_t
bext_ll (int64_t *m, int64_t *i, int64_t *len)
{
    if ((*i + *len > NBLL) || (*i < 0) || (*len < 0) || (*i >= NBLL) ||
	(*len > NBLL))
	return(*m);
    return ((uint64_t)(*m & F77llmask[*i + *len]) >> *i);
}
