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


#ifndef srcpos_INCLUDED
#define srcpos_INCLUDED

#include "config_host.h"	/* for HOST_IS_LITTLE_ENDIAN */

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================
 * ====================================================================
 *
 * Module: srcpos.h
 * $Revision$
 * $Date$
 * $Author$
 * $Source$
 *
 * Description:
 *
 *   Description of the various fields in the source position structure.
 *
 * ====================================================================
 */


/* SRCPOS is defined as a 64bit unsigned integer. This is the declaration 
 * visible to most files. Defining it this way allows existing calls that 
 * pass a INT32 line number to still compile fine. The prototype will 
 * automatically do the conversion from INT32 to the SRCPOS type. The 
 * value of the linenum field is still valid, since it is the lower 
 * 32bits of the source_position structure.
 */

 typedef mUINT64 SRCPOS;


/* The following struct, the union and with SRCPOS and the macros are 
 * for use by routines that need to setup or access the individual 
 * fields of the source position descriptor.
 */
struct srcpos_struct {
  mUINT16	filenum;
  mUINT16	column : 12;
  mUINT16	stmt_begin : 1;
  mUINT16	bb_begin : 1;
  mUINT16	unused : 2;
  mINT32	linenum;
};
#define SRC_POS_SIZE  2       /* 2 * sizeof(mINT32) */

typedef union source_position {
  SRCPOS srcpos;
  struct srcpos_struct t;
  mINT32 fillers[SRC_POS_SIZE];
} USRCPOS;

#define CHECK_SIZE_CONSISTENCY(s) check_assertion(sizeof(s) == (SRC_POS_SIZE*sizeof(INT32)))
#define USRCPOS_clear(s)          ((s).fillers[0] = 0,(s).fillers[1] = 0)

#define USRCPOS_srcpos(s)	((s).srcpos)
#define USRCPOS_filenum(s) 	((s).t.filenum)
#define USRCPOS_column(s) 	((s).t.column)
#define USRCPOS_stmt_begin(s) 	((s).t.stmt_begin)
#define USRCPOS_bb_begin(s) 	((s).t.bb_begin)
#define USRCPOS_linenum(s) 	((s).t.linenum)

#define SRCPOS_clear(s)		((s) = 0)
#define SRCPOS_filenum(s) 	(((USRCPOS *)&(s))->t.filenum)
#define SRCPOS_column(s) 	(((USRCPOS *)&(s))->t.column)
#define SRCPOS_stmt_begin(s) 	(((USRCPOS *)&(s))->t.stmt_begin)
#define SRCPOS_bb_begin(s) 	(((USRCPOS *)&(s))->t.bb_begin)
#define SRCPOS_linenum(s) 	(((USRCPOS *)&(s))->t.linenum)

#if HOST_IS_LITTLE_ENDIAN
#define Srcpos_To_Line(s)	((mINT32)((s)>>32))
#else
#define Srcpos_To_Line(s)	((mINT32)(s))
#endif /* HOST_IS_LITTLE_ENDIAN */

#ifdef __cplusplus
}
#endif
#endif /* srcpos_INCLUDED */
