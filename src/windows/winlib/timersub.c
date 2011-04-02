/*
 * Copyright (C) 2010. PathScale Inc. All Rights Reserved.
 */

/*

   Path64 is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation version 3

   Path64 is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with Path64; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

*/

#include <sys/time.h>

void
timersub(struct timeval *a, struct timeval *b, struct timeval *res)
{
    res->tv_usec = a->tv_usec - b->tv_usec;
    res->tv_sec = a->tv_sec - b->tv_sec;
    if (res->tv_usec > a->tv_usec) {
        res->tv_sec--;
        res->tv_usec += 1000;
    }
}

