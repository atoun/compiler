/*
 *  Copyright (C) 2008 PathScale, LLC.  All Rights Reserved.
 */

/*
 * Copyright 2002, 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
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


#include "basic.h"
#include "file_def.h"

/* drops path prefix in string; result points inside old string */
extern char *drop_path (char *s);

/* drops the last component of the path, leaving only the directory */
extern char *directory_path (char *s);

/* check whether file exists */
extern boolean file_exists (char *path);

/* check whether file exists and has any executable bits set */
extern boolean is_executable (char *path);

/* check whether is a directory */
extern boolean is_directory (char *path);

/* check whether is a directory */
extern boolean want_directory (char *path);

/* check whether directory is writable */
extern boolean directory_is_writable (char *path);

/* get current working directory */
extern char *get_cwd (void);

/* save the program name in case get_executable_dir needs it */
extern void file_utils_set_program_name(char *name);

#ifdef KEY
/* get the saved program name */
extern char *file_utils_get_program_name (void);
#endif

/* get the directory containing the executable */
extern char *get_executable_dir (void);

/* Copy content of file to stdout. */
extern void dump_file_to_stdout (char *filename);
