/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 PathScale Inc.  All Rights Reserved.
 */
/* 
 *

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

//*************************************************************
// the driver for pathgcc front end
//  -- transform spin to whirl through libspin
//*************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cmplrs/rcodes.h>
#include "gspin-wgen-interface.h" 
#include "defs.h"
#include "glob.h"
#include "erglob.h"
#include "err_host.tab"
#include "config.h"
#include "file_util.h" // for Last_Pathname_Component
#include "wgen_misc.h"

extern BOOL List_Enabled;
extern INT Opt_Level;
extern BOOL Enable_WFE_DFE;

#ifndef TARG_MIPS
BOOL TARGET_64BIT = TRUE;
#else
BOOL TARGET_64BIT = FALSE;  // 11953: On MIPS, n32 is default abi
#endif

BOOL wgen_invoke_inliner = FALSE;
int lineno = 0;
char *Spin_File_Name = NULL;
FILE *Spin_File = NULL;
BOOL flag_no_common = FALSE;
int pstatic_as_global = 0;
int key_exceptions = 0;
BOOL opt_regions = 0;
BOOL lang_cplus = FALSE;
BOOL c_omit_external = TRUE;
#ifdef FE_GNU_4_2_0
BOOL enable_cxx_openmp = TRUE;
#endif
gs_t program;
/*       MAX_DEBUG_LEVEL        2  :: Defined in flags.h */
# define DEF_DEBUG_LEVEL        0
INT Debug_Level = DEF_DEBUG_LEVEL;	/* -gn: debug level */
//zwu
int wgen_pic;
extern void WGEN_Weak_Finish(void);
extern void WGEN_Expand_Top_Level_Decl(gs_t);
extern void WGEN_Expand_Defers(void);
extern void WGEN_Expand_Decl(gs_t, BOOL);
#ifdef KEY
extern void WGEN_Alias_Finish(void);
#endif

//*******************************************************
// Process the command line arguments.
//*******************************************************
void
Process_Command_Line(INT argc, char **argv)
{
  INT i;
  char *cp;

	//zwu
	wgen_pic = 0;

  for (i = 1; i < argc; i++) {
      if ( argv[i] != NULL && *(argv[i]) == '-' ) {
	  cp = argv[i]+1;	    /* Pointer to next flag character */

	  switch ( *cp++ ) {

	  case 'f':		    /* file options */
	      if (*cp == 0)
		  ;
	      else if (*(cp+1) != ',' && *(cp+1) != ':')
		  ;
	      else {
		  switch (*cp) {
		  case 'f':
		      Feedback_File_Name = cp + 2;
		      break;
		  case 'l':	    /* listing file */
		      List_Enabled = TRUE;
		      Lst_File_Name = cp + 2;
		      break;

		  case 't':	    /* Error file: */
		      Trc_File_Name = cp + 2;
		      break;

		  case 'B':	    /* WHIRL file */
		      Irb_File_Name = cp + 2;
		      break;

		  case 'S':	   /* Spin file */
		      Spin_File_Name = cp + 2;
		      break;
		  case 'i':
			wgen_pic = 1;
		  default:
		      break;
		  }
	      }
	      break;

	  case 'O':
	      if (strncmp(cp, "PT:", 3) == 0) 
		Process_Command_Line_Group (cp-1, Common_Option_Groups);
	      break;
	    
	  case 'v':
	      Show_Progress = TRUE;
	      break;

	  default:		    /* What's this? */
	      break;
	  }
      } 
  }
}

//*******************************************************
// Process the cc1 command line arguments.
//*******************************************************
void
Process_Cc1_Command_Line(gs_t arg_list)
{
  INT i, j;
  char *cp;
  INT Src_Count = 0;
  BOOL opt_set = FALSE;
  INT argc = gs_length(arg_list);
  char *argv;

  // determine if it is C or C++ and set lang_cplus accordingly
  argv = gs_s(gs_index(arg_list, 0));
  char *command = Last_Pathname_Component(argv);
//printf("%s\n", command);
#ifdef FE_GNU_4_2_0
  lang_cplus = !strcmp(command, FN_EXE("cc1plus42"));
#else
  lang_cplus = !strcmp(command, FN_EXE("cc1plus"));
#endif

  if (lang_cplus)
    key_exceptions = 1;

  for (i = 1; i < argc; i++) {
      argv = gs_s(gs_index(arg_list, i));
//    printf("%s\n", argv);
      if ( *argv == '-' ) {
	  cp = argv+1;	    /* Pointer to next flag character */

	  switch ( *cp++ ) {

	  case 'a':
	      if (!strcmp( cp, "uxbase" )) 
		i++;
	      break;

	  case 'd':
	      if (!strcmp( cp, "umpbase" )) 
	      {
		i++;
		Orig_Src_File_Name = gs_s(gs_index(arg_list, i));
	      }
	      break;

	  case 'e':
	      if (lang_cplus && !strcmp( cp, "xceptions" ))
		key_exceptions = TRUE;
	      break;

	  case 'f':
	      if (!strcmp( cp, "no-exceptions" )) {
		key_exceptions = FALSE;
	      }
	      else if (lang_cplus && !strcmp( cp, "exceptions" )) {
		key_exceptions = TRUE;
	      }
	      else if (!strcmp( cp, "no-gnu-exceptions")) {
		// GNU exceptions off, turn off exception here also.
		key_exceptions = FALSE;
	      }
	      else if (!lang_cplus && !strcmp( cp, "no-c-omit-external")) {
		c_omit_external = FALSE;
	      }
	      else if (!lang_cplus && !strcmp( cp, "c-omit-external")) {
		c_omit_external = TRUE;
	      }
#ifdef FE_GNU_4_2_0
	      else if (!strcmp( cp, "no-cxx-openmp")) {
	        enable_cxx_openmp = FALSE;
	      }
	      else if (lang_cplus && !strcmp( cp, "cxx-openmp")) {
	        enable_cxx_openmp = TRUE;
	      }
#endif
	      break;

	  case 'g':		    /* Debug level: */
	      Debug_Level = Get_Numeric_Flag (&cp, 0, MAX_DEBUG_LEVEL, 2,
					      argv);
	      if (Debug_Level > 1 && !opt_set)
		  Opt_Level = 0;
	      break;

	  case 'i':
	      if (!strcmp( cp, "prefix" )) 
		i++;
	      break;

	  case 'm':
#ifndef TARG_MIPS
	      if (!strcmp( cp, "32" )) {
		TARGET_64BIT = FALSE;
	      }
	      else if (!strcmp( cp, "64" )) {
		TARGET_64BIT = TRUE;
	      }
	      else if (!strncmp( cp, "regparm=", 8 )) {
	        cp += 8;
	        Reg_Parm_Count = Get_Numeric_Flag (&cp, 0, 3, 0, argv ); 
	      }
	      else if (!strcmp( cp, "sseregparm" )) {
	        SSE_Reg_Parm = TRUE;
	      }
#else
	      // 11953: MIPS expects -mabi=n32 or -mabi=64
	      if (!strcmp( cp, "abi=n32" )) {
		TARGET_64BIT = FALSE;
	      }
	      else if (!strcmp( cp, "abi=64" )) {
		TARGET_64BIT = TRUE;
	      }
#endif
	      break;

	  case 'o':
	      if (*cp == 0)
		i++;
	      break;

	  case 'p':
	      if (!strcmp( cp, "static_as_global" )) {
		pstatic_as_global = TRUE;
	      }
	      break;
      
	  case 'O':		    /* Optimization level: */
	      Opt_Level = Get_Numeric_Flag (&cp, 0, MAX_OPT_LEVEL,
					    DEF_O_LEVEL, argv ); 
	      opt_set = TRUE;
	      break;

	  case 's':
	      if (!strcmp( cp, "pinfile" )) 
		i++;
	      break;
	    
	  case 'v':
	      Show_Progress = TRUE;
	      break;
	    
	  default:		    /* What's this? */
	      break;
	  }
      } else if (argv != NULL) {
	  Src_Count++;
	  FmtAssert(Src_Count == 1,
	  	    ("wgen passed more than one source file in command line"));
	  Src_File_Name = argv;
	  if (Orig_Src_File_Name == NULL)
	    Orig_Src_File_Name = argv;
      } 
  }
}


//*******************************************************
// WGEN driver
//*******************************************************
int
main ( INT argc, char **argv, char **envp)
{
      INT error_count, sorry_count;
      BOOL need_inliner;
      struct stat sbuf;
      int st;

      Set_Error_Tables ( Phases, host_errlist );
      Process_Command_Line(argc, argv);

      st = stat(Spin_File_Name, &sbuf);
      if (st == -1 && (errno == ENOENT || errno == ENOTDIR))
	printf ("wgen: file %s does not exist\n", Spin_File_Name);
      else if ((program = gs_read_file (Spin_File_Name)) != (gs_t) NULL) {
//	gs_dump(program);
	
	Process_Cc1_Command_Line(gs_cc1_command_line_args(program));

	WGEN_Init(argc, argv, envp);

	WGEN_File_Init(argc, argv);

	gs_t list = gs_operand(program, GS_PROGRAM_DECLARATIONS);
	// in bug 10185, first list node is  NULL, so skip first node
	if (gs_code(list) != EMPTY)
	  list = gs_operand(list, 1);
	for (; gs_code(list) != EMPTY; list = gs_operand(list, 1)) {
	  gs_t decl = gs_operand(list, 0);
	  WGEN_Expand_Top_Level_Decl(decl);
#ifdef KEY
	  WGEN_Expand_Defers();
#endif
	}

#ifdef KEY
	if (!lang_cplus)
	  WGEN_Alias_Finish();
#endif
	WGEN_Weak_Finish();
	WGEN_File_Finish ();
	WGEN_Finish ();
      }
      else
	 printf ("wgen: libspin returned (gs_t) NULL.\n");

      WGEN_Check_Errors (&error_count, &sorry_count, &need_inliner);
      if (error_count)
	      Terminate (RC_INTERNAL_ERROR) ;
      if (need_inliner &&
	  ((!Enable_WFE_DFE) || (Opt_Level > 1)))
	      exit ( RC_NEED_INLINER );
      exit (RC_OKAY);
}
