/*
 * Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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


/* ====================================================================
 * ====================================================================
 *
 * Module: vcos.c
 * $Revision: 1.5 $
 * $Date: 04/12/21 14:58:20-08:00 $
 * $Author: bos@eng-25.internal.keyresearch.com $
 * $Source: /home/bos/bk/kpro64-pending/libm/SCCS/s.vcos.c $
 *
 * Revision history:
 *  01-Dec-94 - Original Version
 *
 * Description:	source code for vector cosine function
 *
 * ====================================================================
 * ====================================================================
 */

#include "libm.h"

#if defined(mips) && !defined(__GNUC__)
extern	void	vcos(double *, double *, long, long, long);

#pragma weak vcos = __vcos
#endif

#if defined(BUILD_OS_DARWIN) /* Mach-O doesn't support aliases */
extern void __vcos( double *x, double *y, long count, long stridex,
  long stridey );
#pragma weak vcos
void vcos( double *x, double *y, long count, long stridex, long stridey ) {
  __vcos(x, y, count, stridex, stridey);
}
#elif defined(__GNUC__)
extern  void  __vcos(double *, double *, long, long, long);
void    vcos() __attribute__ ((weak, alias ("__vcos")));
#endif

static const du	rpiby2 =
{D(0x3fe45f30, 0x6dc9c883)};

static const du	piby2hi =
{D(0x3ff921fb, 0x54400000)};

static const du	piby2lo =
{D(0x3dd0b461, 0x1a600000)};

static const du	piby2tiny =
{D(0x3ba3198a, 0x2e037073)};

static const du	Twopm30 =
{D(0x3e100000, 0x00000000)};

static const du	Twop19xpi =
{D(0x413921fb, 0x54442d18)};

/* coefficients for polynomial approximation of sin on +/- pi/2 */

static const du	P[] =
{
{D(0x3ff00000, 0x00000000)},
{D(0xbfc55555, 0x55555548)},
{D(0x3f811111, 0x1110f7d0)},
{D(0xbf2a01a0, 0x19bfdf03)},
{D(0x3ec71de3, 0x567d4896)},
{D(0xbe5ae5e5, 0xa9291691)},
{D(0x3de5d8fd, 0x1fcf0ec1)},
};

/* coefficients for polynomial approximation of cos on +/- pi/2 */

static const du	Q[] =
{
{D(0x3ff00000, 0x00000000)},
{D(0xbfdfffff, 0xffffff96)},
{D(0x3fa55555, 0x5554f0ab)},
{D(0xbf56c16c, 0x1640aaca)},
{D(0x3efa019f, 0x81cb6a1d)},
{D(0xbe927df4, 0x609cb202)},
{D(0x3e21b8b9, 0x947ab5c8)},
};

static const	du	Qnan =
{D(QNANHI, QNANLO)};


/* ====================================================================
 *
 * FunctionName		vcos
 *
 * Description		computes vector cosine of arg
 *
 * ====================================================================
 */

void
__vcos( double	*x, double *y, long count, long stridex, long stridey )
{
long	i;
int	n;
double	dx, arg;
double	w;
double	xsq;
double	result;
double	*p;
double	dn;

	/* i = 0, 1, ..., count-1; y[i*stridey] = cos(x[i*stridex])	*/

	for ( i=0; i<count; i++ )
	{
#ifdef _PREFETCH
#pragma prefetch_ref=*(x+8)
#pragma prefetch_ref=*(y+8)
#endif

		arg = *x;

		dx = arg;

		if ( arg != arg )
			dx = 0.0;

		/* for large args, return 1.0	*/

		if ( fabs(arg) > Twop19xpi.d )
			dx = 0.0;

		/* for small args, return 1.0	*/

		if ( fabs(arg) < Twopm30.d )
			dx = 0.0;

		/*  reduce argument to +/- pi/4  */
	
		w = dx;

		dn = w*rpiby2.d;
	
		n = ROUND(dn);
		dn = n;
	
		dx = dx - dn*piby2hi.d;
		dx = dx - dn*piby2lo.d;
		dx = dx - dn*piby2tiny.d;	/* dx = x - n*pi/2 */
	
		w = dx;

		n++;

		p = (double *)&P[0].d;

		/* if ( n&1 )
			compute cos(dx)
		   else
			compute sin(dx)

		   if ( n&2 )
			negate result
		*/

		if ( n&1 )
		{
			p = (double *)&Q[0].d;
			dx = 1.0;
		}

		xsq = w*w;

		result = (((((p[6]*xsq + p[5])*xsq + p[4])*xsq 
			+ p[3])*xsq + p[2])*xsq + p[1])*(xsq*dx) + dx;

		if ( n&2 )
			result = -result;

		if ( arg != arg )
			result = Qnan.d;

		*y = result;

		x += stridex;
		y += stridey;
	}
}

