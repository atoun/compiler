/*
 * Copyright 2005, 2006 PathScale, Inc.  All Rights Reserved.
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
 * Module: em_elf.c
 * $Revision: 1.5 $
 * $Date: 05/11/07 20:34:18-08:00 $
 * $Author: fchow@fluorspar.internal.keyresearch.com $
 * $Source: common/com/x8664/SCCS/s.targ_em_elf.cxx $
 *
 * Description:
 *
 * Generate the elf headers and sections for the object file.
 *
 * ====================================================================
 * ====================================================================
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "elf_stuff.h"
#include <elfaccess.h>
#include "libelf/libelf.h"
#include <stamp.h>
#if HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <cmplrs/leb128.h>
#include <cmplrs/elf_interfaces.h>
#include <sys/unwindP.h>

#define	USE_STANDARD_TYPES 1
#include "defs.h"
#include "erlib.h"
#include "erglob.h"
#include "config.h"
#include "targ_const.h"
#include "glob.h"
#include "config.h"
#include "config_elf_targ.h"
#include "em_elf.h"

INT GP_DISP = 0;

pSCNINFO Interface_Scn;

void Em_Write_Reginfo (
    Elf64_Addr gprvalue, 
    Elf64_Word gprmask, 
    Elf64_Word fprmask,
    BOOL pure_abi)
{
    /* should we put reginfo into the .options section? */
    /* A:  no */
    return;

#if 0
    if (Sixtyfour_Bit) {
	if (Get_Elf_Target_Machine() == EM_IA_64) {
	  Elf_IA64_RegInfo reginfo_ia64;
	  reginfo_ia64.ri_gp_value = gprvalue;
	  Em_Add_New_Option (ODK_IA64_REGINFO, SHN_UNDEF,0, &reginfo_ia64, sizeof(reginfo_ia64));
	}
    }
    else {
    	ErrMsg(EC_Assertion, __FILE__, __LINE__, "NYI - n32 reginfo");
    }
#endif
}

/* Add new entry to the .options section. */
void
Em_Add_New_Option (
    Elf32_Byte option_kind, 
    Elf32_Section option_section, 
    Elf32_Word option_info, 
    void *buffer, 
    Elf32_Byte length)
{
    Elf_Options option;
#if 0 // don't put out options period. Its not defined in ABI
    if (Options_Scn == NULL) {
	Options_Scn = Em_New_Section (IA64_OPTIONS, SHT_IRIX_OPTIONS, 
			  SHF_ALLOC | SHF_IRIX_NOSTRIP, 0, 
			  ELF64_FSZ_XWORD);
    }
    option.kind = option_kind;
    option.size = sizeof (Elf_Options);
    option.section = option_section;
    option.info = option_info;
    option.size += Roundup (length, 8);
    Em_Add_Bytes_To_Scn (Options_Scn, &option, sizeof(option), ELF64_FSZ_WORD);
    if (length != 0) {
      Em_Add_Bytes_To_Scn (Options_Scn, buffer, length, ELF64_FSZ_WORD);
    }
#else
    return;
#endif
}


/* Add a new event to the .events section. The operand1 and operand2
   parameters are used to pass additional information needed for 
   certain event kinds.

     EK_IF_ENTRY:		operand1 is offset in interface scn.

     EK_FCALL_LOCAL,
     EK_FCALL_EXTERN,
     EX_FCALL_EXTERN_BIG:	operand1 is elf symbol index of called proc.
*/
void
Em_Add_New_Event (
    Elf64_Word ev_kind, 
    Elf64_Word ev_ofst, 
    Elf64_Word operand1,
    Elf64_Word operand2,
    Elf64_Word operand3,
    pSCNINFO scn)
{
  return;
}


/* Add a new entry to the .contents section. */
void
Em_Add_New_Content (
    Elf64_Word con_kind, 
    Elf64_Xword con_ofst, 
    Elf64_Word operand1,
    Elf64_Word operand2,
    pSCNINFO scn)
{
  return;
}

void
Em_End_Unwind (FILE *trace_file, pSCNINFO text_scn)
{
}

void
Em_Cleanup_Unwind (void)
{
}

