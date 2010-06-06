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
 * Module: language.h
 * $Revision$
 * $Date$
 * $Author$
 * $Source$
 *
 * Revision history:
 *  26-Feb-96 - Original Version (extracted from config.h)
 *
 * Description:
 *
 * Define an enumeration type for the language being compiled.  Used
 * primarily by config.c, but extracted for use in other contexts
 * (e.g. IPA).
 *
 * ====================================================================
 * ====================================================================
 */

#ifndef language_INCLUDED
#define language_INCLUDED

#ifdef _KEEP_RCS_ID
static char *language_rcs_id = "$Source$ $Revision$";
#endif /* _KEEP_RCS_ID */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***** Language being compiled *****/
typedef enum {
  LANG_UNKNOWN,
  LANG_F77,
  LANG_F90,
  LANG_KR_C,	/* Kernighan & Richie C */
  LANG_ANSI_C,	/* ANSI standard C */
  LANG_CPLUS,	/* simple C++ */
  LANG_DELTA,	/* Delta C++ */
  LANG_COUNT	/* Must be last */
} LANGUAGE;

#ifdef __cplusplus
}
#endif /* __cplusplus */
    
#endif /* xxx_INCLUDED */

