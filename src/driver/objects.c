/*
   Copyright (C) 2010 PathScale Inc. All Rights Reserved.
*/
/*
 * Copyright (C) 2007, 2008, 2009 Pathscale, LLC.  All Rights Reserved.
 */

/*
 *  Copyright (C) 2007. QLogic Corporation. All Rights Reserved.
 */

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


#if HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include "basic.h"
#include "string_utils.h"
#include "objects.h"
#include "option_names.h"
#include "options.h"
#include "option_seen.h"
#include "opt_actions.h"
#include "get_options.h"
#include "errors.h"
#include "lang_defs.h"
#include "file_names.h"
#include "file_utils.h"
#include "main_defs.h"
#include "run.h"

#include "sys/elf_whirl.h"

string_list_t *objects;
string_list_t *lib_objects;
static string_list_t *cxx_prelinker_objects;
static string_list_t *ar_objects; 
static string_list_t *library_dirs;

#ifdef TARG_MIPS
char * sysroot_path_n32 = NULL;
char * sysroot_path_64 = NULL;
#endif
char * sysroot_path = NULL;

static int check_for_whirl(char *name);

void
init_objects (void)
{
 	objects = init_string_list();
 	lib_objects = init_string_list();
 	cxx_prelinker_objects = init_string_list();
 	ar_objects = init_string_list();
	library_dirs = init_string_list();
}


/* whether option is an object or not */
boolean
is_object_option (int flag)
{
	switch (flag) {
	case O_object:
	case O_l:
	case O_all:
        case O__whole_archive:
        case O__no_whole_archive:
        case O_WlC:
		return TRUE;
	default:
		return FALSE;
	}
}

struct prof_lib 
{
    const char *name;
    int always;
};

static struct prof_lib prof_libs[] = {
    /* from glibc-profile */
    { "BrokenLocale", 0 },
    { "anl", 0 },
    { "c", 0 },
    { "crypt", 0 },
    { "dl", 0 },
    { "m", 0 },
    { "nsl", 0 },
    { "pthread", 0 },
    { "resolv", 0 },
    { "rpcsvc", 0 },
    { "rt", 0 },
    { "util", 0 },
    /* from our own libraries */
    { "instr", 1 },
    { "mpath", 1 },
    { "mv", 1 },
    { PSC_NAME_PREFIX "fortran", 1 },
    { "pscrt", 1 },
    { NULL, 0 },
};

int prof_lib_exists(const char *lib)
{
    char *path;
    int exists;
    asprintf(&path, "%s/lib%s_p.a", abi == ABI_N32 ? "/usr/lib" : "/usr/lib64",
	     lib);
    exists = access(path, R_OK) == 0;
    free(path);
    return exists;
}

void add_library(string_list_t *list, const char *lib)
{
    if (option_was_seen(O_profile)) {
	for (struct prof_lib *l = prof_libs; l->name; l++) {
	    if (strcmp(l->name, lib) != 0)
		continue;
	    if (!l->always && !prof_lib_exists(lib))
		continue;
	    lib = concat_strings(lib, "_p");
	}
    }

    add_string(list, concat_strings("-l", lib));
}

/* library list options get put in object list,
 * so order w.r.t. libraries is preserved. */
void
add_object (int flag, char *arg)
{
    /* cxx_prelinker_object_list contains real objects, -objectlist flags. */
	switch (flag) {
	case O_l:
		/* when -lm, implicitly add extra math libraries */
		if ((strcmp(arg, "m") == 0 ||
		     strcmp(arg, "mpath") == 0)	// bug 5184
		    && ! option_was_seen(O_fbootstrap_hack)) {
			/* add -lmv -lmblah */
			if (xpg_flag && invoked_lang == L_f77) {
			  add_library(lib_objects, "mv");
#ifdef TARG_MIPS
              if (is_target_arch_MIPS()) {
			    if (ffast_math_prescan == 1) {  // Bug 14245
			      // Link with libscm and open64 libmpath before libm
			      add_library(lib_objects, "scm");
			      add_library(lib_objects, "m" PSC_NAME_PREFIX);
			    }
              } else {
#endif // TARG_MIPS
			    add_library(lib_objects, "m" PSC_NAME_PREFIX);
#ifdef TARG_MIPS
              }
#endif // TARG_MIPS
			} else {
			  add_library(objects, "mv");
#ifdef TARG_MIPS
              if (is_target_arch_MIPS()) {
			    if (ffast_math_prescan == 1) {  // Bug 14245
			      // Link with libscm and open64 libmpath before libm
			      add_library(objects, "scm");
			      add_library(objects, "m" PSC_NAME_PREFIX);
			    }
              } else {
#endif // TARG_MIPS
			    add_library(objects, "m" PSC_NAME_PREFIX);
#ifdef TARG_MIPS
              }
#endif // TARG_MIPS
			}
			if (invoked_lang == L_CC) {
			  add_library(cxx_prelinker_objects, "mv");
#ifdef TARG_MIPS
              if (is_target_arch_MIPS()) {
			    if (ffast_math_prescan == 1) {  // Bug 14245
			      // Link with libscm and open64 libmpath before libm
			      add_library(cxx_prelinker_objects, "scm");
			      add_library(cxx_prelinker_objects,
			      	"m" PSC_NAME_PREFIX);
			    }
              } else {
#endif // TARG_MIPS
			    add_library(cxx_prelinker_objects,
				        "m" PSC_NAME_PREFIX);
#ifdef TARG_MIPS
              }
#endif // TARG_MIPS
			}
			extern boolean link_with_mathlib;
			// Bug 4680 - It is too early to check target_cpu so we
			// set a flag here to note that -lm was seen and later 
			// use this to add -lacml_mv in add_final_ld_args.
			link_with_mathlib = 1;
		}

		/* xpg fort77 has weird rule about putting all libs after objects */
		if (xpg_flag && invoked_lang == L_f77) {
			add_library(lib_objects, arg);
		} else {
			add_library(objects, arg);
		}
		if (invoked_lang == L_CC) {
		    add_library(cxx_prelinker_objects, arg);
		}
		break;
	case O_object:
	       if (dashdash_flag && arg[0] == '-') {
		 add_string(objects,"--");
		 dashdash_flag = 1;
	       }
	       if (strncmp(arg, "-l", 2) == 0)
		   add_object(O_l, arg + 2);
	       else
		   add_string(objects, arg);
	       if (invoked_lang == L_CC) {
		   add_string(cxx_prelinker_objects, arg);
	       }

	       break;
	case O_WlC:
	       add_string(objects, concat_strings("-Wl,", arg));
	       break;
	case O__whole_archive:
	       add_string(objects, "-Wl,-whole-archive");
	       break;
	case O__no_whole_archive:
	       add_string(objects, "-Wl,-no-whole-archive");
	       break;
	default:
		internal_error("add_object called with not-an-object");
	}
}

/* append object files to the ar_objects list. */
void
add_ar_objects (char *arg)
{
    add_string(ar_objects, arg);
}

/* append objects to end of list */
void
append_objects_to_list (string_list_t *list)
{
	// If without -ipa, don't accept IPA-created objects.
	if (ipa != TRUE) {
	  int has_ipa_obj = FALSE;
	  string_item_t *p;
	  for (p = objects->head; p != NULL; p = p->next) {
	    char *filename = p->name;
	    if (check_for_whirl(filename) == TRUE) {
	      error("IPA-created object %s not allowed without -ipa", filename);
	      has_ipa_obj = TRUE;
	    }
	  }
	  if (has_ipa_obj == TRUE)
	    do_exit(1);
	}

        /* Do not allow user to mess with crt*.o objects. Check and throw out redundant files. */
	string_item_t *p;
	for (p = objects->head; p != NULL; p = p->next) {
	  char *filename = p->name;
         char *dup_filename = strdup(filename);
	  char *base_name = basename(dup_filename); //basename could modify its argument 

	  if (strncmp(base_name, "crt", 3) == 0) {	
            /* cut out superfluous crt*.o objects if specified by libtool */
            add_string_if_new_basename(list, filename);
	  } else {
            add_string(list, filename);
          }
          free(dup_filename);
	}

	if (xpg_flag && invoked_lang == L_f77) {
		append_string_lists (list, lib_objects);
	}
}

/* append cxx_prelinker_objects to end of list */
void
append_cxx_prelinker_objects_to_list (string_list_t *list)
{
	append_string_lists (list, cxx_prelinker_objects);
}

void
append_ar_objects_to_list(string_list_t *list)
{
    append_string_lists (list, ar_objects);
}

void
append_libraries_to_list (string_list_t *list)
{
        string_item_t *p;
        for (p = library_dirs->head; p != NULL; p = p->next) {
		add_string(list, concat_strings("-L", p->name));
        }
}

void
dump_objects (void)
{
	printf("objects:  ");
	print_string_list (stdout, objects);
}

void
add_library_dir (const char *path)
{
	add_string_if_new(library_dirs, path);
}

string_list_t *
get_library_dirs(void)
{
	return library_dirs;
}

void
add_library_options (void)
{
	int flag;
	buffer_t mbuf;
	buffer_t rbuf;
	char *suffix = NULL;
	char *mips_lib = NULL;
	char *proc_lib = NULL;
	char *lib = NULL;
	/*
	 * 32-bit libraries go in /usr/lib32. 
	 * 64-bit libraries go in /usr/lib64.
	 * isa-specific libraries append /mips{2,3,4}.
	 * non_shared libraries append /nonshared.
	 */

#ifdef TARG_MIPS
    if (is_target_arch_MIPS()) {
        switch (abi) {
        case ABI_N32:
        case ABI_I32:
            append_phase_dir(P_library, "32");
            append_phase_dir(P_startup, "32");
            break;
        case ABI_64:
            append_phase_dir(P_library, "64");
            append_phase_dir(P_startup, "64");
            break;
        case ABI_I64:
        case ABI_IA32:
            break;
        default:
            internal_error("no abi set? (%d)", abi);
        }
    } else {
#endif // TARG_MIPS
        switch (abi) {
        case ABI_M32:
        case ABI_M64:
        case ABI_I64:
        case ABI_IA32:
            break;
        default:
            internal_error("no abi set? (%d)", abi);
        }
#ifdef TARG_MIPS
    }
#endif // TARG_MIPS
}

/* search library_dirs for the crt file */
char *
find_crt_path (char *crtname)
{
        string_item_t *p;
	buffer_t buf;
        for (p = library_dirs->head; p != NULL; p = p->next) {
		sprintf(buf, "%s/%s", p->name, crtname);
		if (file_exists(buf)) {
			return string_copy(buf);
		}
        }
	/* not found */
	if (option_was_seen(O_nostdlib) || option_was_seen(O_L)) {
		error("crt files not found in any -L directories:");
        	for (p = library_dirs->head; p != NULL; p = p->next) {
			fprintf(stderr, "\t%s/%s\n", p->name, crtname);
		}
		return crtname;
	} else {
		/* use default */
		sprintf(buf, "%s/%s", get_phase_dir(P_startup), crtname);
		return string_copy(buf);
	}
}

// Check whether the option should be turned into a linker option when pathcc
// is called as a linker.
boolean
is_maybe_linker_option (int flag)
{
  // Example:
  //  case O_static:
  //    return TRUE;
  //    break;

  switch (flag) {
    default:
      break;
  }
  return FALSE;
}

// Add the linker version of the option.
void
add_maybe_linker_option (int flag)
{
  // Add ',' in front of the option name to indicate that the option is active
  // only if pathcc is called as a linker.  For example:
  //  case O_static:
  //    add_string(objects, ",-Wl,-static");
  //    break;

  switch (flag) {
    default:
      break;
  }
}

// If is_linker is TRUE, then turn the potential linker options into real
// linker options; otherwise delete them.
void
finalize_maybe_linker_options (boolean is_linker)
{
  string_item_t *p;

  if (is_linker) {
    // Potential linker options begin with ','.
    for (p = objects->head; p != NULL; p = p->next) {
      if (p->name[0] == ',') {
	// Remove the ',' in front.
        char *new_str = string_copy(&(p->name[1]));
	p->name = new_str;
      }
    }
  } else {
    string_item_t *prev = NULL;
    int deleted = FALSE;
    for (p = objects->head; p != NULL; p = p->next) {
      // Potential linker options begin with ','.
      if (p->name[0] == ',') {
	// Put back the non-linker version of the option if necessary.
	char *str = p->name;

	// Currently there is nothing, but if there is, follow this example:
	//   if (!strcmp (str, ",-Wl,-static")) {
	//     add_option_seen (O_static);
	//   }

	// Delete the option.
	if (prev == NULL) {
	  objects->head = p->next;
	} else {
	  prev->next = p->next;
	}
	deleted = TRUE;
      } else {
	prev = p;
      }
    }

    // Update the tail.
    if (deleted) {
      string_item_t *tail = NULL;
      for (p = objects->head; p != NULL; p = p->next) {
	tail = p;
      }
      objects->tail = tail;
    }
  }
}

#if defined(BUILD_OS_DARWIN)
/* Need to fix this eventually */
static int check_for_whirl(char *name) { return 0; }
#else /* defined(BUILD_OS_DARWIN) */
// Check for ELF files containing WHIRL objects.  Code taken from
// ../cygnus/bfd/ipa_cmdline.c.

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

#ifdef X86_WHIRL_OBJECTS
static int look_for_elf32_section (const Elf32_Ehdr* ehdr, Elf32_Word type, Elf32_Word info)
{
    int i;

    Elf32_Shdr* shdr = (Elf32_Shdr*)((char*)ehdr + ehdr->e_shoff);
    for (i = 1; i < ehdr->e_shnum; ++i) {
        if (shdr[i].sh_type == type && shdr[i].sh_info == info)
            return 1;
    }

    return 0;
}

static int look_for_elf64_section (const Elf64_Ehdr* ehdr, Elf64_Word type, Elf64_Word info)
{
    int i;

    Elf64_Shdr* shdr = (Elf64_Shdr*)((char*)ehdr + ehdr->e_shoff);
    for (i = 1; i < ehdr->e_shnum; ++i) {
        if (shdr[i].sh_type == type && shdr[i].sh_info == info)
            return 1;
    }

    return 0;
}
#endif

// Check to see if this is an ELF file and then if it is a WHIRL object.
#define ET_SGI_IR   (ET_LOPROC + 0)
static int
check_for_whirl(char *name)
{
    int fd = -1;
    char *raw_bits = NULL;
    int size,bufsize;
    Elf32_Ehdr *p_ehdr = NULL;
    struct stat statb;
    int test;
    
    fd = open(name, O_RDONLY, 0755);
    if (fd < 0)
	return FALSE;

    if ((test = fstat(fd, &statb) != 0)) {
    	close(fd);
	return FALSE;
    }

    if (statb.st_size < sizeof(Elf64_Ehdr)) {
    	close(fd);
    	return FALSE;
    }
    
    bufsize = sizeof(Elf64_Ehdr);
    
    raw_bits = (char *)alloca(bufsize*4);

    size = read(fd, raw_bits, bufsize);
    
		/*
		 * Check that the file is an elf executable.
		 */
    p_ehdr = (Elf32_Ehdr *)raw_bits;
    if (p_ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
	p_ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
	p_ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
	p_ehdr->e_ident[EI_MAG3] != ELFMAG3) {
	    close(fd);
	    return(FALSE);
    }

    if(p_ehdr->e_ident[EI_CLASS] == ELFCLASS32){
    	Elf32_Ehdr *p32_ehdr = (Elf32_Ehdr *)raw_bits;
#ifdef X86_WHIRL_OBJECTS
        char *second_buf = NULL; 

        int new_size = p32_ehdr->e_shoff + sizeof(Elf32_Shdr)*p32_ehdr->e_shnum;
        lseek(fd, 0, SEEK_SET);
        second_buf = (char *)alloca(new_size);
        size = read(fd, second_buf, new_size);
        p32_ehdr = (Elf32_Ehdr *)second_buf;
        if (p32_ehdr->e_type == ET_REL && look_for_elf32_section(p32_ehdr, SHT_PROGBITS, WT_PU_SECTION)) {
#else
        if (p32_ehdr->e_type == ET_SGI_IR) {
#endif
	    close(fd);
	    return TRUE;
	}
    }
    else {
	Elf64_Ehdr *p64_ehdr = (Elf64_Ehdr *)raw_bits;
#ifdef X86_WHIRL_OBJECTS
        char *second_buf = NULL; 

        int new_size = p64_ehdr->e_shoff + sizeof(Elf64_Shdr)*p64_ehdr->e_shnum;
        lseek(fd, 0, SEEK_SET);
        second_buf = (char *)alloca(new_size);
        size = read(fd, second_buf, new_size);
        p64_ehdr = (Elf64_Ehdr *)second_buf;
        if (p64_ehdr->e_type == ET_REL && look_for_elf64_section(p64_ehdr, SHT_PROGBITS, WT_PU_SECTION)) {
#else
        if (p64_ehdr->e_type == ET_SGI_IR) {
#endif
	    close(fd);
	    return TRUE;
	}
     }

    close(fd);
    return FALSE;
    
}
#endif /* defined(BUILD_OS_DARWIN) */
