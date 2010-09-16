/*
 * Copyright (C) 2006. QLogic Corporation. All Rights Reserved.
 */

/* 
   Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
   File modified June 20, 2003 by PathScale, Inc. to update Open64 C/C++ 
   front-ends to GNU 3.2.2 release.
 */

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
 *
 * Module: erfe.h
 * $Revision: 1.6 $
 * $Date: 05/06/21 17:21:29-07:00 $
 * $Author: gautam@jacinth.keyresearch $
 * $Source: kgccfe/SCCS/s.erfe.h $
 *
 * Revision history:
 *  12-Sep-89 - Original Version
 *  11-Mar-91 - Copied for TP/Muse
 *
 * Description:
 *
 * Define the compiler front end error codes for use with the error
 * message handler errors.c.  The associated error descriptors may be
 * found in the file erfe.desc.
 *
 * ====================================================================
 * ====================================================================
 */


#ifndef erfe_INCLUDED
#define erfe_INCLUDED

#ifdef _KEEP_RCS_ID
static char *erfe_rcs_id = "$Source: kgccfe/SCCS/s.erfe.h $ $Revision: 1.6 $";
#endif /* _KEEP_RCS_ID */

#include "errors.h"	/* Always needed */

/* The error codes in each erxxx.h file should start from some multiple
 * of 1000, which is the phase number.
 */
#define EC_BASE_FE	EP_FE*1000

#define EC_Not_Same_Type EC_BASE_FE+0		/* str */
#define EC_Not_Typed	EC_BASE_FE+1		/* str */
#define EC_Dup_Decl	EC_BASE_FE+2		/* str */
#define EC_Not_Class	EC_BASE_FE+3		/* str */
#define EC_Undeclared	EC_BASE_FE+4		/* str */
#define EC_Intcon_Expected EC_BASE_FE+5		/* str */
#define EC_Fltcon_Expected EC_BASE_FE+6		/* str */
#define EC_Return_Num	EC_BASE_FE+7		/* int, int */
#define EC_Return_Type	EC_BASE_FE+8		/* str */
#define EC_Missing_Spec EC_BASE_FE+9		/* str */
#define EC_Sets_Num	EC_BASE_FE+10		/* int, int */
#define EC_Sets_Type	EC_BASE_FE+11		/* str */
#define EC_Skip_Stmt    EC_BASE_FE+12		/* str */
#define EC_Spec_Prev	EC_BASE_FE+13		/* str */
#define EC_Int_Expected EC_BASE_FE+14		/* str,str */
#define EC_Spec_Lib	EC_BASE_FE+15		/* str */
#define EC_Invalid_Regno EC_BASE_FE+16		/* str, int */
#define EC_GE_Only	EC_BASE_FE+20		/* str */
#define EC_PE_Only	EC_BASE_FE+21		/* str */
#define EC_Diff_Decl	EC_BASE_FE+22		/* str */
#define EC_Flag_Var	EC_BASE_FE+23		/* stab */
#ifdef TARG_ST
#define EC_Invalid_Asm	EC_BASE_FE+24		/* str */
#define EC_Unused_Pragma EC_BASE_FE+25		/* str, str, str */
#define EC_Pragma_Override EC_BASE_FE+26	/* str, str, str, str */
#define EC_Pragma_Override_All EC_BASE_FE+27	/* str, str */
#define EC_Malformed_Pragma EC_BASE_FE+28	/* str */
#define EC_Obsolete_Pragma EC_BASE_FE+29	/* str,str */
#define EC_Pragma_Undefined_Extension EC_BASE_FE+30 /* str,str */
#define EC_BitField_Init  EC_BASE_FE+31         /* str */
#define EC_Common_Alias  EC_BASE_FE+32          /* str */
#define EC_Asm_Constraint_Operand  EC_BASE_FE+33 /* str, int */
#define EC_VisSpec_File  EC_BASE_FE+34          /* str */
#define EC_Asm_Constraint_Result EC_BASE_FE+35  /* str, int */
#endif


#if defined(FRONT_END_C) || defined(FRONT_END_CPLUSPLUS) || \
    defined(FRONT_END_FORTRAN)
#define EC_Null_Backptr	EC_BASE_FE+100		/* tree,str */
#endif /* FRONT_END_C || FRONT_END_CPLUSPLUS || FRONT_END_FORTRAN */

#ifdef KEY
#define EC_Unsupported_Type EC_BASE_FE+105	/* str */
#endif

#endif /* erfe_INCLUDED */
