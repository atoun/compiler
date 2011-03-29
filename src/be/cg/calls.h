/*
 *  Copyright (C) 2007 PathScale, LLC.  All Rights Reserved.
 */

/*
 *  Copyright (C) 2007 QLogic Corporation.  All Rights Reserved.
 */

/*
 * Copyright 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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


#ifndef calls_INCLUDED
#define calls_INCLUDED

/* ====================================================================
 * ====================================================================
 *
 * Module: calls.h
 * $Revision: 1.11 $
 * $Date: 05/05/04 09:52:06-07:00 $
 * $Author: gautam@eng-27.pathscale.com $
 * $Source: ../../be/cg/SCCS/s.calls.h $
 *
 * Revision history:
 *  03-Oct-91 - Original Version
 *  24-Oct-91 - Added support for entry/exit code.
 *
 * Description:
 *
 * General support for call-related functionality in the back end.
 *
 * Defines a descriptor for calls, to be used to contain such
 * information as which registers are actually used to pass parameters,
 * which registers are killed by the call, and so on.
 *
 * At this time, the contents of this descriptor are minimal; it is
 * expected to expand when we add inter-procedural register allocation
 * or similar features.
 *
 * TODO:  should change this to entry_exit.{h,cxx} to reflect real usage.
 *
 * ====================================================================
 * ====================================================================
 */


/* for ST */
#include "symtab.h"
#include "wn.h"
#ifdef KEY
#include "bb.h"
#endif

/* ================================================================= */

/* Incomplete types to keep ANSI happy: */
struct bb;
struct op;

/* frame length */
extern INT64 Frame_Len;
extern void Set_Frame_Len (INT64 val);		// sets spadjust TN values

/* ================================================================= */

/* We need to keep track of the callee-saved registers for save/restore
 * purposes.  For this purpose, we keep an array of structures (with
 * size determined by the actual number of callee saved regs) which
 * maps each callee-saved register to a "save-tn". We keep track of 
 * the return-address register the same way.
 */
typedef struct save_reg {
  struct tn 	*ded_tn; /* the dedicated TN for the callee-saved register */
  struct tn	*sv_tn;	 /* the save-tn for the callee-saved register */
} SAVE_REG;

/* Define the access macros, including indirect TN access: */
#define SAVE_ded_tn(s)	((s)->ded_tn)
#define SAVE_tn(s)	((s)->sv_tn)

/* Return address register map: */
extern SAVE_REG *Return_Address_Reg;

/* Callee saved register map: */
extern SAVE_REG *Callee_Saved_Regs;	/* Really an array */
extern INT32 Callee_Saved_Regs_Count;	/* Size of array */
#define CALLEE_ded_tn(s)	SAVE_ded_tn(Callee_Saved_Regs+s)
#define CALLEE_tn(s)		SAVE_tn(Callee_Saved_Regs+s)

/* Special PREGs corresponding to special TNs */
extern PREG_NUM *Callee_Saved_Pregs;
extern PREG_NUM Caller_FP_Preg;
extern PREG_NUM Caller_GP_Preg;
extern PREG_NUM Return_Preg;
extern PREG_NUM GP_Preg;
extern PREG_NUM Return_Int_Preg[2];
extern PREG_NUM Return_Float_Preg[2];

extern BOOL Gen_Frame_Pointer;

/* assign a special preg to each CALLEE_tn.  also ra, and gp */
extern void Init_Callee_Saved_Regs_for_REGION( ST *pu, BOOL is_region );

/* Init subprogram entry/exit code: */
extern void Init_Entry_Exit_Code ( WN *pu_wn);

/* Produce subprogram entry/exit code: */
extern void Generate_Entry_Exit_Code ( ST* pu, BOOL is_region );
extern void Adjust_Entry_Exit_Code ( ST* pu );
extern void Adjust_GP_Setup_Code ( ST* pu, BOOL allocate_registers );
extern void Adjust_LC_Setup_Code ( void);
extern BOOL LC_Used_In_PU;	/* flag whether LC_TN was used */

/* Tail calls: */
extern void Optimize_Tail_Calls( ST* pu );

#ifdef TARG_X8664
void Adjust_SP_After_Call( BB* );
extern INT Push_Pop_Int_Saved_Regs (void);
#endif

#ifdef KEY
// The following are interfaces into calls.cxx Callee saved registers stack
typedef struct save_reg_loc {
  struct tn 	*ded_tn; /* the dedicated TN for the callee-saved register */
  ST 		*temp;   /* the save location */
  BOOL		user_allocated; /* true if allocated by user via asm */
} SAVE_REG_LOC;

// Return TRUE if this tn is not in the list of callee-saved TNs
extern BOOL Is_Unique_Callee_Saved_Reg (TN *);
// Number of callee saved registers
extern INT Cgdwarf_Num_Callee_Saved_Regs (void);
// Nth callee saved register dedicated TN
extern struct tn* Cgdwarf_Nth_Callee_Saved_Reg (INT n);
// The location on the stack that corresponds to the nth TN on the stack.
extern ST* Cgdwarf_Nth_Callee_Saved_Reg_Location (INT n);
#endif

#ifdef TARG_MIPS
extern TN *Caller_GP_TN;

/* ====================================================================
 *   Is_Callee_Register
 *
 *   Return a boolean that indicates if <tn> is a callee register.
 * ====================================================================
 */
inline BOOL
Is_Callee_Register(TN *tn)
{
  return (ABI_PROPERTY_Is_callee (TN_register_class(tn),
       REGISTER_machine_id(TN_register_class(tn), TN_register(tn))));
}

extern TN *CGTARG_Gen_Got_Disp_TN ( ST *st, OPS *ops );
extern TN *CGTARG_Gen_GP_Value_TN( ST *st, OPS *ops );
extern TN *CGTARG_Gen_FP_TN (void);
extern TN *CGTARG_Gen_GP_TN (void);
extern TN *CGTARG_Gen_Pfs_TN (void);

extern TN *CGTARG_Get_SP_Incr (OP *sp_adj);
extern TN *CGTARG_Get_FP_Incr (OP *fp_adj);


#endif
#endif /* calls_INCLUDED */
