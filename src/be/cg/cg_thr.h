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


/* ====================================================================
* ====================================================================
*
* Module: cg_thr.h
* $Revision: 1.5 $
* $Date: 04/12/21 14:57:05-08:00 $
* $Author: bos@eng-25.internal.keyresearch.com $
* $Source: /home/bos/bk/kpro64-pending/be/cg/SCCS/s.cg_thr.h $
*
* Description:
*
* Interface to the CG Tree-Height Reduction Phase.
*
* ====================================================================
* ====================================================================
*/

#ifndef cg_thr_INCLUDED
#define cg_thr_INCLUDED

#include <list>

// Exported interfaces
#define THR_NONE                                       0x0000
#define THR_DATA_SPECULATION_NO_RB                     0x0001
#define THR_DATA_SPECULATION_RB                        0x0002
#define THR_DATA_SPECULATION_NO_RB_CLEANUP             0x0004
#define THR_ALL                                        0x1111

typedef UINT16 THR_TYPE;

extern BOOL Trace_THR;
extern void Remove_Unnecessary_Check_Instrs(BB *bb);

struct ARC;

// Misc flags

// Accessors:

// ====================================================================
//
// CG_THR:
//
// This class provides the infra-structure to perform various tree-height
// reduction tasks.
//
// ====================================================================
class CG_THR {
private:
  // private functions:
  MEM_POOL     _thr_pool;
  MEM_POOL     _thr_map_pool;
  BB           *_bb;
  THR_TYPE     _thr_type;
  BOOL         _thr_before_regalloc;
  BB_MAP       _thr_map;
  std::list<ARC*>   _candidate_list;
  BOOL         _chk_instr_inserted;

public:

  // Constructor/Destructors:

  CG_THR ();
  ~CG_THR();

  //  Set members

  // Member access functions:

  // Exported functions:
  void Init (BB*, THR_TYPE, BOOL);
  void Perform_THR();
  BOOL OP_Has_Restrictions(OP*, OP*, BOOL);
  void Check_THR_Profitability();
  void Perform_THR_Code_Generation();
};


// Other Exported utility routines

// Trace Utility routines

#endif /* cg_thr_INCLUDED */

