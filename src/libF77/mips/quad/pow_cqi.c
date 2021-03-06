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


#include "cmplrs/host.h"
#include "qcmplx.h"
#include "cq_div.h"

qcomplex __powcqi(long double aqreal, long double aqimag, int32 n)   /* __powcqi = a**n  */
{
  long double t;
  qcomplex x, p;

  p.qreal = 1;
  p.qimag = 0;

  if(n == 0)
    return p;

  if(n < 0) {
    n = -n;
    x = __cqdiv(p.qreal, p.qimag, aqreal, aqimag);
  } else {
    x.qreal = aqreal;
    x.qimag = aqimag;
  }

  for( ; ; ) {
    if(n & 01) {
      t = p.qreal * x.qreal - p.qimag * x.qimag;
      p.qimag = p.qreal * x.qimag + p.qimag * x.qreal;
      p.qreal = t;
    }
    if(n >>= 1) {
      t = x.qreal * x.qreal - x.qimag * x.qimag;
      x.qimag = 2 * x.qreal * x.qimag;
      x.qreal = t;
    } else {
      break;
    }
  }
  return p;
}

qcomplex __powcql(long double aqreal, long double aqimag, int64 n)   /* __powcql = a**n  */
{
  long double t;
  qcomplex x, p;

  p.qreal = 1;
  p.qimag = 0;

  if(n == 0)
    return p;

  if(n < 0) {
    n = -n;
    x = __cqdiv(p.qreal, p.qimag, aqreal, aqimag);
  } else {
    x.qreal = aqreal;
    x.qimag = aqimag;
  }

  for( ; ; ) {
    if(n & 01) {
      t = p.qreal * x.qreal - p.qimag * x.qimag;
      p.qimag = p.qreal * x.qimag + p.qimag * x.qreal;
      p.qreal = t;
    }
    if(n >>= 1) {
      t = x.qreal * x.qreal - x.qimag * x.qimag;
      x.qimag = 2 * x.qreal * x.qimag;
      x.qreal = t;
    } else {
      break;
    }
  }
  return p;
}

void __pow_cqi(qcomplex *p, qcomplex *a, int32 *b)   /* p = a**b  */
{
  *p = __powcqi(a->qreal, a->qimag, *b);
}

void __pow_cql(qcomplex *p, qcomplex *a, int64 *b)   /* p = a**b  */
{
  *p = __powcql(a->qreal, a->qimag, *b);
}
