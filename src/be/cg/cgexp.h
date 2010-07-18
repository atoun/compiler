/*
 *  Copyright (C) 2007. QLogic Corporation. All Rights Reserved.
 */

/*
 * Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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
 * Module: cgexp.h
 * $Revision: 1.24 $
 * $Date: 05/08/29 17:13:33-07:00 $
 * $Author: tkong@hyalite.keyresearch $
 * $Source: be/cg/SCCS/s.cgexp.h $
 *
 * Description:
 * ------------
 * This module exports a functional interface for the code expansion 
 * phase of the code generator. It provides a set of routines to 
 * expand high level operations into a sequence of machine level 
 * instructions.
 *
 * It has several distinct clients:
 *
 * 1. The translation of WHIRL into machine instructions (whirl2ops.c)
 * 2. Spill/restore generation during register allocation (cg_spill.c)
 * 3. Generation of new instructions (SWP, entry-exit, CGPREP).
 *
 * Initialization:
 * ---------------
 * void Init_CG_Expand (void)
 *      Intitialize things for the cgexp phase at the start of each PU.
 *
 * Overall Structure:
 * ------------------
 * All the code expansion routines have a common OPS argument.
 * The new OPs will be added to this OPS list.
 *
 * All the code expansion routines have the prefix "Exp_". 
 * There are internal routines with the prefix Expand_,
 * but those should not be used outside of cgexp.
 * ======================================================================
 */


#ifndef cgexp_INCLUDED
#define cgexp_INCLUDED

#include "variants.h"
#include "cgtarget.h"
#include "cgexp_internals.h"

extern void Init_CG_Expand (void);	/* setup trace flags */


/* ======================================================================
 * Exp_OP0 / Exp_OP1 / Exp_OP2 / Exp_OP3
 *
 * All these routines create an OP by putting together the opcode 'opr',
 * the result 'r' and the operands. The resulting OP is then expanded
 * into machine OPs which are appended to the OPS.
 * ======================================================================*/
/* Expand 'op' and append it to <ops>.*/
/* <ops> MUST NOT BE IN A BB. */
extern void Exp_OP (OPCODE opcode, TN *result, TN *op1, TN *op2, TN *op3, 
		    VARIANT variant, OPS *ops);
#define Exp_OP0(c,r,ops)        	Exp_OP(c,r,NULL,NULL,NULL,V_NONE,ops)
#define Exp_OP1(c,r,o1,ops)     	Exp_OP(c,r,o1,NULL,NULL,V_NONE,ops)
#define Exp_OP1v(c,r,o1,v,ops)     	Exp_OP(c,r,o1,NULL,NULL,v,ops)
#define Exp_OP2(c,r,o1,o2,ops)  	Exp_OP(c,r,o1,o2,NULL,V_NONE,ops)
#define Exp_OP2v(c,r,o1,o2,v,ops)	Exp_OP(c,r,o1,o2,NULL,v,ops)
#define Exp_OP3(c,r,o1,o2,o3,ops)	Exp_OP(c,r,o1,o2,o3,V_NONE,ops)
#define Exp_OP3v(c,r,o1,o2,o3,v,ops)	Exp_OP(c,r,o1,o2,o3,v,ops)


/* Generate code to load the address of 'sym'+'ofst' into 'tgt_tn'. 
 * The 'call_opr' parameter indicates whether the LDA is for a call.
 */
extern void Exp_Lda (
  TYPE_ID mtype, TN *tgt_tn, ST *sym, INT64 ofst, OPERATOR call_opr, OPS *ops);

/* Generate code to load memory location 'sym'+'ofst' into 'tgt_tn',
 * with rtype & desc explicitly given
 */
extern void Exp_Load (
  TYPE_ID rtype, TYPE_ID desc, TN *tgt_tn, ST *sym, INT64 ofst, OPS *ops, 
  VARIANT variant);

/* Generate code to store 'src_tn' into the memory location 'sym'+'ofst'. */
extern void Exp_Store (
  TYPE_ID mtype, TN *src_tn, ST *sym, INT64 ofst, OPS *ops, 
  VARIANT variant);

/* Generate a copy from 'src_tn' to 'tgt_tn'. */
#ifdef TARG_X8664
extern void Exp_COPY (TN *tgt_tn, TN *src_tn, OPS *ops, BOOL copy_pair=FALSE); 
#else
extern void Exp_COPY (TN *tgt_tn, TN *src_tn, OPS *ops); 
#endif

#ifdef TARG_ST
extern void Exp_Enable_Allocate_Object(BOOL onoff);
#endif

#if defined(TARG_X8664) || defined(TARG_MIPS)
/* Generate a copy from 'src_tn' to 'tgt_tn' with zero/sign extension. */
extern void Exp_COPY_Ext (TOP opcode, TN *tgt_tn, TN *src_tn, OPS *ops); 
#endif /* TARG_X8664 or TARG_MIPS */

/* Given a simulated <op>, expand it into the sequence of instructions
 * that must be generated. The <pc_value> is the PC location of the 
 * <op>. It is used to generate offsets for branches in the sequence 
 * of instructions generated.
 */
#ifdef TARG_ST
extern void Exp_Simulated_Op (const OP *op, OPS *ops, INT pc_value, ST **sym);
#else
extern void Exp_Simulated_Op (const OP *op, OPS *ops, INT pc_value);
#endif

/* For the given simulated <op>, return the number of 
 * instructions/instruction-words that will be generated after expansion.
 */
extern INT Simulated_Op_Real_Ops (const OP *op);
extern INT Simulated_Op_Real_Inst_Words (const OP *op);

/* Initial expansion of intrinsic call (may not be complete lowering).
 * return result TN (if set).
 * if creates a loop, then returns loop ops and label for new bb.
 */
#ifdef TARG_ST
/* JV: Use same kind of interface for Exp_Intrinsic_Op and Exp_Intrinsic_Call */
extern void Exp_Intrinsic_Call (INTRINSIC id, INT num_results, INT num_opnds, TN **result, TN **opnd,
				OPS *ops, LABEL_IDX *label, OPS *loop_ops, SRCPOS srcpos);
#else
extern TN * Exp_Intrinsic_Call (
  INTRINSIC id, TN *op0, TN *op1, TN *op2, OPS *ops, 
  LABEL_IDX *label, OPS *loop_ops);
#endif

#ifdef TARG_X8664
/* Expansion of INTRN_SAVEXMMS into TOP_savexmms pseudo instruction */
extern void Exp_Savexmms_Intrinsic(WN *intrncall, TN *rax_tn, LABEL_IDX *label, 
				   OPS *ops);

extern void Exp_Landingpadentry_Intrinsic (ST *dest1, ST *dest2, OPS* ops);

extern void Exp_Fetch_and_And( TN*, TN*, TYPE_ID, OPS* );
extern void Exp_Fetch_and_Or( TN*, TN*, TYPE_ID, OPS* );
extern void Exp_Fetch_and_Xor( TN*, TN*, TYPE_ID, OPS* );
extern TN*  Exp_Compare_and_Swap( TN*, TN*, TN*, TYPE_ID, OPS* );

/* expand intrinsic op */
extern void Exp_Intrinsic_Op (
        INTRINSIC id, TN *result, TN *op0, TN *op1, TN *op2, TN *op3, TN *op4, 
        TYPE_ID mtype, OPS *ops
        );

#else 

/* expand intrinsic op */
#ifdef TARG_ST
/* Arthur: this is because I may have to generate them for Lai */
extern void Exp_Intrinsic_Op (INTRINSIC id, INT num_results, INT num_opnds, TN **result, TN **opnd, OPS *ops, SRCPOS scrpos, BB* curbb);
#else
extern void Exp_Intrinsic_Op (INTRINSIC id, TN *result, TN *op0, TN *op1, TN *op2, OPS *ops);
#endif
#endif

/* Expand TN(const) into a sequence of ops (used in prolog)
 */
extern void Exp_Immediate (TN *dest, TN *src, BOOL is_signed, OPS *);

/* create add/sub/mpy op of given mtype */
#define Exp_ADD(mtype,dest,src1,src2,ops)	\
	Exp_OP2 (OPCODE_make_op(OPR_ADD,mtype,MTYPE_V), dest,src1,src2,ops)
#define Exp_SUB(mtype,dest,src1,src2,ops)	\
	Exp_OP2 (OPCODE_make_op(OPR_SUB,mtype,MTYPE_V), dest,src1,src2,ops)
#define Exp_MPY(mtype,dest,src1,src2,ops)	\
	Exp_OP2 (OPCODE_make_op(OPR_MPY,mtype,MTYPE_V), dest,src1,src2,ops)
#define Exp_BAND(mtype,dest,src1,src2,ops)	\
	Exp_OP2 (OPCODE_make_op(OPR_BAND,mtype,MTYPE_V), dest,src1,src2,ops)

/* check whether to eval the condition separate from the select */
extern BOOL Check_Select_Expansion (OPCODE compare);

#ifdef TARG_X8664
/* create storing vectorized comparison to a preg */
extern void Exp_Stid_And_VComp (
        TN *result, TN *cmp_kid1, TN *cmp_kid2,
        OPCODE compare, OPS *ops);

/* create select and Vector preg as condition for select */
extern void Exp_Select_And_VLdid (
        OPCODE select, TN *result, TN *true_tn, TN *false_tn,
        OPCODE compare, TN *vldid, OPS *ops);
#endif

/* create select and condition for select */
extern void Exp_Select_And_Condition (
        OPCODE select, TN *result, TN *true_tn, TN *false_tn,
        OPCODE compare, TN *cmp_kid1, TN *cmp_kid2, VARIANT variant, OPS *ops);


/*
 * By default, when we expand multi-instruction sequences, we use a new
 * temporary TN for each intermediate result.  This simplifies things for
 * CGPREP.  But in some case like spill code and and -O0 code, it is more
 * efficient to reuse the same TN for intermediate results.  We reuse the
 * TNs when this flag, Reuse_Temp_TNs, is TRUE.
 */
extern BOOL Reuse_Temp_TNs;
#define Get_Temp_TN(tn)	(Reuse_Temp_TNs ? tn : Build_TN_Like(tn))

/* Expand a local (to a BB) jump, choosing between a PIC (longer) sequence
 * and an absolute (shorter) sequence depending on the compilation mode.
 * A jump implies the ability to jump long distances.
 */
extern void Exp_Local_Jump(BB *bb, INT64 offset, OPS *ops);


/*
 * determine if a given symbol is a stack relative reference that will
 * require multiple instructions to load or store.
 */
extern BOOL Exp_Is_Large_Stack_Sym(ST* sym, INT64 ofst);

/* generate prefetch inst */
extern void Exp_Prefetch (TOP opc, TN *src1, TN *src2, VARIANT variant, OPS *ops);

/* generate instructions to extract a bit field */
extern void Exp_Extract_Bits (TYPE_ID rtype, TYPE_ID desc, UINT bit_offset,
			      UINT bit_size, TN *tgt_tn, TN *src_tn, OPS *ops);

/* generate instructions to deposit value in a bit field */
extern void Exp_Deposit_Bits (TYPE_ID rtype, TYPE_ID desc, 
			      UINT bit_offset, UINT bit_size, 
			      TN *tgt_tn, TN *src1_tn, TN *src2_tn, OPS *ops);
#ifdef TARG_X8664
/* generate instructions to set a bit field to all 1's */
extern void Exp_Set_Bits (TYPE_ID rtype, TYPE_ID desc,
			      UINT bit_offset, UINT bit_size, 
			      TN *tgt_tn, TN *src1_tn, OPS *ops);
#endif
/* generate instructions to rotate-left a bit field */
extern void Exp_LRotate (TYPE_ID rtype,
			 TN *tgt_tn, TN *src1_tn, TN *src2_tn, OPS *ops);

/* generate instructions to rotate-right a bit field */
extern void Exp_RRotate (TYPE_ID rtype,
			 TN *tgt_tn, TN *src1_tn, TN *src2_tn, OPS *ops);

/* expand return instruction */
#ifdef TARG_X8664
extern void Exp_Return (TN *return_address, int sp_adjust, OPS *ops);
#else
extern void Exp_Return (TN *return_address, OPS *ops);
#endif
extern void Exp_Return_Interrupt (TN *return_address, OPS *ops);

/* expand call instruction */
extern void Exp_Call (OPERATOR opr, TN *return_address, TN *target, OPS *ops);

/* expand indirect branch */
extern void Exp_Indirect_Branch (TN *targ_reg, OPS *ops);

/* expand no-op */
extern void Exp_Noop (OPS *ops);

/* build spadjust op */
extern void Exp_Spadjust (TN *dest, TN *size, VARIANT variant, OPS *ops);

/* generate predicate calculation op given an existing comparison operator.
 * this could be a branch op, particularly for MIPS.
 */
extern void Exp_Pred_Calc(TN* result, OP* cmp_op, COMPARE_TYPE ctype,
			  BOOL false_result, OPS* ops);
/* expand GP initialization */
extern void Exp_GP_Init (TN *dest, ST *st, OPS *ops);


/* 
 *  Generate a generic 2-result predicate operation.
 *  COMPARE_type_or sets result1 and result2 true if qual_pred is true
 *  COMPARE_type_and sets result1 and result2 false if qual_pred is true
 */
extern void Exp_Generic_Pred_Calc(TN* result1, TN *result2, COMPARE_TYPE ctype,
				  TN *qual_pred, OPS *ops);

/*
 * Used by if conversion. See expand.cxx for details.
 */
extern void Exp_True_False_Preds_For_Block(BB *bb,
					   TN* &true_tn, TN * &false_tn);
#ifdef KEY
/*
 * Used to reinitilalize Predicate TNs for HB 
 */
extern void HB_Reinit_Pred ();
#endif

#ifdef TARG_X8664
extern void Expand_Start();
extern void Expand_Finish();
extern void Expand_Cmov (TOP top, TN *result, TN *src, TN *rflags, OPS *ops,
			 TN *result2 = NULL, TN *src2 = NULL);
#endif

/* Predicate manipulation routines.
 *
 * Most if not all of these routines take two result TNs. The second
 * result is a complement of the first. If one result is not needed,
 * it can be specified as NULL, in which case the utility will allocate
 * a temp if necessary, or a dedicated TN for a sink register (if
 * the architecture has one). 
 */
extern void Exp_Pred_Set(TN *dest, TN *cdest, INT val, OPS *ops);
extern void Exp_Pred_Copy(TN *dest, TN *cdest, TN *src, OPS *ops);
extern void Exp_Pred_Complement(TN *dest, TN *cdest, TN *src, OPS *ops);
extern void Exp_Pred_Compare(TN *dest, TN *cdest, TN *src1, TN *src2, 
			     VARIANT variant, OPS *ops);

#ifdef TARG_ST
extern void Exp_Var_Extract (INT count, TN **dests, TN *src, OPS *ops);
extern void Exp_Var_Compose (INT count, TN *dst, TN **srcs, OPS *ops);
extern void Exp_SIMD_Shuffle_Bits (TN *tgt_tn, TN *src1, UINT bit_offset1, UINT bit_size1, TN *src2, UINT bit_offset2, UINT bit_size2, OPS *ops);
extern void Expand_Compose (TN *tgt_tn, TN *low_tn, TN *high_tn, OPS *ops);
extern void Expand_Extract (TN *low_tn, TN *high_tn, TN *src_tn, OPS *ops);
#endif
#ifndef TARG_ST

/* check if target can handle immediate operand;
 * True if target can, false if should use target-independent logic.
 */
extern BOOL Target_Has_Immediate_Operand (WN *parent, WN *expr);
#endif


#ifdef TARG_X8664
extern void CG_Set_Is_Stack_Used();
#endif
#ifdef TARG_ST
#include "exp_targ.h"
#endif
#endif /* cgexp_INCLUDED */
