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

#ifndef GRA_LOOP_INCLUDED
#define GRA_LOOP_INCLUDED

#include "cg.h"
#include "bb.h"
#include "findloops.h"

// forward declarations
class GRA_BB;

// GRA uses loops to direct live range splitting.  That is, when
// live ranges are being constructed, a loop is only added to the
// live range if a register can be found throughout.  The nesting
// structure is represented by a tree of GRA_LOOP structs.  In
// this way, we can keep track of the register usage and such of inner loops.
class GRA_LOOP {
friend class GRA_LOOP_MGR;
#ifdef TARG_ST
  REGISTER_SET registers_used[ISA_REGISTER_CLASS_MAX_LIMIT+1];
#else
  REGISTER_SET registers_used[ISA_REGISTER_CLASS_MAX+1]; 
#endif
	      // registers used by all blocks and inner loops within the loop
  GRA_LOOP*    parent; 		// parent of this loop, i.e. the next outer loop
  GRA_LOOP*    outermost; 	// outermost loop in this nest
  GRA_LOOP*    next_loop; 	// next in list of loops in nest (outermost first)
  INT          nest_level; 	// nesting level of the loop
  BB*	       loop_head; 	// bb at head of loop
#if defined( KEY) && !defined(TARG_ST)
  LOOP_DESCR   *loop_descr;	// the LOOP_DESCR for this loop
#endif

  void Registers_Used_Init(ISA_REGISTER_CLASS rc) 
				{ registers_used[rc] = REGISTER_SET_EMPTY_SET; }
  void Parent_Set(GRA_LOOP *p)	{ parent = p; }
  void Outermost_Set(GRA_LOOP *p) { outermost = p; }
  void Next_Loop_Set(GRA_LOOP *p) { next_loop = p; }
  void Nest_Level_Set(INT i) 	{ nest_level = i; }
  void Loop_Head_Set(BB *b) 	{ loop_head = b; }
#if defined( KEY) && !defined(TARG_ST)
  void Loop_Descr_Set(LOOP_DESCR *ld)	{ loop_descr = ld; };
  LOOP_DESCR *Loop_Descr(void)	{ return loop_descr; }
#endif

  GRA_LOOP *Make_Loop_Nest(GRA_LOOP *outermost, GRA_LOOP *parent);

public:
  GRA_LOOP(void) {}
  ~GRA_LOOP(void) {}

  // access functions
  REGISTER_SET Registers_Used(ISA_REGISTER_CLASS rc){return registers_used[rc];}
  GRA_LOOP *Parent(void)	{ return parent; }
  GRA_LOOP *Outermost(void)	{ return outermost; }
  GRA_LOOP *Next_Loop(void)	{ return next_loop; }
  INT Nest_Level(void)		{ return nest_level; }
  BB *Loop_Head(void)		{ return loop_head; }

  // non-inlined member functions
  void Make_Register_Used(ISA_REGISTER_CLASS rc, REGISTER reg
#ifndef TARG_ST
                          , BOOL reclaim = FALSE
#endif
                          );

#if defined( KEY) && !defined(TARG_ST)
  REGISTER_SET Registers_Referenced(ISA_REGISTER_CLASS rc);
#endif
};

class GRA_LOOP_MGR {
  BB_MAP _map;
  LOOP_DESCR *cg_loops;

  GRA_LOOP *Create(LOOP_DESCR *ld);

public:
  GRA_LOOP_MGR(void) {}
  ~GRA_LOOP_MGR(void) {}

  void Find_Loops(void);
  void Set_GBB_Loop(GRA_BB *gbb);
};

extern GRA_LOOP_MGR gra_loop_mgr;

#endif
