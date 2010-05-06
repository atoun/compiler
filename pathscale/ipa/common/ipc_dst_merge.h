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


/* -*-Mode: c++;-*- (Tell emacs to use c++ mode) */
#ifndef __cplusplus
#error This header must be compiled as C++
#endif

#ifndef IPC_DST_MERGE_included
#define IPC_DST_MERGE_included

#define USE_DST_INTERNALS
#include "dwarf_DST_mem.h"
#include "pu_info.h"
#include "mempool.h"

DST_TYPE IPC_merge_DSTs(pu_info* pu_tree, MEM_POOL* p);

#endif /* IPC_DST_MERGE_included */
