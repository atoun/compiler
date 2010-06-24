/*
 * Copyright 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/*

  Copyright (C) 2000,2004 Silicon Graphics, Inc.  All Rights Reserved.

   Path64 is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation version 2.1

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



#include "config.h"
#include "pro_incl.h"

/*---------------------------------------------------------------
	This routine deallocates all memory, and does some 
	finishing up
-----------------------------------------------------------------*/
 /*ARGSUSED*/ Dwarf_Unsigned
dwarf_producer_finish(Dwarf_P_Debug dbg, Dwarf_Error * error)
{
    if (dbg->de_version_magic_number != PRO_VERSION_MAGIC) {
	DWARF_P_DBG_ERROR(dbg, DW_DLE_IA, DW_DLV_NOCOUNT);
    }

    dwarf_p_dealloc((void *) dbg, 0);
    return 0;
}
