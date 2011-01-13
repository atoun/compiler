/*
 * Copyright (C) 2007. PathScale, LLC. All Rights Reserved.
 */
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


#pragma ident "@(#) libfi/matrix/matmul_s4s4.c	92.1	07/09/99 15:18:08"

#include "matmul.h"

#define NAME _MATMUL_S4S4

/*
 * Subroutine called to do computation:
 */
#if defined(UNICOS) || defined(UNIX_NAMING_CONVENTION)
#define SUBNAME1 _S4GEMMX
#define SUBNAME2 _S4GEMVX
#elif defined(BUILD_COMPILER_GNU) && defined(BUILD_OS_DARWIN)
#define SUBNAME1 underscore_s4gemmx_
#define SUBNAME2 underscore_s4gemvx_
#else
#define SUBNAME1 _s4gemmx_
#define SUBNAME2 _s4gemvx_
#endif
/*
 * Type of constants alpha and beta
 */
#define RESULTTYPE	_f_real4

void
NAME(DopeVectorType *RESULT, DopeVectorType *MATRIX_A, 
	DopeVectorType *MATRIX_B)
{
    void    SUBNAME1();
    void    SUBNAME2();
    const RESULTTYPE   one =  (RESULTTYPE) 1.0;
    const RESULTTYPE   zero = (RESULTTYPE) 0.0;
    MatrixDimenType matdimdata, *MATDIM;
	
	MATDIM = (MatrixDimenType *) &matdimdata;

    /*
     * Parse dope vectors, and perform error checking.
     */

    _premult(RESULT, MATRIX_A, MATRIX_B, MATDIM);

    /*
     * Perform the matrix multiplication.
     */

    if (MATDIM->ndimb == 1) {
	/*
	 * y = Ax
	 */
	SUBNAME2(&MATDIM->n1a, &MATDIM->n2a, &one, MATDIM->A, &MATDIM->inc1a,
	   &MATDIM->inc2a, MATDIM->B, &MATDIM->inc1b, &zero, MATDIM->C,
	   &MATDIM->inc1c);
	return;
    } else if (MATDIM->ndima == 1) {
	/*
	 * y = xB, equivalent to y' = B'x'
	 */
	SUBNAME2(&MATDIM->n2b, &MATDIM->n1b, &one, MATDIM->B, &MATDIM->inc2b,
	   &MATDIM->inc1b, MATDIM->A, &MATDIM->inc1a, &zero, MATDIM->C,
	   &MATDIM->inc1c);
	return;
    } else {
	/*
	 * C = AB (full matrix multiplication)
	 */
	SUBNAME1(&MATDIM->n1a, &MATDIM->n2b, &MATDIM->n2a, &one, MATDIM->A,
	   &MATDIM->inc1a, &MATDIM->inc2a, MATDIM->B, &MATDIM->inc1b,
	   &MATDIM->inc2b, &zero, MATDIM->C, &MATDIM->inc1c, &MATDIM->inc2c);
	return;
    }
}
