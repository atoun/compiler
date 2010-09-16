/*
 * Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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


/* $Header: /home/bos/bk/kpro64-pending/include/stamp.h 1.8 04/12/21 14:57:36-08:00 bos@eng-25.internal.keyresearch.com $ */

#ifndef __STAMP_H__
#define __STAMP_H__

#include "main_defs.h"

#ifndef PSC_MAJOR_VERSION
#error PSC_MAJOR_VERSION not defined - check include path for main_defs.h
#endif

#define INCLUDE_STAMP PSC_FULL_VERSION

#endif  /* __STAMP_H__ */
