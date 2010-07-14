/*
 *  Copyright (C) 2006, 2007. QLogic Corporation. All Rights Reserved.
 */

/*
 * Copyright 2002, 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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
 * Module: cgexp.cxx
 * $Revision: 1.23 $
 * $Date: 05/08/25 12:01:03-07:00 $
 * $Author: gautam@jacinth.keyresearch $
 * $Source: be/cg/SCCS/s.cgexp.cxx $
 *
 * History:  extracted target-independent parts from expand.cxx
 *
 * Description:
 *
 * This file contains the internal switch-stmt of code expansion. Its interface
 * is 'Exp_OP', which takes an OP, expands it into a list of OPs which
 * are appended to the oplist passed in.
 *
 *
 * ====================================================================
 * ====================================================================
 */

#include "defs.h"
#include "config.h"
#include "erglob.h"
#include "ercg.h"
#include "glob.h"
#include "tracing.h"
#include "util.h"

#include "topcode.h"
#include "tn.h"
#include "cg_flags.h"
#include "symtab.h"
#include "opcode.h"
#include "op.h"
#include "bb.h"
#include "cgexp.h"
#include "cgexp_internals.h"
#include "w2op.h"

BOOL Trace_Exp = FALSE;	/* General code expansion trace */


/* ====================================================================
 *
 * Expand_OP
 *
 * Given an operator, expand it.  There are two possibilities:
 *
 *  1)	A machine operator requires no further expansion.
 *
 *  2)  An OP_* operator is expanded into a sequence of TOP_* instructions.
 *
 * ====================================================================
 */

static void
Expand_OP (OPCODE opcode, TN *result, TN *op1, TN *op2, TN *op3, VARIANT variant, OPS *ops)
{
  TYPE_ID desc = OPCODE_desc(opcode);
  TYPE_ID rtype = OPCODE_rtype(opcode);

  switch (OPCODE_operator(opcode)) {
  case OPR_TRUEBR:
  case OPR_FALSEBR:
	Expand_Branch (op1, op2, op3, variant, ops);
	break;
  case OPR_GOTO:
  case OPR_GOTO_OUTER_BLOCK:
	Expand_Branch (op1, op2, op3, V_BR_ALWAYS, ops);
	break;
  case OPR_LDA:
	Expand_Lda (result, op1, ops);
	break;
  case OPR_LDA_LABEL:
	Expand_Lda_Label (result, op1, ops);
	break;
  case OPR_INTCONST:
#ifdef TARG_ST
	Expand_Immediate (result, op1, rtype, ops);
#else
	Expand_Immediate (result, op1, TRUE /* is_signed */, ops);
#endif
	break;
  case OPR_CONST:
	Expand_Const (result, op1, rtype, ops);
	break;
  case OPR_ADD:
	if (MTYPE_is_float(rtype))
		Expand_Flop (opcode, result, op1, op2, op3, ops);
	else
		Expand_Add (result, op1, op2, rtype, ops);
	break;
  case OPR_SUB:
	if (MTYPE_is_float(rtype))
		Expand_Flop (opcode, result, op1, op2, op3, ops);
	else
		Expand_Sub (result, op1, op2, rtype, ops);
	break;
  case OPR_NEG:
	Expand_Neg (result, op1, rtype, ops);
	break;
  case OPR_SHL:
	Expand_Shift (result, op1, op2, rtype, shift_left, ops);
	break;
  case OPR_ASHR:
	Expand_Shift (result, op1, op2, rtype, shift_aright, ops);
	break;
  case OPR_LSHR:
	Expand_Shift (result, op1, op2, rtype, shift_lright, ops);
	break;
#ifdef TARG_X8664
  case OPR_RROTATE:
	Expand_Rrotate (result, op1, op2, rtype, desc, ops);
	break;
#endif
  case OPR_ILOADX:
  case OPR_ILOAD:
  case OPR_LDID:
#ifdef TARG_ST
	if ((V_alignment(variant) != V_NONE) && V_misalign(variant)) {
	  Expand_Misaligned_Load ( opcode, result, op1, op2, variant, ops);
	}
	else {
	  // variant might contain an overalignment information
	  Expand_Load (opcode, result, op1, op2, ops, variant);
	}
#else
	if ( V_align_all(variant) != 0 ) {
		Expand_Misaligned_Load ( opcode, result, op1, op2, variant, ops);
	}
	else {
#if defined(TARG_MIPS) || defined(TARG_X8664)
		Expand_Load (opcode, result, op1, op2, ops);
#else
		Expand_Load (opcode, result, op1, op2, variant, ops);
#endif
	}
#endif
	break;
  case OPR_ISTOREX:
  case OPR_ISTORE:
  case OPR_STID:
#ifdef TARG_ST
    if ((V_alignment(variant) != V_NONE) && V_misalign(variant)) {
	  Expand_Misaligned_Store (desc, op1, op2, op3, variant, ops);
	}
	else {
	  // variant might contain an overalignment information
	  Expand_Store (desc, op1, op2, op3, ops, variant);
    }
#else        
	if ( V_align_all(variant) != 0 ) {
		Expand_Misaligned_Store (desc, op1, op2, op3, variant, ops);
	}
	else {
#if defined(TARG_MIPS) || defined(TARG_X8664)
		Expand_Store (desc, op1, op2, op3, ops);
#else
		Expand_Store (desc, op1, op2, op3, variant, ops);
#endif
	}
#endif
	break;
  case OPR_ABS:
	Expand_Abs (result, op1, rtype, ops);
	break;
  case OPR_MPY:
#ifdef TARG_X8664
	if (MTYPE_is_float(rtype) || MTYPE_is_mmx_vector(rtype))
#else
	if (MTYPE_is_float(rtype))
#endif
		Expand_Flop (opcode, result, op1, op2, op3, ops);
	else
#ifdef TARG_ST
		Expand_Multiply (result, rtype, op1, rtype, op2, rtype, ops);
#else
		Expand_Multiply (result, op1, op2, rtype, ops);
#endif
	break;
  case OPR_HIGHMPY:
	Expand_High_Multiply (result, op1, op2, rtype, ops);
	break;
  case OPR_REM:
	Expand_Rem (result, op1, op2, rtype, ops);
	break;
  case OPR_MOD:
	if (MTYPE_is_signed(rtype))
		Expand_Mod (result, op1, op2, rtype, ops);
	else
		// unsigned MOD acts like REM
		Expand_Rem (result, op1, op2, rtype, ops);
	break;
  case OPR_DIV:
	if (MTYPE_is_float(rtype))
		Expand_Flop (opcode, result, op1, op2, op3, ops);
	else
		Expand_Divide (result, op1, op2, rtype, ops);
	break;
  case OPR_DIVREM:
	Expand_DivRem(result, op1, op2, op3, rtype, ops);
	break;
  case OPR_SQRT:
	Expand_Sqrt (result, op1, rtype, ops);
	break;
  case OPR_LNOT:
	Expand_Logical_Not (result, op1, variant, ops);
	break;
  case OPR_LAND:
	Expand_Logical_And (result, op1, op2, variant, ops);
	break;
  case OPR_LIOR:
	Expand_Logical_Or (result, op1, op2, variant, ops);
	break;
  case OPR_BNOT:
	Expand_Binary_Complement (result, op1, rtype, ops);
	break;
  case OPR_BAND:
	Expand_Binary_And (result, op1, op2, rtype, ops);
	break;
  case OPR_BIOR:
	Expand_Binary_Or (result, op1, op2, rtype, ops);
	break;
  case OPR_BXOR:
	Expand_Binary_Xor (result, op1, op2, rtype, ops);
	break;
  case OPR_BNOR:
	Expand_Binary_Nor (result, op1, op2, rtype, ops);
	break;
  case OPR_LT:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Less (result, op1, op2, variant, desc, ops);
	else
		Expand_Int_Less (result, op1, op2, desc, ops);
	break;
  case OPR_LE:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Less_Equal (result, op1, op2, variant, desc, ops);
	else
		Expand_Int_Less_Equal (result, op1, op2, desc, ops);
	break;
  case OPR_EQ:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Equal (result, op1, op2, variant, desc, ops);
	else if (desc == MTYPE_B)
		Expand_Bool_Equal (result, op1, op2, ops);
	else
		Expand_Int_Equal (result, op1, op2, desc, ops);
	break;
  case OPR_NE:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
#ifdef TARG_ST
        if (MTYPE_is_class_pointer(desc)) {
          Expand_Ptr_Not_Equal (result, op1, op2, desc, ops);
        } else
#endif
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Not_Equal (result, op1, op2, variant, desc, ops);
	else if (desc == MTYPE_B)
		Expand_Bool_Not_Equal (result, op1, op2, ops);
	else
		Expand_Int_Not_Equal (result, op1, op2, desc, ops);
	break;
  case OPR_GE:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Greater_Equal (result, op1, op2, variant, desc, ops);
	else
		Expand_Int_Greater_Equal (result, op1, op2, desc, ops);
	break;
  case OPR_GT:
#ifdef TARG_X8664
	if (MTYPE_is_vector(desc))
		Exp_Stid_And_VComp(result, op1, op2, opcode, ops);
	else if (MTYPE_is_float(desc))
#else
	if (MTYPE_is_float(desc))
#endif
		Expand_Float_Greater (result, op1, op2, variant, desc, ops);
	else
		Expand_Int_Greater (result, op1, op2, desc, ops);
	break;

  case OPR_CVTL:
	Expand_Convert_Length ( result, op1, op2, rtype, MTYPE_is_signed(rtype), ops);
	break;
  case OPR_CVT:
#ifdef TARG_ST
	Is_True(rtype != MTYPE_B, ("conversion to bool unsupported"));
	if (MTYPE_is_float(rtype) && MTYPE_is_float(desc)) {
		Expand_Float_To_Float (result, op1, rtype, ops);
	}
	else if (MTYPE_is_float(rtype) && MTYPE_is_signed(desc)) {
		// desc is int
		Expand_Int_To_Float (result, op1, desc, rtype, ops);
	}
	else if (MTYPE_is_float(rtype)) {
		// desc is unsigned int
		Expand_Unsigned_To_Float (result, op1, desc, rtype, ops);
	}
	else if (MTYPE_is_float(desc) && MTYPE_is_signed(rtype)) {
		// rtype is int
	        Expand_Float_To_Int_Cvt (result, op1, rtype, desc, ops);
	}
	else if (MTYPE_is_float(desc)) {
		// rtype is unsigned int
		Expand_Float_To_Unsigned_Cvt (result, op1, rtype, desc, ops);
	} else {
                Expand_Convert (result, op1, op2, rtype, desc, ops);
        }
#else

	Is_True(rtype != MTYPE_B, ("conversion to bool unsupported"));
	if (MTYPE_is_float(rtype) && MTYPE_is_float(desc)) {
#ifdef TARG_X8664
		Expand_Float_To_Float (result, op1, rtype, desc, ops);
#else
		Expand_Float_To_Float (result, op1, rtype, ops);
#endif
	}
	else if (MTYPE_is_float(rtype)) {
		// desc is int
		Expand_Int_To_Float (result, op1, desc, rtype, ops);
	}
	else if (MTYPE_is_float(desc)) {
		// rtype is int
		Expand_Float_To_Int_Cvt (result, op1, rtype, desc, ops);
	}
	else if (desc == MTYPE_B) {
		// desc is bool
		Expand_Bool_To_Int (result, op1, rtype, ops);
	}
#ifdef TARG_X8664
	else if (MTYPE_is_vector (rtype)) {
		// vector conversions not covered above
		Expand_Conv_To_Vector (result, op1, desc, rtype, ops);
	}
	else if (MTYPE_is_vector (desc)) {
		Expand_Conv_From_Vector (result, op1, desc, rtype, ops);
	}
#endif
	else {
		// both are int
  		// only zero-extend when enlarging an unsigned value; 
		// else sign-extend.
		Expand_Convert_Length ( result, op1, op2, 
			rtype, 
			(MTYPE_is_signed(desc)
			|| (MTYPE_bit_size(desc) > MTYPE_bit_size(rtype) ) ),
			ops);
	}
#endif
	break;
#ifdef TARG_X8664
  case OPR_TAS:
	if (MTYPE_is_vector(rtype)) // bugs 11797 11876
	  Expand_Int_To_Vect_Tas(result, op1, rtype, ops);
  	else if (MTYPE_is_integral(rtype))
	  Expand_Float_To_Int_Tas(result, op1, rtype, ops);
	else Expand_Int_To_Float_Tas(result, op1, rtype, ops);
	break;
#endif
  case OPR_RND:
        if(MTYPE_is_signed(rtype)) {
	  Expand_Float_To_Int_Round (result, op1, rtype, desc, ops);
	}
#ifdef MERGED_CODE
	else {
	  Expand_Float_To_Unsigned_Round (result, op1, rtype, desc, ops);
	}
#endif
	break;
  case OPR_TRUNC:
#ifdef TARG_ST
	if (MTYPE_is_float(desc) && MTYPE_is_signed(rtype)) {
		// rtype is int
		Expand_Float_To_Int_Trunc (result, op1, rtype, desc, ops);
	}
	else if (MTYPE_is_float(desc)) {
		// rtype is unsigned int
		Expand_Float_To_Unsigned_Trunc (result, op1, rtype, desc, ops);
	}
#else
	Expand_Float_To_Int_Trunc (result, op1, rtype, desc, ops);
#endif
	break;
  case OPR_CEIL:
       if(MTYPE_is_signed(rtype)) {
	  Expand_Float_To_Int_Ceil (result, op1, rtype, desc, ops);
	}
#ifdef MERGED_CODE
	else {
	  Expand_Float_To_Unsigned_Ceil (result, op1, rtype, desc, ops);
	}
#endif
	break;
  case OPR_FLOOR:
#ifdef TARG_X8664
    if( MTYPE_is_float( rtype ) ){
      if( MTYPE_is_quad( rtype ) )
	Expand_Float_To_Float_Floorl( result, op1, rtype, desc, ops );
      else if( rtype == MTYPE_F8 )
	Expand_Float_To_Float_Floor( result, op1, rtype, desc, ops );
      else
	Expand_Float_To_Float_Floorf( result, op1, rtype, desc, ops );
      break;
    }	
#elif defined (TARG_MIPS)
        if (MTYPE_is_float (rtype)) {
	  Expand_Float_To_Float_Floor (result, op1, rtype, desc, ops);
	  break;
	}
#endif
	Expand_Float_To_Int_Floor (result, op1, rtype, desc, ops);
	break;
  case OPR_MIN:
	Expand_Min (result, op1, op2, rtype, ops);
	break;
  case OPR_MAX:
	Expand_Max (result, op1, op2, rtype, ops);
	break;

  case OPR_MINMAX:
	Expand_MinMax (result, op1, op2, op3, rtype, ops);
	break;

  case OPR_SELECT:
	Expand_Select (result, op1, op2, op3, rtype, V_select_uses_fcc(variant), ops);
	break;

  case OPR_MADD:
  case OPR_NMADD:
  case OPR_MSUB:
  case OPR_NMSUB:
  case OPR_RECIP:
  case OPR_RSQRT:
#ifdef TARG_X8664
  case OPR_ATOMIC_RSQRT:	// bug 6123
#endif
	Expand_Flop (opcode, result, op1, op2, op3, ops);
	break;

#if defined( KEY) && !defined( TARG_ST)
  case OPR_REPLICATE:
        Expand_Replicate (opcode, result, op1, ops);
	break;

  case OPR_REDUCE_ADD:
        Expand_Reduce_Add (opcode, result, op1, ops);
	break;

  case OPR_REDUCE_MPY:
        Expand_Reduce_Mpy (opcode, result, op1, ops);
	break;

  case OPR_REDUCE_MAX:
        Expand_Reduce_Max (opcode, result, op1, ops);
	break;

  case OPR_REDUCE_MIN:
        Expand_Reduce_Min (opcode, result, op1, ops);
	break;

#endif /* KEY */
  default:
	FmtAssert(FALSE, 
		("Expand_OP:  unexpected opcode %s", OPCODE_name(opcode)));
	break;
  }

#ifdef TARG_X8664
  /* For comparsions, the upper 32-bit of the result is ignored.
     Thus under -m32, the value of the higher 32-bit part is undefined.
     (bug#2499)
  */
  if( OPCODE_is_compare( opcode ) &&
      OP_NEED_PAIR( rtype ) ){
    TN* result_hi = Create_TN_Pair( result, rtype );
    Exp_Immediate( result_hi, Gen_Literal_TN(0,4), FALSE, ops );
  }
#endif // TARG_X8664
}


extern void 
Exp_OP (OPCODE opcode, TN *result, TN *op1, TN *op2, TN *op3, VARIANT variant, OPS *ops)
{
  TOP top = OPCODE_To_TOP (opcode);

  if (Trace_Exp) {
	#pragma mips_frequency_hint NEVER
	fprintf(TFile, "expand %s: ", OPCODE_name(opcode)); 
	if (result) Print_TN(result,FALSE);
  	fprintf(TFile, " :- ");
	if (op1) Print_TN(op1,FALSE);
	fprintf(TFile, " ");
	if (op2) Print_TN(op2,FALSE);
	fprintf(TFile, " ");
	if (op3) Print_TN(op3,FALSE);
	fprintf(TFile, " ");
	if (variant) fprintf(TFile, "(0x%" SCNx64 ")", (INT64)variant);
	fprintf(TFile, "\n");
  }

  if (top != TOP_UNDEFINED) {
	TN *args[5];
	OP *op;
	INT i = 0;

	// Build up a list of arguments to Mk_OP. The code here
	// relies on the fact that Mk_OP will ignore unnecessary
	// trailing arguments.
	if (result) args[i++] = result;
	if (TOP_is_predicated(top)) {
#ifdef TARG_ST
      Is_True(TOP_Find_Operand_Use(top, OU_predicate) == 0,
	      ("predicate operand is not 0"));
#else
	  Is_True(OP_PREDICATE_OPND == 0, ("predicate operand is not 0"));
#endif
	  args[i++] = True_TN;
	}
	args[i++] = op1;
	args[i++] = op2;
	args[i] = op3;
	op = Mk_OP(top, args[0], args[1], args[2], args[3], args[4]);
#ifndef TARG_ST
	if (OP_defs_fpu_int(op)) Set_TN_is_fpu_int(result);
#endif
    	/* Add the new OPs to the end of the list passed in */
	OPS_Append_Op(ops, op);
  	if (Trace_Exp) {
		#pragma mips_frequency_hint NEVER
		fprintf(TFile, " into "); Print_OP (op);
	}
  }
  else {
	OPS new_ops;
  	OPS_Init(&new_ops);
  	Expand_OP (opcode, result, op1, op2, op3, variant, &new_ops);
  	if (Trace_Exp) {
		#pragma mips_frequency_hint NEVER
		OP *op;
		FOR_ALL_OPS_OPs (&new_ops, op) {
			fprintf(TFile, " into "); Print_OP (op);
		}
  	}
	/* check if there is at least one OP in the expansion */
	if (OPS_first(&new_ops) != NULL) {
		/* Add the new OPs to the end of the list passed in */
		OPS_Append_Ops(ops, &new_ops);
  	}
  }
}

