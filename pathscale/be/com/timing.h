/*
 * Copyright (C) 2008 Advanced Micro Devices, Inc.  All Rights Reserved.
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


#ifndef timing_INCLUDED
#define timing_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================
 * ====================================================================
 *
 * Module: timing.h
 * $Revision$
 * $Date$
 * $Author$
 * $Source$
 *
 * Revision history:
 *  06-Feb-90 - Original Version
 *  01-Feb-91 - Copied for TP/Muse
 *
 * Description:
 *
 * This module provides the interface for phase/function timing in the
 * Muse compiler.  It is based on the program-independent functions
 * provided by util/resource.h.
 *
 * ====================================================================
 * ====================================================================
 */


#ifdef _KEEP_RCS_ID
static char *timing_rcs_id = "$Source$ $Revision$";
#endif /* _KEEP_RCS_ID */

/* Provide identifiers for the various time accumulators supplied: */
typedef enum {
	T_BE_Comp,		/* time for whole back-end */
	T_BE_PU_CU,		/* BE PU processing -- per compilation unit */
	T_BE_PU_Comp,		/* 		       per compilation */
	T_ReadIR_CU,		/* Reading IR */
	T_ReadIR_Comp,	      
	T_Lower_CU,		/* Lowering WHIRL */
	T_Lower_Comp,	      
	T_ORI_CU,		/* Olimit Region Insertion */
	T_ORI_Comp,	      

	T_Preopt_CU,		/* Pre-optimizer */
	T_Preopt_Comp,
	T_Wopt_CU,		/* Global optimizer */
	T_Wopt_Comp,

	T_IPA_S_CU,		/* IPA summary phase */
	T_IPA_S_Comp,

	T_LNO_CU,		/* Loop Nest Optimization */
	T_LNO_Comp,	
	T_LNOParentize_CU,	/* Parentization in LNO */
	T_LNOParentize_Comp,	
	T_LNOAccess_CU,		/* Build access arrays in LNO */
	T_LNOAccess_Comp,	
	T_LNOBuildDep_CU,       /* Build array dep graph in LNO */
	T_LNOBuildDep_Comp,	

	T_W2C_CU,		/* Whirl to C translation */
	T_W2C_Comp,	
	T_W2F_CU,		/* Whirl to Fortran translation */
	T_W2F_Comp,	

	T_CodeGen_CU,		/* Code generator total */
	T_CodeGen_Comp,	

	T_GLRA_CU,		/* Global register allocation */
	T_GLRA_Comp,	
	T_Expand_CU,		/* Code expansion */
	T_Expand_Comp,	
	T_Localize_CU,		/* Localize */
	T_Localize_Comp,
	T_SWpipe_CU,		/* Software pipelining */
	T_SWpipe_Comp,	
	T_GCM_CU,		/* Global code motion */
	T_GCM_Comp,	
	T_EBO_CU,		/* Extended Block Optimization */
	T_EBO_Comp,	
	T_CFLOW_CU,		/* Control Flow Optimization */
	T_CFLOW_Comp,	
	T_Loop_CU,		/* CG Loop */
	T_Loop_Comp,	
	T_Freq_CU,		/* BB frequency */
	T_Freq_Comp,	
	T_HBF_CU,		/* HyperBlock Formation */
	T_HBF_Comp,	
	T_Sched_CU,		/* HyperBlock Scheduling */
	T_Sched_Comp,	
	T_THR_CU,		/* Tree-Height Reduction Phase */
	T_THR_Comp,	
	T_LRA_CU,		/* Local Register Allocation */
	T_LRA_Comp,	
	T_GRA_CU,		/* Register allocation */
	T_GRA_Comp,	
        T_GRU_CU,		/* Fuse Spills */
	T_GRU_COMP,
	T_Emit_CU,		/* Code emission */
	T_Emit_Comp,	
	T_Region_Finalize_CU,	/* Region Finalize */
	T_Region_Finalize_Comp,
	T_CalcDom_CU,		/* Calculate_Dominators 
				 * (this time is included in callers time) */
	T_CalcDom_Comp,
	T_LAST			/* Last index defined */
} TIMER_ID;

/***** External routine definitions *****/

/* Initialize timers for entire compilation: */
extern void Initialize_Timing ( BOOL enable );

/* Reset timers for new source file: */
extern void Reset_Timers (void);

/* Start / stop timing accumulator: */
extern void Start_Timer ( INT ID );
extern void Stop_Timer ( INT ID );

/* Read timer time : can only be called when the timer is running,
   i.e., Start_Timer() and Stop_Timer() */
extern double Get_User_Time(INT ID);

/* Report an accumulated delta time: */
extern void Report_Delta_Time ( FILE *file, INT ID );

/* report timing in cg for a region */
extern void Report_CG_Region_Timing (FILE *file, char *name);

/* Finish timing for a single compilation unit, entire compilation: */
extern void Finish_BE_Timing ( FILE *file, char *name );
extern void Finish_Compilation_Timing ( FILE *file, char *source );

#ifdef __cplusplus
}
#endif
#endif /* timing_INCLUDED */
