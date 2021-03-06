/*
 *  Copyright (C) 2008 PathScale, LLC.  All Rights Reserved.
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


#ifndef	cg_INCLUDED
#define	cg_INCLUDED



#include "opt_alias_interface.h"
#include "region_util.h"

#include "wn.h"
#include "dwarf_DST_mem.h"
#include "symtab.h"
#include "tn.h"
#include "tn_map.h"            /* needed for TN_To_PREG_Map */


/* type stubs - can't include appropriate files since
 * this file is included by be/com/driver.c ...
 */
struct bb;

extern BOOL PU_Has_Calls;
extern BOOL PU_References_GP;

#ifdef KEY
extern BOOL PU_Has_Exc_Handler;
extern BOOL PU_Has_Nonlocal_Goto_Target;
extern BOOL CG_file_scope_asm_seen;
#endif

#ifdef TARG_X8664
extern BOOL PU_References_GOT; // for -m32 -fpic
#endif

extern BOOL CG_PU_Has_Feedback;

/* WOPT alias manager */
extern struct ALIAS_MANAGER *Alias_Manager;

#ifdef __cplusplus
extern "C" {
#endif

/* Generate code for the given REGION or PU
   The results are returned in a WHIRL statement-list
   attached to a BLOCK. */
extern WN *CG_Generate_Code (
  WN            *rwn,           /* The REGION to compile */
  struct ALIAS_MANAGER *am,	/* WOPT alias manager */
  DST_IDX	pu_dst,		/* dst_idx for pu, (NULL for region) */
  BOOL		region		/* processing PU or region */
);

extern void CG_PU_Initialize( WN *wn );
extern void CG_PU_Finalize( void );

extern void CG_Change_Elf_Symbol_To_Undefined (ST* st);

/* initialization for statics to keep the ST for locals generated
   when creating quad glue code for regions */
extern void Init_gen_quad_preg(void);

#ifdef __cplusplus
}
#endif

/* Print the IR for a program unit after a phase, if enabled: */
extern void Trace_IR (
  INT phase,		/* Phase after which to print */
  const char *pname,	/* Print name of phase */
  struct bb *bb		/* BB to print, or NULL */
);

/* Print IR, ST, TNs for a program unit after a phase, if enabled: */
extern void Check_for_Dump (
  INT phase,	/* Phase after which to print */
  struct bb *bb	/* BB to print, or NULL */
);

/* Overloaded version of standard Get_Trace with additional bb
 * parameter. If bb parameter is non-NULL and BBs were explicitly 
 * enabled for tracing, return FALSE unless the given BB is in the 
 * enabled list.
 */
extern BOOL Get_Trace ( INT func, INT arg, struct bb *bb );

/* memory pools for allocations local to processing a region */
extern MEM_POOL MEM_local_region_pool;
extern MEM_POOL MEM_local_region_nz_pool;

extern RID *Current_Rid;

#endif /* cg_INCLUDED */
