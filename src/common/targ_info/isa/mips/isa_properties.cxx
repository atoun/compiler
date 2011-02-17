/*
 *  Copyright 2007, 2008, 2009 PathScale, LLC.  All Rights Reserved.
 */

/*
 *  Copyright (C) 2007. QLogic Corporation. All Rights Reserved.
 */

/*
 * Copyright 2003, 2004 PathScale, Inc.  All Rights Reserved.
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


//  
//  Generate ISA properties information
///////////////////////////////////////
// The instructions are listed by category. The different categories of
// instructions are:
//
//   1. Operator attributes descriptors
//   2. Exception classification descriptors
//   3. Other operator descriptors (mostly for global optimization). 
//
// Within each ISA_PROPERTY instructions are listed in alphabetical order and
// as shown in the ISA manual
/////////////////////////////////////
//  $Revision: 1.8 $
//  $Date: 05/03/02 15:27:55-08:00 $
//  $Author: bos@eng-24.internal.keyresearch.com $
//  $Source: common/targ_info/isa/mips/SCCS/s.isa_properties.cxx $


#include <stddef.h>
#include "topcode.h"
#include "isa_properties_gen.h"

main()
{
  ISA_PROPERTY
    move,   		/* Move operator */
    load,   		/* Memory load operator */
    store, 		/* Memory store operator */
    fp_load,   		/* FP memory load operator */
    fp_store, 		/* FP memory store operator */
    prefetch,		/* Prefetch operator */
    xfer, 		/* Control transfer operator */
    call, 		/* Subprogram call operator */
    cond, 		/* Call/xfer is conditional */
    likely, 		/* Cond call/xfer is likely */
    unalign_ld, 	/* Unaligned load operator */
    unalign_store,	/* Unaligned store operator */
    unknown_addr,	/* Memory operator potentially access any memory location */
    unknown_memdata,	/* Memory operator potentially alter data loaded/stored */
    cond_move, 		/* conditional move */
    fp_move, 		/* move to FP register */
    uniq_res, 		/* Result must not be opnd */
    same_res,		/* Result must be same as opnd */
    noop, 		/* No-op operator */
    select, 		/* Operator is a machine level select */
    dummy, 		/* No-op doesn't get emitted */
    iadd, 		/* Integer add operator */
    isub, 		/* Integer subtract operator */
    imul, 		/* Integer multiply operator */
    idiv,		/* Integer divide operator */
    iop64,		/* 64-bit only integer operator */
    flop, 		/* Any proper floating point op */
    fadd, 		/* FP add operator */
    fsub,		/* FP subtract operator */
    fmul, 		/* FP multiply operator */
    fmisc,              /* FP miscellaneous class type */
    madd,		/* The kind that do two at once */
    mmalu,              /* Multimedia ALU operator */
    mmmul,              /* Multimedia multiply operator */
    mmshf,              /* Multimedia shift operator */
    itrap,		/* Integer trap potential */
    safe,		/* Never traps -- always safe */
    ftrap,		/* Floating point trap potential */
    fdiv,		/* Floating point divides */
    sqrt,		/* Square root operator */
    memtrap,		/* Memory trap potential */
    unsafe,		/* Unsafe always */
    defs_fp,		/* Operator defines FP reg */
    defs_fcc,		/* Operator defines FP CC reg */
    defs_fcr,		/* Operator defines FCR reg */
    refs_fcr,		/* Operator uses FCR reg */
    defs_fpu_int,	/* Operator defs int val in FP reg */
    ior,		/* Integer logical OR operator */
    jump,		/* Jump operator */
    ijump,		/* Indirect jump operator */
    ixor,		/* Integer logical exclusive OR operator */
    iand,		/* Integer logical AND operator */
    icmp,		/* Integer compare operator */
    f_group,            /* Instruction must be first in an instruction group */
    l_group,            /* Instruction must be last in an instruction group */
    privileged,         /* Instruction is a privileged instruction */
    simulated,		/* Instruction is simulated, i.e. a macro */
    predicated,		/* Instruction is predicated */
    access_reg_bank,	/* Instruction accesses rotating register banks */
    side_effects,	/* Instruction has side effects */
    branch_predict,	/* Branch prediction (but not actual xfer) */
    mem_fill_type,      /* Memory instructions which are fill/spill type */
    var_opnds,		/* Variable number of operands AND/OR results */
    base_update,	/* Instruction updates 'base' operand */
    loop_start,         /* Instruction marks the start of a zero-cost loop */

			/* Workaround for FP stall and rounding bugs */
    fp_arith,		/* Consume a slot in the fp pipeline */
    fp_unpredictable_latency, /* Write fp reg but don't flow down fp pipeline */
    fp_stall_hazard,	/* wrong result if stalled in FR or M1 pipeline stage */
    fp_round_hazard,	/* result corrupted by a subsequent fp_round_trigger */
    fp_round_trigger;	/* can damage previous fp_round_hazard result */

  ISA_Properties_Begin ("MIPS");

/* ====================================================================
 *              Operator attributes descriptors
 * ====================================================================
 */

/* ===== Move operator ====== */
  move = ISA_Property_Create ("move");
  Instruction_Group (move,
		      TOP_movf,
		      TOP_movn,
		      TOP_movt,
		      TOP_movz,
		      TOP_mov_s,
		      TOP_mov_d,
		      TOP_mov_ps,
		      TOP_movf_s,
		      TOP_movf_d,
		      TOP_movf_ps,
		      TOP_movn_s,
		      TOP_movn_d,
		      TOP_movn_ps,
		      TOP_movt_s,
		      TOP_movt_d,
		      TOP_movt_ps,
		      TOP_movz_s,
		      TOP_movz_d,
		      TOP_movz_ps,
                     TOP_UNDEFINED);

/* ===== Memory load operator ====== */
  load = ISA_Property_Create ("load");
  Instruction_Group (load,
		      TOP_lb,
		      TOP_lbu,
		      TOP_lh,
		      TOP_lhu,
		      TOP_lw,
		      TOP_ll,
		      TOP_lwu,
		      TOP_ld,
		      TOP_lld,
		      TOP_lwc1,
		      TOP_ldc1,
		      TOP_lwxc1,
		      TOP_ldxc1,
		      TOP_lwl,
		      TOP_lwr,
		      TOP_ldl,
		      TOP_ldr,
		      TOP_lbx,
		      TOP_ldx,
		      TOP_lhx,
		      TOP_lwx,
		      TOP_luxc1,
                     TOP_UNDEFINED);

/* ===== Memory store operator ====== */
  store = ISA_Property_Create ("store");
  Instruction_Group (store,
		      TOP_sb,
		      TOP_sh,
		      TOP_sw,
		      TOP_sc,
		      TOP_sd,
		      TOP_scd,
		      TOP_swc1,
		      TOP_sdc1,
		      TOP_swxc1,
		      TOP_sdxc1,
		      TOP_swl,
		      TOP_swr,
		      TOP_sdl,
		      TOP_sdr,
		      TOP_suxc1,
                     TOP_UNDEFINED);

/* ===== FP memory load operator ====== */
  fp_load = ISA_Property_Create ("fp_load");
  Instruction_Group (fp_load,
		      TOP_lwc1,
		      TOP_ldc1,
		      TOP_lwxc1,
		      TOP_ldxc1,
		      TOP_luxc1,
                     TOP_UNDEFINED);

/* ===== FP memory store operator ====== */
  fp_store = ISA_Property_Create ("fp_store");
  Instruction_Group (fp_store,
		      TOP_swc1,
		      TOP_sdc1,
		      TOP_swxc1,
		      TOP_sdxc1,
		      TOP_suxc1,
                     TOP_UNDEFINED);

/* ===== Prefetch operator ====== */
  prefetch = ISA_Property_Create ("prefetch");
  Instruction_Group (prefetch,
	  	      TOP_pref,
	  	      TOP_prefx,
                     TOP_UNDEFINED);

/* ===== Memory fill/spill type instructions ====== */
  mem_fill_type = ISA_Property_Create ("mem_fill_type");
  Instruction_Group (mem_fill_type,
		     TOP_UNDEFINED);

/* ===== Control transfer operator ====== */
  xfer = ISA_Property_Create ("xfer");
  Instruction_Group (xfer,
		      TOP_j,
		      TOP_jal,
		      TOP_jalr,
		      TOP_jr,
		      TOP_beq,
		      TOP_bgez,
		      TOP_bgezal,
		      TOP_bgtz,
		      TOP_blez,
		      TOP_bltz,
		      TOP_bltzal,
		      TOP_bne,
		      TOP_bc1f,
		      TOP_bc1t,
		      TOP_bc2any2f,
		      TOP_bc2any2t,
                     TOP_UNDEFINED);

/* ===== Subprogram call operator ====== */
  call = ISA_Property_Create ("call");
  Instruction_Group (call,
		      TOP_jal,
		      TOP_jalr,
                     TOP_UNDEFINED);

/* ===== Call/xfer is conditional ====== */
  cond = ISA_Property_Create ("cond");
  Instruction_Group (cond,
		      TOP_beq,
		      TOP_bgez,
		      TOP_bgezal,
		      TOP_bgtz,
		      TOP_blez,
		      TOP_bltz,
		      TOP_bltzal,
		      TOP_bne,
		      TOP_bc1f,
		      TOP_bc1t,
		      TOP_bc2any2f,
		      TOP_bc2any2t,
                     TOP_UNDEFINED);

/* ===== Cond call/xfer is likely ====== */
  likely = ISA_Property_Create ("likely");
  Instruction_Group (likely,
                     TOP_UNDEFINED);

/* ===== Result def is conditional ====== */
  cond_move = ISA_Property_Create ("cond_move");
  Instruction_Group (cond_move,
		      TOP_movf,
		      TOP_movn,
		      TOP_movt,
		      TOP_movz,
		      TOP_movf_s,
		      TOP_movf_d,
		      TOP_movf_ps,
		      TOP_movn_s,
		      TOP_movn_d,
		      TOP_movn_ps,
		      TOP_movt_s,
		      TOP_movt_d,
		      TOP_movt_ps,
		      TOP_movz_s,
		      TOP_movz_d,
		      TOP_movz_ps,
                     TOP_UNDEFINED);

/* ===== FP Move operator ====== */
  fp_move = ISA_Property_Create ("fp_move");
  Instruction_Group (fp_move,
		      TOP_mov_s,
		      TOP_mov_d,
		      TOP_mov_ps,
		      TOP_movf_s,
		      TOP_movf_d,
		      TOP_movf_ps,
		      TOP_movn_s,
		      TOP_movn_d,
		      TOP_movn_ps,
		      TOP_movt_s,
		      TOP_movt_d,
		      TOP_movt_ps,
		      TOP_movz_s,
		      TOP_movz_d,
		      TOP_movz_ps,
                     TOP_UNDEFINED);

/* ===== Result must not be opnd ====== */
  uniq_res = ISA_Property_Create ("uniq_res");
  Instruction_Group (uniq_res,
		     TOP_intrncall,
                     TOP_UNDEFINED);

/* ===== Result must be same as opnd ====== */
  same_res = ISA_Property_Create ("same_res");
  Instruction_Group (same_res,
		     /* Because asm macros can share the same operand between
		        inputs and outputs, we conservatively mark this 
			property so that the compiler doesn't try to separate
			any common operands. */
                      TOP_asm,
		      TOP_movf,
		      TOP_movn,
		      TOP_movt,
		      TOP_movz,
		      TOP_movf_s,
		      TOP_movf_d,
		      TOP_movf_ps,
		      TOP_movn_s,
		      TOP_movn_d,
		      TOP_movn_ps,
		      TOP_movt_s,
		      TOP_movt_d,
		      TOP_movt_ps,
		      TOP_movz_s,
		      TOP_movz_d,
		      TOP_movz_ps,
                      TOP_mov_fcc,
		      TOP_lwl,
		      TOP_lwr,
		      TOP_ldl,
		      TOP_ldr,
		      TOP_dins,
		      TOP_dinsm,
		      TOP_dinsu,
		      TOP_ins,
                     TOP_UNDEFINED);

/* ===== Operator is a machine level select ====== */
  select = ISA_Property_Create ("select");
  Instruction_Group (select,
                     TOP_UNDEFINED);

/* ===== Unaligned load operator ====== */
  unalign_ld = ISA_Property_Create ("unalign_ld");
  Instruction_Group (unalign_ld,
		      TOP_lwl,
		      TOP_lwr,
		      TOP_ldl,
		      TOP_ldr,
                     TOP_UNDEFINED);

/* ===== Unaligned store operator ====== */
  unalign_store = ISA_Property_Create ("unalign_store");
  Instruction_Group (unalign_store,
		      TOP_swl,
		      TOP_swr,
		      TOP_sdl,
		      TOP_sdr,
		     TOP_UNDEFINED);

/* ===== Unknown addr operator ====== */
  unknown_addr = ISA_Property_Create ("unknown_memdata");
  Instruction_Group (unknown_memdata,
		     TOP_UNDEFINED);

/* ===== Unknown addr operator ====== */
  unknown_addr = ISA_Property_Create ("unknown_addr");
  Instruction_Group (unknown_addr,
		     TOP_UNDEFINED);

/* ===== Integer add operator ====== */
  iadd = ISA_Property_Create ("iadd");
  Instruction_Group (iadd,
		      TOP_add,
		      TOP_addi,
		      TOP_addiu,
		      TOP_addu,
		      TOP_dadd,
		      TOP_daddi,
		      TOP_daddiu,
		      TOP_daddu,
                     TOP_UNDEFINED);

/* ===== Integer subtract operator ====== */
  isub = ISA_Property_Create ("isub");
  Instruction_Group (isub,
                     TOP_sub,
		     TOP_subu,
		     TOP_dsub,
		     TOP_dsubu,
                     TOP_UNDEFINED);

/* ===== Integer multiply operator ====== */
  imul = ISA_Property_Create ("imul");
  Instruction_Group (imul,
	  	     TOP_mul,
	  	     TOP_mult,
	  	     TOP_multu,
	  	     TOP_dmult,
	  	     TOP_dmultu,
		     // The following are included because they also
		     // impact hi/low registers
		     TOP_madd,
		     TOP_maddu,
		     TOP_msub,
		     TOP_msubu,
		     TOP_dmadd,
		     TOP_dmaddu,
		     TOP_dmsub,
		     TOP_dmsubu,
		     TOP_dmulg,
		     TOP_dmulgu,
                     TOP_UNDEFINED);

/* ===== Integer divide operator ====== */
  idiv = ISA_Property_Create ("idiv");
  Instruction_Group (idiv,
	  	     TOP_div,
	  	     TOP_divu,
	  	     TOP_ddiv,
	  	     TOP_ddivu,
                     TOP_UNDEFINED);

/* ===== 64-bit-only integer operator ====== */
  iop64 = ISA_Property_Create ("iop64");
  Instruction_Group (iop64,
		     TOP_dadd,
		     TOP_daddi,
		     TOP_daddiu,
		     TOP_daddu,
		     TOP_dsub,
		     TOP_dsubu,
	  	     TOP_dmult,
	  	     TOP_dmultu,
	  	     TOP_ddiv,
	  	     TOP_ddivu,
		     TOP_dclo,
		     TOP_dclz,
		     TOP_dsll,
		     TOP_dsll32,
		     TOP_dsllv,
		     TOP_dsra,
		     TOP_dsra32,
		     TOP_dsrav,
		     TOP_dsrl,
		     TOP_dsrl32,
		     TOP_dsrlv,
		     TOP_dmfc1,
		     TOP_dmtc1,
		     TOP_dext,
		     TOP_dextm,
		     TOP_dextu,
		     TOP_dins,
		     TOP_dinsm,
		     TOP_dinsu,
		     TOP_drotr,
		     TOP_drotr32,
		     TOP_drotrv,
		     TOP_dmadd,
		     TOP_dmaddu,
		     TOP_dmsub,
		     TOP_dmsubu,
		     TOP_dmulg,
		     TOP_dmulgu,
		     TOP_dperm,
		     TOP_dpop,
                     TOP_UNDEFINED);

/* ===== Any proper floating point op ====== */
  flop = ISA_Property_Create ("flop");
  Instruction_Group (flop,
		      TOP_abs_s,
		      TOP_abs_d,
		      TOP_abs_ps,
		      TOP_add_s,
		      TOP_add_d,
		      TOP_add_ps,
		      TOP_c_f_s,
		      TOP_c_f_d,
		      TOP_c_f_ps,
		      TOP_c_t_s,
		      TOP_c_t_d,
		      TOP_c_t_ps,
		      TOP_c_un_s,
		      TOP_c_un_d,
		      TOP_c_un_ps,
		      TOP_c_or_s,
		      TOP_c_or_d,
		      TOP_c_or_ps,
		      TOP_c_eq_s,
		      TOP_c_eq_d,
		      TOP_c_eq_ps,
		      TOP_c_neq_s,
		      TOP_c_neq_d,
		      TOP_c_neq_ps,
		      TOP_c_ueq_s,
		      TOP_c_ueq_d,
		      TOP_c_ueq_ps,
		      TOP_c_ogl_s,
		      TOP_c_ogl_d,
		      TOP_c_ogl_ps,
		      TOP_c_olt_s,
		      TOP_c_olt_d,
		      TOP_c_olt_ps,
		      TOP_c_uge_s,
		      TOP_c_uge_d,
		      TOP_c_uge_ps,
		      TOP_c_ult_s,
		      TOP_c_ult_d,
		      TOP_c_ult_ps,
		      TOP_c_oge_s,
		      TOP_c_oge_d,
		      TOP_c_oge_ps,
		      TOP_c_ole_s,
		      TOP_c_ole_d,
		      TOP_c_ole_ps,
		      TOP_c_ugt_s,
		      TOP_c_ugt_d,
		      TOP_c_ugt_ps,
		      TOP_c_ule_s,
		      TOP_c_ule_d,
		      TOP_c_ule_ps,
		      TOP_c_ogt_s,
		      TOP_c_ogt_d,
		      TOP_c_ogt_ps,
		      TOP_c_sf_s,
		      TOP_c_sf_d,
		      TOP_c_sf_ps,
		      TOP_c_st_s,
		      TOP_c_st_d,
		      TOP_c_st_ps,
		      TOP_c_ngle_s,
		      TOP_c_ngle_d,
		      TOP_c_ngle_ps,
		      TOP_c_gle_s,
		      TOP_c_gle_d,
		      TOP_c_gle_ps,
		      TOP_c_seq_s,
		      TOP_c_seq_d,
		      TOP_c_seq_ps,
		      TOP_c_sne_s,
		      TOP_c_sne_d,
		      TOP_c_sne_ps,
		      TOP_c_ngl_s,
		      TOP_c_ngl_d,
		      TOP_c_ngl_ps,
		      TOP_c_gl_s,
		      TOP_c_gl_d,
		      TOP_c_gl_ps,
		      TOP_c_lt_s,
		      TOP_c_lt_d,
		      TOP_c_lt_ps,
		      TOP_c_nlt_s,
		      TOP_c_nlt_d,
		      TOP_c_nlt_ps,
		      TOP_c_nge_s,
		      TOP_c_nge_d,
		      TOP_c_nge_ps,
		      TOP_c_ge_s,
		      TOP_c_ge_d,
		      TOP_c_ge_ps,
		      TOP_c_le_s,
		      TOP_c_le_d,
		      TOP_c_le_ps,
		      TOP_c_nle_s,
		      TOP_c_nle_d,
		      TOP_c_nle_ps,
		      TOP_c_ngt_s,
		      TOP_c_ngt_d,
		      TOP_c_ngt_ps,
		      TOP_c_gt_s,
		      TOP_c_gt_d,
		      TOP_c_gt_ps,
		      TOP_div_s,
		      TOP_div_d,
		      TOP_mul_s,
		      TOP_mul_d,
		      TOP_mul_ps,
		      TOP_neg_s,
		      TOP_neg_d,
		      TOP_neg_ps,
		      TOP_sub_s,
		      TOP_sub_d,
		      TOP_sub_ps,
		      TOP_sqrt_s,
		      TOP_sqrt_d,
		      TOP_madd_s,
		      TOP_madd_d,
		      TOP_madd_ps,
		      TOP_msub_s,
		      TOP_msub_d,
		      TOP_msub_ps,
		      TOP_nmadd_s,
		      TOP_nmadd_d,
		      TOP_nmadd_ps,
		      TOP_nmsub_s,
		      TOP_nmsub_d,
		      TOP_nmsub_ps,
		      TOP_pll_ps,
		      TOP_plu_ps,
		      TOP_pul_ps,
		      TOP_puu_ps,
		      TOP_recip_s,
		      TOP_recip_d,
		      TOP_rsqrt_s,
		      TOP_rsqrt_d,
		      TOP_addred_ps,
		      TOP_mulred_ps,
		      TOP_ma_nlupuu_ps,
		      TOP_ma_plunuu_ps,
		      TOP_ma_plupll_ps,
		      TOP_ma_puupul_ps,
		      TOP_recipit1_d,
		      TOP_recipit1_s,
		      TOP_recipit1_ps,
		      TOP_recipit2_d,
		      TOP_recipit2_s,
		      TOP_recipit2_ps,
		      TOP_rsqrtit1_d,
		      TOP_rsqrtit1_s,
		      TOP_rsqrtit1_ps,
		      TOP_rsqrtit2_d,
		      TOP_rsqrtit2_s,
		      TOP_rsqrtit2_ps,
                     TOP_UNDEFINED);

/* ===== FP add operator ====== */
  fadd = ISA_Property_Create ("fadd");
  Instruction_Group (fadd,
		      TOP_add_s,
		      TOP_add_d,
                     TOP_UNDEFINED);

/* ===== FP subtract operator ====== */
  fsub = ISA_Property_Create ("fsub");
  Instruction_Group (fsub,
		      TOP_sub_s,
		      TOP_sub_d,
                     TOP_UNDEFINED);

/* ===== FP multiply operator ====== */
  fmul = ISA_Property_Create ("fmul");
  Instruction_Group (fmul,
		      TOP_mul_s,
		      TOP_mul_d,
		      TOP_mul_ps,
                     TOP_UNDEFINED);

/* ===== FP miscellaneous operator ====== */
  fmisc = ISA_Property_Create ("fmisc");
  Instruction_Group (fmisc,
		      TOP_abs_s,
		      TOP_abs_d,
		      TOP_abs_ps,
		      TOP_recip_s,
		      TOP_recip_d,
		      TOP_rsqrt_s,
		      TOP_rsqrt_d,
		      TOP_neg_s,
		      TOP_neg_d,
		      TOP_neg_ps,
		      TOP_pll_ps,
		      TOP_plu_ps,
		      TOP_pul_ps,
		      TOP_puu_ps,
		     TOP_UNDEFINED);

/* ===== The kind that do two at once ====== */
  // Used in isa/expand.cxx only and not for any purpose we could use.
  madd = ISA_Property_Create ("madd");
  Instruction_Group (madd,
		      TOP_madd_s,
		      TOP_madd_d,
		      TOP_madd_ps,
		      TOP_msub_s,
		      TOP_msub_d,
		      TOP_msub_ps,
		      TOP_nmadd_s,
		      TOP_nmadd_d,
		      TOP_nmadd_ps,
		      TOP_nmsub_s,
		      TOP_nmsub_d,
		      TOP_nmsub_ps,
                     TOP_UNDEFINED);

/* ===== Instructions belonging to Multimedia ALU type ====== */
  mmalu = ISA_Property_Create ("mmalu");
  Instruction_Group (mmalu,
		     TOP_UNDEFINED);

/* ===== Instructions belonging to Multimedia shift (MMSHF) type ====== */
  mmshf = ISA_Property_Create ("mmshf");
  Instruction_Group (mmshf,
		     TOP_UNDEFINED);

/* ===== Instructions belonging to Multimedia multiply (MMMUL) type ====== */
  mmmul = ISA_Property_Create ("mmmul");
  Instruction_Group (mmmul,
		     TOP_UNDEFINED);

  noop = ISA_Property_Create ("noop");
  Instruction_Group (noop,
                     TOP_nop,
                     TOP_noop,
                     TOP_UNDEFINED);

  dummy = ISA_Property_Create ("dummy");
  Instruction_Group (dummy,
		     TOP_dfixup,
		     TOP_ffixup,
		     TOP_ifixup,
		     TOP_begin_pregtn,
		     TOP_end_pregtn,
		     TOP_fwd_bar,
		     TOP_bwd_bar,
		     TOP_label,
                     TOP_noop,
                     TOP_swp_start,
                     TOP_swp_stop,
		     TOP_UNDEFINED);

/* ====================================================================
 *              Exception classification descriptors
 * ====================================================================
 */

  /* ===== Integer trap potential ====== */
  itrap = ISA_Property_Create ("itrap");
  Instruction_Group (itrap,
		     TOP_teq,
		     TOP_teqi,
		     TOP_tge,
		     TOP_tgei,
		     TOP_tgeiu,
		     TOP_tgeu,
		     TOP_tlt,
		     TOP_tlti,
		     TOP_tltiu,
		     TOP_tltu,
		     TOP_tne,
		     TOP_tnei,
                     TOP_UNDEFINED);

  /* ===== Never traps -- always safe ====== */
  safe = ISA_Property_Create ("safe");
  Instruction_Group (safe,
			// TODO
                     TOP_UNDEFINED);

  /* ===== Unsafe always ====== */
  unsafe = ISA_Property_Create ("unsafe");
  Instruction_Group (unsafe,
                     TOP_fwd_bar, TOP_bwd_bar,
                     TOP_UNDEFINED);

  /* ===== Floating point trap potential ====== */
  ftrap = ISA_Property_Create ("ftrap");
  Instruction_Group (ftrap,
                     TOP_UNDEFINED);

  /* ===== Floating point divides ====== */
  fdiv = ISA_Property_Create ("fdiv");
  Instruction_Group (fdiv,
		     TOP_recip_s,
		     TOP_recip_d,
		     TOP_rsqrt_s,
		     TOP_rsqrt_d,
		     TOP_div_s,
		     TOP_div_d,
		     TOP_recipit1_d,
		     TOP_recipit1_s,
		     TOP_recipit1_ps,
		     TOP_recipit2_d,
		     TOP_recipit2_s,
		     TOP_recipit2_ps,
		     TOP_rsqrtit1_d,
		     TOP_rsqrtit1_s,
		     TOP_rsqrtit1_ps,
		     TOP_rsqrtit2_d,
		     TOP_rsqrtit2_s,
		     TOP_rsqrtit2_ps,
                     TOP_UNDEFINED);

  /* ===== Square roots ====== */
  sqrt = ISA_Property_Create ("sqrt");
  Instruction_Group (sqrt,
		      TOP_rsqrt_s,
		      TOP_rsqrt_d,
		      TOP_sqrt_s,
		      TOP_sqrt_d,
                     TOP_UNDEFINED);

  /* ===== Memory trap potential ====== */
  memtrap = ISA_Property_Create ("memtrap");
  Instruction_Group (memtrap,
		      TOP_lb,
		      TOP_lbu,
		      TOP_lh,
		      TOP_lhu,
		      TOP_lw,
		      TOP_ll,
		      TOP_lwu,
		      TOP_ld,
		      TOP_lld,
		      TOP_lwc1,
		      TOP_ldc1,
		      TOP_lwxc1,
		      TOP_ldxc1,
		      TOP_lwl,
		      TOP_lwr,
		      TOP_ldl,
		      TOP_ldr,
		      TOP_sb,
		      TOP_sh,
		      TOP_sw,
		      TOP_sc,
		      TOP_sd,
		      TOP_scd,
		      TOP_swc1,
		      TOP_sdc1,
		      TOP_swxc1,
		      TOP_sdxc1,
		      TOP_swl,
		      TOP_swr,
		      TOP_sdl,
		      TOP_sdr,
                     TOP_UNDEFINED);

  /* ===== Instruction must be first in an instruction group ====== */
  f_group = ISA_Property_Create ("f_group");
  Instruction_Group (f_group,
		     TOP_UNDEFINED);

  /* ===== Instruction must be last in an instruction group ====== */
  l_group = ISA_Property_Create ("l_group");
  Instruction_Group (l_group,
		     TOP_UNDEFINED);

  /* ===== Instruction is a privileged instruction ====== */
  privileged = ISA_Property_Create ("privileged");
  Instruction_Group (privileged,
		     TOP_UNDEFINED);

/* ====================================================================
 * Other operator descriptors (mostly for global optimization).
 * TODO: These descriptors should actually be determined from mips_operands.
 * ====================================================================
 */

/* ===== Operator defines FP CC reg ====== */
  defs_fcc = ISA_Property_Create ("defs_fcc");
  Instruction_Group (defs_fcc,
		      TOP_c_f_s,
		      TOP_c_f_d,
		      TOP_c_t_s,
		      TOP_c_t_d,
		      TOP_c_un_s,
		      TOP_c_un_d,
		      TOP_c_or_s,
		      TOP_c_or_d,
		      TOP_c_eq_s,
		      TOP_c_eq_d,
		      TOP_c_neq_s,
		      TOP_c_neq_d,
		      TOP_c_ueq_s,
		      TOP_c_ueq_d,
		      TOP_c_ogl_s,
		      TOP_c_ogl_d,
		      TOP_c_olt_s,
		      TOP_c_olt_d,
		      TOP_c_uge_s,
		      TOP_c_uge_d,
		      TOP_c_ult_s,
		      TOP_c_ult_d,
		      TOP_c_oge_s,
		      TOP_c_oge_d,
		      TOP_c_ole_s,
		      TOP_c_ole_d,
		      TOP_c_ugt_s,
		      TOP_c_ugt_d,
		      TOP_c_ule_s,
		      TOP_c_ule_d,
		      TOP_c_ogt_s,
		      TOP_c_ogt_d,
		      TOP_c_sf_s,
		      TOP_c_sf_d,
		      TOP_c_st_s,
		      TOP_c_st_d,
		      TOP_c_ngle_s,
		      TOP_c_ngle_d,
		      TOP_c_gle_s,
		      TOP_c_gle_d,
		      TOP_c_seq_s,
		      TOP_c_seq_d,
		      TOP_c_sne_s,
		      TOP_c_sne_d,
		      TOP_c_ngl_s,
		      TOP_c_ngl_d,
		      TOP_c_gl_s,
		      TOP_c_gl_d,
		      TOP_c_lt_s,
		      TOP_c_lt_d,
		      TOP_c_nlt_s,
		      TOP_c_nlt_d,
		      TOP_c_nge_s,
		      TOP_c_nge_d,
		      TOP_c_ge_s,
		      TOP_c_ge_d,
		      TOP_c_le_s,
		      TOP_c_le_d,
		      TOP_c_nle_s,
		      TOP_c_nle_d,
		      TOP_c_ngt_s,
		      TOP_c_ngt_d,
		      TOP_c_gt_s,
		      TOP_c_gt_d,
                     TOP_UNDEFINED);

/* ===== Operator defines FCR reg ====== */
  defs_fcr = ISA_Property_Create ("defs_fcr");
  Instruction_Group (defs_fcr,
		      TOP_c_f_s,
		      TOP_c_f_d,
		      TOP_c_t_s,
		      TOP_c_t_d,
		      TOP_c_un_s,
		      TOP_c_un_d,
		      TOP_c_or_s,
		      TOP_c_or_d,
		      TOP_c_eq_s,
		      TOP_c_eq_d,
		      TOP_c_neq_s,
		      TOP_c_neq_d,
		      TOP_c_ueq_s,
		      TOP_c_ueq_d,
		      TOP_c_ogl_s,
		      TOP_c_ogl_d,
		      TOP_c_olt_s,
		      TOP_c_olt_d,
		      TOP_c_uge_s,
		      TOP_c_uge_d,
		      TOP_c_ult_s,
		      TOP_c_ult_d,
		      TOP_c_oge_s,
		      TOP_c_oge_d,
		      TOP_c_ole_s,
		      TOP_c_ole_d,
		      TOP_c_ugt_s,
		      TOP_c_ugt_d,
		      TOP_c_ule_s,
		      TOP_c_ule_d,
		      TOP_c_ogt_s,
		      TOP_c_ogt_d,
		      TOP_c_sf_s,
		      TOP_c_sf_d,
		      TOP_c_st_s,
		      TOP_c_st_d,
		      TOP_c_ngle_s,
		      TOP_c_ngle_d,
		      TOP_c_gle_s,
		      TOP_c_gle_d,
		      TOP_c_seq_s,
		      TOP_c_seq_d,
		      TOP_c_sne_s,
		      TOP_c_sne_d,
		      TOP_c_ngl_s,
		      TOP_c_ngl_d,
		      TOP_c_gl_s,
		      TOP_c_gl_d,
		      TOP_c_lt_s,
		      TOP_c_lt_d,
		      TOP_c_nlt_s,
		      TOP_c_nlt_d,
		      TOP_c_nge_s,
		      TOP_c_nge_d,
		      TOP_c_ge_s,
		      TOP_c_ge_d,
		      TOP_c_le_s,
		      TOP_c_le_d,
		      TOP_c_nle_s,
		      TOP_c_nle_d,
		      TOP_c_ngt_s,
		      TOP_c_ngt_d,
		      TOP_c_gt_s,
		      TOP_c_gt_d,
		      TOP_ctc1,
                     TOP_UNDEFINED);

/* ===== Operator uses FCR reg ====== */
  refs_fcr = ISA_Property_Create ("refs_fcr");
  Instruction_Group (refs_fcr,
	  	      TOP_cfc1,
	  	      TOP_bc1f,
	  	      TOP_bc1t,
		      TOP_bc2any2f,
		      TOP_bc2any2t,
                     TOP_UNDEFINED);

/* ===== Operator defs int val in FP reg ====== */
  defs_fpu_int = ISA_Property_Create ("defs_fpu_int");
  Instruction_Group (defs_fpu_int,
	  		// TODO
                     TOP_UNDEFINED);

/* ===== Operator defines FP reg ====== */
  defs_fp = ISA_Property_Create ("defs_fp");
  Instruction_Group (defs_fp,
	  		// TODO
                     TOP_UNDEFINED);

/* ===== Logical OR operator ====== */
  ior = ISA_Property_Create ("ior");
  Instruction_Group (ior,
                     TOP_or,
                     TOP_ori,
                     TOP_UNDEFINED);

/* ===== Jump operator ====== */
  jump = ISA_Property_Create ("jump");
  Instruction_Group (jump,
                     TOP_j,
                     TOP_UNDEFINED);

/* ===== Indirect jump operator ====== */
  ijump = ISA_Property_Create ("ijump");
  Instruction_Group (ijump,
                     TOP_jr,
                     TOP_UNDEFINED);

/* ===== Logical exclusive OR operator ====== */
  ixor = ISA_Property_Create ("ixor");
  Instruction_Group (ixor,
                     TOP_xor,
                     TOP_xori,
                     TOP_UNDEFINED);

/* ===== Logical AND operator ====== */
  iand = ISA_Property_Create ("iand");
  Instruction_Group (iand,
                     TOP_and,
                     TOP_andi,
                     TOP_UNDEFINED);

/* ===== Integer compare operator ====== */
  icmp = ISA_Property_Create ("icmp");
  Instruction_Group (icmp,
	  	     TOP_slt,
	  	     TOP_slti,
	  	     TOP_sltu,
	  	     TOP_sltiu,
                     TOP_UNDEFINED);

/* ===== Simulated instructions ====== */
  simulated = ISA_Property_Create ("simulated");
  Instruction_Group (simulated,
                     TOP_asm,
                     TOP_sext,
                     TOP_spadjust,
                     TOP_intrncall,
                     TOP_mov_hilo,
                     TOP_mov_fcc,
                     TOP_UNDEFINED);

/* ===== Predicated instructions ====== */
  predicated = ISA_Property_Create ("predicated");
  Instruction_Group (predicated,
                     TOP_UNDEFINED);

/* ===== Instructions access rotating register banks ====== */
  access_reg_bank = ISA_Property_Create ("access_reg_bank");
  Instruction_Group (access_reg_bank,
		     TOP_UNDEFINED);

/* ===== Instructions with side effects ====== */
  side_effects = ISA_Property_Create ("side_effects");
  Instruction_Group (side_effects,
		     /* Because asm macros can trash memory, we conservatively 
			mark this property so that the compiler doesn't move
			instructions around it. */
                     TOP_asm, 
		     TOP_sync, 
		     TOP_intrncall,
		     TOP_UNDEFINED);

/* ===== Instructions with branch predictions ====== */
  branch_predict = ISA_Property_Create ("branch_predict");
  Instruction_Group (branch_predict,
		     TOP_UNDEFINED);

/* ===== Instructions with variable number of operands/results ====== */
  var_opnds = ISA_Property_Create ("var_opnds");
  Instruction_Group (var_opnds,
                     TOP_asm,
		     TOP_UNDEFINED);

/* ===== Instructions that update 'base' operand ====== */
  base_update = ISA_Property_Create ("base_update");
  Instruction_Group (base_update,
		     TOP_UNDEFINED);

/* ===== Instructions that mark the head of a zero-cost loop ====== */
  loop_start = ISA_Property_Create ("loop_start");
  Instruction_Group (loop_start,
		     TOP_UNDEFINED);

/* ===== Workaround FP stall and rounding bugs  ====== */
  fp_arith = ISA_Property_Create ("fp_arith");
  Instruction_Group (fp_arith,
		     TOP_abs_d,
		     TOP_abs_s,
		     TOP_abs_ps,
		     TOP_add_d,
		     TOP_add_s,
		     TOP_add_ps,
		     TOP_c_eq_s,
		     TOP_c_f_s,
		     TOP_c_le_s,
		     TOP_c_lt_s,
		     TOP_c_nge_s,
		     TOP_c_ngl_s,
		     TOP_c_ngt_s,
		     TOP_c_ole_s,
		     TOP_c_olt_s,
		     TOP_c_seq_s,
		     TOP_c_sf_s,
		     TOP_c_ueq_s,
		     TOP_c_ule_s,
		     TOP_c_ult_s,
		     TOP_c_un_s,
		     TOP_ceil_l_d,
		     TOP_ceil_l_s,
		     TOP_ceil_w_d,
		     TOP_ceil_w_s,
		     TOP_cvt_d_l,
		     TOP_cvt_d_s,
		     TOP_cvt_d_w,
		     TOP_cvt_l_d,
		     TOP_cvt_l_s,
		     TOP_cvt_s_d,
		     TOP_cvt_s_l,
		     TOP_cvt_s_w,
		     TOP_cvt_w_d,
		     TOP_cvt_w_s,
		     TOP_cvt_ps_s,
		     TOP_cvt_s_pl,
		     TOP_cvt_s_pu,
		     TOP_div_d,
		     TOP_div_s,
		     TOP_floor_l_d,
		     TOP_floor_l_s,
		     TOP_floor_w_d,
		     TOP_floor_w_s,
		     TOP_madd_d,
		     TOP_madd_s,
		     TOP_madd_ps,
		     TOP_mov_d,
		     TOP_mov_s,
		     TOP_movn_d,
		     TOP_movn_s,
		     TOP_movt_d,
		     TOP_movt_s,
		     TOP_movz_d,
		     TOP_movz_s,
		     TOP_msub_d,
		     TOP_msub_s,
		     TOP_msub_ps,
		     TOP_mul_d,
		     TOP_mul_s,
		     TOP_mul_ps,
		     TOP_neg_d,
		     TOP_neg_s,
		     TOP_neg_ps,
		     TOP_nmadd_d,
		     TOP_nmadd_s,
		     TOP_nmadd_ps,
		     TOP_nmsub_d,
		     TOP_nmsub_s,
		     TOP_nmsub_ps,
		     TOP_recip_d,
		     TOP_recip_s,
		     TOP_round_l_d,
		     TOP_round_l_s,
		     TOP_round_w_d,
		     TOP_round_w_s,
		     TOP_rsqrt_d,
		     TOP_rsqrt_s,
		     TOP_sqrt_d,
		     TOP_sqrt_s,
		     //TOP_ssnop,	not used by compiler
		     TOP_sub_d,
		     TOP_sub_s,
		     TOP_sub_ps,
		     TOP_trunc_l_d,
		     TOP_trunc_l_s,
		     TOP_trunc_w_d,
		     TOP_trunc_w_s,
		     TOP_UNDEFINED);

  fp_unpredictable_latency = ISA_Property_Create ("fp_unpredictable_latency");
  Instruction_Group (fp_unpredictable_latency,
		     TOP_ldc1,
		     TOP_ldxc1,
		     TOP_lwc1,
		     TOP_lwxc1,
		     TOP_dmtc1,
		     TOP_luxc1,
		     TOP_UNDEFINED);

  fp_stall_hazard = ISA_Property_Create ("fp_stall_hazard");
  Instruction_Group (fp_stall_hazard,
		     TOP_madd_d,
		     TOP_msub_d,
		     TOP_mul_d,
		     TOP_nmadd_d,
		     TOP_nmsub_d,
		     TOP_recip_d,
		     TOP_rsqrt_d,
		     TOP_UNDEFINED);

  fp_round_hazard = ISA_Property_Create ("fp_round_hazard");
  Instruction_Group (fp_round_hazard,
		     TOP_madd_d,
		     TOP_msub_d,
		     TOP_mul_d,
		     TOP_nmadd_d,
		     TOP_nmsub_d,
		     TOP_recip_d,
		     TOP_rsqrt_d,
		     TOP_sqrt_d,
		     TOP_UNDEFINED);

  fp_round_trigger = ISA_Property_Create ("fp_round_trigger");
  Instruction_Group (fp_round_trigger,
		     TOP_abs_d,
		     TOP_abs_s,
		     TOP_add_d,
		     TOP_add_s,
		     TOP_c_eq_s,
		     TOP_c_f_s,
		     TOP_c_le_s,
		     TOP_c_lt_s,
		     TOP_c_nge_s,
		     TOP_c_ngl_s,
		     TOP_c_ngt_s,
		     TOP_c_ole_s,
		     TOP_c_olt_s,
		     TOP_c_seq_s,
		     TOP_c_sf_s,
		     TOP_c_ueq_s,
		     TOP_c_ule_s,
		     TOP_c_ult_s,
		     TOP_c_un_s,
		     TOP_ceil_l_d,
		     TOP_ceil_l_s,
		     TOP_ceil_w_d,
		     TOP_ceil_w_s,
		     TOP_cvt_d_l,
		     TOP_cvt_d_s,
		     TOP_cvt_d_w,
		     TOP_cvt_l_d,
		     TOP_cvt_l_s,
		     TOP_cvt_s_d,
		     TOP_cvt_s_l,
		     TOP_cvt_s_w,
		     TOP_cvt_w_d,
		     TOP_cvt_w_s,
		     TOP_div_d,
		     TOP_div_s,
		     TOP_floor_l_d,
		     TOP_floor_l_s,
		     TOP_floor_w_d,
		     TOP_floor_w_s,
		     TOP_madd_s,
		     TOP_mov_d,
		     TOP_mov_s,
		     TOP_movn_d,
		     TOP_movn_s,
		     TOP_movt_d,
		     TOP_movt_s,
		     TOP_movz_d,
		     TOP_movz_s,
		     TOP_msub_s,
		     TOP_mul_s,
		     TOP_neg_d,
		     TOP_neg_s,
		     TOP_nmadd_s,
		     TOP_nmsub_s,
		     TOP_recip_s,
		     TOP_round_l_d,
		     TOP_round_l_s,
		     TOP_round_w_d,
		     TOP_round_w_s,
		     TOP_rsqrt_s,
		     TOP_sqrt_d,
		     TOP_sqrt_s,
		     TOP_sub_d,
		     TOP_sub_s,
		     TOP_trunc_l_d,
		     TOP_trunc_l_s,
		     TOP_trunc_w_d,
		     TOP_trunc_w_s,
		     TOP_UNDEFINED);

  ISA_Properties_End();
  return 0;
}
