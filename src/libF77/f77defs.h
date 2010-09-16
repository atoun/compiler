/*

  Copyright 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2.1 of the GNU Lesser General Public License 
  as published by the Free Software Foundation.

   Path64 is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with Path64; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   Special thanks goes to SGI for their continued support to open source

  Contact information:  PathScale, Inc., 2071 Stierlin Court, Suite 200,
  Mountain View CA 94043, USA, or:

  http://www.pathscale.com

  For further information regarding this notice, see:

  http://oss.sgi.com/projects/GenInfo/NoticeExplan

 */

#ifndef _f77_defs_h
#define _f77_defs_h

extern int8_t shft_b(int8_t *m,int8_t *k);
extern int16_t shft_h(int16_t *m,int16_t *k);
extern int32_t shft_l(int32_t *m, int32_t *k);
extern int64_t shft_ll(int64_t *m,int64_t *k);

#endif
