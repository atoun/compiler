//-*-c++-*-
// ====================================================================
// ====================================================================
//
// Module: opt_daVinci.cxx
// $Revision$
// $Date$
// $Author$
// $Source$
//
// ====================================================================
//
// Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.
//
/*
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
//
// ====================================================================
//
// Description:
//
//	interface to DAVINCI, a visualization system for displaying
//	directed graphs.
//
// ====================================================================
// ====================================================================


#include <stdio.h>
#include <signal.h>
#include <sys/types.h>              // for pid_t
#include <sys/wait.h>                   // for waitpid()
#include <unistd.h>                 // for fork(), pipe(), etc.
#include <signal.h>                 // for SIGINT

#define USE_STANDARD_TYPES
#include "defs.h"
#include "mempool.h"                 // for Malloc_Mem_Pool
#include "cxx_memory.h"             // for CXX_NEW

// -- to trace traffic to/from DaVinci use 'Trace' script (gwe).
//    1. put a wrapper script around daVinci:
//       #!/bin/sh
//       exec Trace io daVinci.trace /full-path/daVinci
//    2. make sure the daVinci wrapper script preceeds daVinci itself
//       on PATH.
//    3. run command (be, etc.) as before; look for daVinci.trace
//
// -- TBD: modify DAVINCI class so from_display/to_display are
//         not exported.  then i/o tracing can be done w/o use
//         of 'Trace' and a wrapper script.
//     or  make Trace solid and add it to source tree (somewhere?).

class DAVINCI {
private:
    BOOL display_ok;
    pid_t pid;
    FILE *to_display, *from_display;

    void wait_for (const char *str = "ok\n");
    void cleanup ();
    
public:
    DAVINCI(void);
    FILE *From_display(void)   { return from_display; }
    FILE *To_display(void)     { return to_display; }

}; // daVinci


DAVINCI::DAVINCI(void)
{
#ifdef _WIN32
  fprintf(stderr, "DAVINCI::DAVINCI is unimplemented\n");
#else
  to_display = from_display = NULL;
  
  if (isatty (1) == 0 && isatty (2) == 0)
    return;			    // none of stdout and stderr is a tty
  
  /* now connect to DAVINCI */
  int read_pipe[2], write_pipe[2];
  
  if (pipe (read_pipe) == -1 || pipe (write_pipe) == -1)
    return;
  
  from_display = fdopen (read_pipe[0], "r");
  setbuf (from_display, NULL);
  to_display = fdopen (write_pipe[1], "w");
  setbuf (to_display, NULL);
  
  switch (pid = fork ()) {
  case -1:			    // can't fork
    close (read_pipe[0]);
    close (read_pipe[1]);
    close (write_pipe[0]);
    close (write_pipe[1]);
    return;
    
  case 0:			    // child
    dup2 (write_pipe[0], 0);    // reset stdin, stdout, and stderr
    dup2 (read_pipe[1], 1);
    dup2 (read_pipe[1], 2);
    
    close (write_pipe[0]);	    // close unused pipes.
    close (read_pipe[1]);	    // leave the other pair opened so
    // that DAVINCI does not die when
    // ipa exits.
    
    execlp ("daVinci", "daVinci", "-pipe", NULL);
    fprintf(stderr,"cannot found daVinci.\n");
    exit (1);		    // exec fail, just exit
    
  default:			    // parent
    close (read_pipe[1]);
    close (write_pipe[0]);
    wait_for ();

    if (display_ok) {
      // setup default properties
      fprintf(to_display, "set(font_size(8))\n");
      fprintf(to_display, "set(gap_height(4))\n");
      fprintf(to_display, "set(gap_width(20))\n");
      fprintf(to_display, "menu(view_menu(fit_scale_to_window))\n");
    }
  }
#endif
} 


/* wait for the "ok" message from DAVINCI */
void
DAVINCI::wait_for (const char *str)
{
  char buf[512];
  
  while (fgets (buf, 512, from_display) != NULL) {
    if (strcmp (buf, "ok\n") == 0) {
      display_ok = TRUE;
      return;
    } else {
      switch (buf[0]) {
      case 'n':		    // node* or no_*
      case 'e':		    // edge*
      case 'm':		    // menu*
      case 'a':		    // answer*
	if (strncmp (buf, str, strlen(str)) == 0) {
	  display_ok = TRUE;
	  return;
	}
	break;
      default:
	fputs (buf, stderr);
	cleanup ();
	return;
      }
    }
  }
  
  cleanup ();
  return;
} // DAVINCI::wait_for


void
DAVINCI::cleanup (void)
{
  int stat;
  
  display_ok = FALSE;
//  kill (pid, SIGINT);
//  waitpid (pid, &stat, WNOHANG);  // capture any SIGCHLD so not to
  // confuse ipacom.
  fclose (to_display);
  fclose (from_display);
} // DAVINCI::cleanup


FILE *
Init_daVinci(void)
{
  static DAVINCI *daVinci = NULL;

  if (daVinci == NULL) {
    daVinci = CXX_NEW(DAVINCI(), Malloc_Mem_Pool);
  }
  return daVinci->To_display();
}

FILE *
New_daVinci(void)
{
  DAVINCI *daVinci = CXX_NEW(DAVINCI(), Malloc_Mem_Pool);
  return daVinci->To_display();
}

