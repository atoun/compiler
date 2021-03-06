#ifndef names_h
#define names_h
/* 
  Copyright (C) 2000,2004 Silicon Graphics, Inc.  All Rights Reserved.

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

	makename.h   
	$Revision: 1.2 $
	$Date: 2001/01/16 17:47:55 $

	This is for putting strings into stable storage.

        Effectively an strdup() wrapper.

	Rarely called.

	It leaks memory, (the memory
        is never freed) but that seems unimportant since
	use of this is very rare.

*/

char * makename(char *); /* makes a copy of the string in
	a malloc area.  Can never return 0. */

#endif
