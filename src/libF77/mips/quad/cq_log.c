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


/* ====================================================================
 * ====================================================================
 *
 * Module: cq_log.c
 *
 * Description:	source code for complex quad precision log function
 *
 * ====================================================================
 * ====================================================================
 */

#include "quad.h"

	/* intrinsic CQLOG */

/* ====================================================================
 *
 * FunctionName		cq_log
 *
 * Description		computes complex log of arg
 *
 * ====================================================================
 */

qcomplex __cqlog(long double zqreal, long double zqimag)
{
  qcomplex result;

  result.qimag = __qatan2(zqimag, zqreal);
  result.qreal = __qlog(__qhypot(zqreal, zqimag));
  return result;
}

void __cq_log(qcomplex *result, qcomplex *z)
{
  *result = __cqlog(z->qreal, z->qimag);
}
