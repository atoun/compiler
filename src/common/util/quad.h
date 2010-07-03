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


/*$Headers: $*/

#ifndef quad_INCLUDE
#define quad_INCLUDE
#ifdef __cplusplus
extern "C" {
#endif


typedef struct { float real, imag; } complex;
typedef struct { double dreal, dimag; } dcomplex;

typedef	struct {
double	hi;
double	lo;
} QUAD;

typedef	struct {
QUAD qreal;
QUAD qimag;
} qcomplex;

#define	DMANTWIDTH	52

#define	DBL2LL(x, l)	l = *(INT64 *)&x
#define	LL2DBL(l, x)	x = *(double *)&l

#ifdef __cplusplus
}
#endif
#endif /* quad_INCLUDE */
