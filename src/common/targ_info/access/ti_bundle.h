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
 *  Module: ti_bundle.h
 *  $Revision$
 *  $Date$
 *  $Author$
 *  $Source$
 *
 *  Synopsis:
 *
 *      Bundle (or template) accounting package for various clients including
 *	the software pipeliner, local (BB) scheduler, disassembler and 
 *      linker. 
 *
 *  Interface Description:
 *
 *	Exported types:
 *
 *      Exported functions:
 *
 *	    BOOL TI_BUNDLE_Has_Property(
 *		TI_BUNDLE *bundle
 *		ISA_EXEC_UNIT_PROPERTY property
 *              INT *error
 *	    )
 *
 *		Returns TRUE if <bundle> contains an instance of <property>.
 *              If an error occurs, <error> is set to TI_RC_ERROR.
 *
 *	    BOOL TI_BUNDLE_Is_Full(
 *		TI_BUNDLE *bundle
 *              INT *error
 *	    )
 *
 *		Returns TRUE if <bundle> is fully packed with insts. If 
 *              an error occurs, <error> is set to TI_RC_ERROR.
 *
 *	    BOOL TI_BUNDLE_Is_Empty(
 *		TI_BUNDLE *bundle
 *              INT *error
 *	    )
 *
 *		Returns TRUE if <bundle> is empty. If an error occurs, 
 *		<error> is set to TI_RC_ERROR.
 *
 *          INT TI_BUNDLE_Return_Template(
 *              TI_BUNDLE *bundle
 *          )
 *              
 *              Returns the template encoding bit for the slot pattern in
 *              <bundle>.
 *              
 *          void TI_BUNDLE_Clear(
 *              TI_BUNDLE *bundle
 *          )
 *              
 *              Reset the contents of the bundle.
 *
 *	    BOOL TI_BUNDLE_Slot_Available(
 *		TI_BUNDLE  *bundle
 *		ISA_EXEC_UNIT_PROPERTY  property
 *		INT slot
 *	    )
 *
 *              Check to see if 'slot' for 'property' is available for
 *		the given 'bundle'. Returns TRUE if available; FALSE 
 *		otherwise.
 *
 *	    BOOL TI_BUNDLE_Stop_Bit_Available(
 *		TI_BUNDLE  *bundle
 *		INT slot
 *	    )
 *
 *              Check to see if a stop bit can be set for 'slot'
 *		in the given 'bundle'. Returns TRUE if available; FALSE 
 *		otherwise.  
 *
 *	    void TI_BUNDLE_Reserve_Slot(
 *		TI_BUNDLE  *bundle
 *		INT        slot
 *              ISA_EXEC_UNIT_PROPERTY property
 *	    )
 *
 *              Reserve 'slot' for 'property' at the given 'bundle'.
 *
 *	    void TI_BUNDLE_Reserve_Stop_Bit(
 *		TI_BUNDLE  *bundle
 *		INT        slot
 *	    )
 *
 *              Reserve 'slot' position for STOP bit at the given 'bundle'.
 *
 *	    void TI_BUNDLE_Unreserve_Stop_Bit(
 *		TI_BUNDLE  *bundle
 *		INT        slot
 *	    )
 *
 *              Unreserve 'slot' position for STOP bit at the given 'bundle'.
 *
 *
 * ====================================================================
 * ====================================================================
 */

#ifndef ti_bundle_INCLUDED
#define ti_bundle_INCLUDED

#include "topcode.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _KEEP_RCS_ID
static const char ti_bundle_rcs_id[] = "$Source$ $Revision$";
#endif /* _KEEP_RCS_ID */

#ifndef defs_INCLUDED
#define defs_INCLUDED
typedef signed int INT;
typedef signed int INT32;
typedef signed long long INT64;
typedef signed char mINT8;
typedef signed short mINT16;
typedef signed int mINT32;
typedef signed long long mINT64;
typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned char mUINT8;
typedef unsigned short mUINT16;
typedef unsigned int mUINT32;
typedef unsigned long long mUINT64;
typedef int BOOL;
typedef unsigned char mBOOL;
#ifndef TRUE
#define TRUE    ((BOOL) 1)
#endif
#ifndef FALSE
#define FALSE   ((BOOL) 0)
#endif
#if defined(_LANGUAGE_C) && !defined(inline)
#define inline static __inline
#endif
#endif

typedef struct ti_bundle {
  ISA_BUNDLE_INFO *bundle_info; /* exported interface from targ_info */
#ifdef TARG_ST
  BOOL slot_filled[ISA_BUNDLE_MAX_SLOTS];
#else
  BOOL slot_filled[ISA_MAX_SLOTS];  
#endif
} TI_BUNDLE;

/* TI_BUNDLE accessors:
 */

#define TI_BUNDLE_bundle_info(t)            ((t)->bundle_info)
#define TI_BUNDLE_slot_count(t)             ((t)->bundle_info->slot_count)
#define TI_BUNDLE_exec_property(t,i)        ((t)->bundle_info->slot[i])
#define TI_BUNDLE_stop_bit(t,i)             ((t)->bundle_info->stop[i])
#define TI_BUNDLE_slot_filled(t,i)          ((t)->slot_filled[i])
#define TI_BUNDLE_slot_mask(t)              ((t)->bundle_info->slot_mask)
#define TI_BUNDLE_stop_mask(t)              ((t)->bundle_info->stop_mask)
#define TI_BUNDLE_pack_code(t)	    	    ((t)->bundle_info->pack_code)

#define Set_TI_BUNDLE_exec_property(t, i, value) ((t)->bundle_info->slot[i] = \
						  (value))
#define Set_TI_BUNDLE_slot_mask(t, i)       ((t)->bundle_info->slot_mask = i)
#define Set_TI_BUNDLE_stop_mask(t, i)       ((t)->bundle_info->stop_mask = i)
#define Set_TI_BUNDLE_slot_count(t, i)      ((t)->bundle_info->slot_count = i)
#define Set_TI_BUNDLE_stop_bit(t, i, value) ((t)->bundle_info->stop[i] = \
						  (value))
#define Set_TI_BUNDLE_pack_code(t, value)   ((t)->bundle_info->pack_code = \
    	    	    	    	    	    	  (value))

/* Iterator Macros */

#define FOR_ALL_SLOT_MEMBERS(bundle, i) \
     for (i = 0; i < TI_BUNDLE_slot_count(bundle); ++i)

#ifdef TARG_ST
#define FOR_ALL_SLOT_MEMBERS_IN_REVERSE(bundle, i) \
     for (i = TI_BUNDLE_slot_count(bundle) - 1; i >= 0; --i)
#endif

inline BOOL
TI_BUNDLE_Stop_Bit_Present(TI_BUNDLE *bundle) {
  INT i;
  for (i = 0; i < TI_BUNDLE_slot_count(bundle) - 1; ++i) {
    if (TI_BUNDLE_stop_bit(bundle, i)) return TRUE;
  }
  return FALSE;
}

#ifdef TARG_ST
TARGINFO_EXPORTED extern void TI_BUNDLE_initialize(int max_slot);
#endif
extern BOOL TI_BUNDLE_Has_Property(
  TI_BUNDLE *bundle,
  ISA_EXEC_UNIT_PROPERTY property,
  INT *error
);

extern BOOL TI_BUNDLE_Is_Full(
  TI_BUNDLE *bundle,
  INT *error
);

extern BOOL TI_BUNDLE_Is_Empty(
  TI_BUNDLE *bundle,
  INT *error
);

extern INT TI_BUNDLE_Return_Template(
  TI_BUNDLE *bundle
);

extern void TI_BUNDLE_Clear(
  TI_BUNDLE *bundle
);

extern BOOL TI_BUNDLE_Slot_Available(
  TI_BUNDLE  *bundle,
  ISA_EXEC_UNIT_PROPERTY  property,
  INT        slot
);

extern BOOL TI_BUNDLE_Stop_Bit_Available(
  TI_BUNDLE  *bundle,
  INT        slot
);

#ifdef TARG_ST
TARGINFO_EXPORTED extern ISA_EXEC_MASK TI_BUNDLE_Set_Slot_Mask_Property(
  ISA_EXEC_MASK slot_mask,
  INT slot,
  ISA_EXEC_UNIT_PROPERTY property
);

TARGINFO_EXPORTED extern ISA_EXEC_UNIT_PROPERTY TI_BUNDLE_Get_Slot_Mask_Property(
  ISA_EXEC_MASK slot_mask,
  INT slot
);
#endif


extern void TI_BUNDLE_Reserve_Slot(
  TI_BUNDLE  *bundle,
  INT slot,
  ISA_EXEC_UNIT_PROPERTY property
);

extern void TI_BUNDLE_Reserve_Stop_Bit(
  TI_BUNDLE  *bundle,
  INT slot
);

extern void TI_BUNDLE_Unreserve_Stop_Bit(
  TI_BUNDLE  *bundle,
  INT slot
);

#ifdef __cplusplus
}
#endif
#endif /* ti_res_res_INCLUDED */

