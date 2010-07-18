/*
 * Copyright (C) 2006, 2007. QLogic Corporation. All Rights Reserved.
 */

/* 
   Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
   File modified June 20, 2003 by PathScale, Inc. to update Open64 C/C++ 
   front-ends to GNU 3.2.2 release.
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


/* translate gnu decl trees to symtab references */

#ifndef spin_symtab_INCLUDED
#define spin_symtab_INCLUDED
#ifdef TARG_ST
extern gs_t  lookup_attribute(const char *, gs_t);
#endif
extern TY_IDX Create_TY_For_Tree (gs_t, TY_IDX idx = TY_IDX_ZERO);
extern "C" ST* Create_ST_For_Tree (gs_t);
extern "C" void Create_DST_For_Tree (gs_t, ST*);
#ifdef TARG_ST
// (cbr) 
extern void set_variable_attributes(gs_t, ST*);
#endif

TY_IDX& TYPE_TY_IDX(gs_t);
extern "C" void add_duplicates (gs_t, gs_t);
extern "C" void erase_duplicates (gs_t);
void set_DECL_ST(gs_t, ST*);
ST*& get_DECL_ST(gs_t);
BOOL& expanded_decl(gs_t);
BOOL& func_PU_uplevel(gs_t);
#define DECL_ST(x)	get_DECL_ST(x)
SYMTAB_IDX& DECL_SYMTAB_IDX(gs_t);
LABEL_IDX& DECL_LABEL_IDX(gs_t);
ST*& TREE_STRING_ST(gs_t);
BOOL& DECL_LABEL_DEFINED(gs_t);
INT32& DECL_FIELD_ID(gs_t);
#ifdef TARG_ST
void SET_TYPE_FIELD_IDS_USED(gs_t,INT32);
INT32 GET_TYPE_FIELD_IDS_USED(gs_t);
BOOL TYPE_FIELD_IDS_USED_KNOWN(gs_t);
#else
INT32& TYPE_FIELD_IDS_USED(gs_t);
#endif
INT32& SCOPE_NUMBER(gs_t);
gs_t& LABEL_SCOPE(gs_t);
DST_INFO_IDX& DECL_DST_IDX(gs_t);
DST_INFO_IDX& DECL_DST_FIELD_IDX(gs_t);
DST_INFO_IDX& TYPE_DST_IDX(gs_t);
DST_INFO_IDX& DECL_DST_SPECIFICATION_IDX(gs_t);
DST_INFO_IDX& DECL_DST_ABSTRACT_ROOT_IDX(gs_t);
LABEL_IDX& HANDLER_LABEL(gs_t);
#ifdef KEY
gs_t& PARENT_SCOPE(gs_t);
INT32& WEAK_WORKAROUND(ST*);
#endif
ST* & DECL_ST2(gs_t);

extern BOOL flag_no_common;
#ifdef TARG_ST
extern BOOL flag_short_double;
#endif

#ifdef KEY
// States for tracking whether we want to make a symbol weak as a workaround
// for emitting all symbols referenced in cleanup code.
#define WEAK_WORKAROUND_unknown		0
#define WEAK_WORKAROUND_dont_make_weak	1
#define WEAK_WORKAROUND_made_weak	2

extern BOOL expanding_function_definition;
#endif

/* 
 * either return a previously created TY_IDX associated with a type,
 * or create a new one.
 */
inline TY_IDX
Get_TY (gs_t type_tree)
{
#ifdef KEY
	// If the type is a nested record and we are generating DSTs, then
	// create the type for the enclosing record first, so that the current
	// record can be specified as a child of the parent in the DST.
	// Bug 4168.
	if (gs_type_context(type_tree) &&
	    (gs_tree_code(type_tree) == GS_RECORD_TYPE ||
	     gs_tree_code(type_tree) == GS_UNION_TYPE) &&
	    (gs_tree_code(gs_type_context(type_tree)) == GS_RECORD_TYPE ||
	     gs_tree_code(gs_type_context(type_tree)) == GS_UNION_TYPE) &&
	    !TYPE_TY_IDX(gs_type_context(type_tree))) {
	  Get_TY(gs_type_context(type_tree));	// get type for parent record
	}
#endif
	TY_IDX idx = TYPE_TY_IDX(type_tree);
        if (idx > 1) {
	    if (gs_tree_code(type_tree) == GS_RECORD_TYPE ||
	        gs_tree_code(type_tree) == GS_UNION_TYPE) {
	      FLD_HANDLE elt_fld = TY_fld(idx);
	      if (elt_fld.Is_Null() 
                  && TY_content_seen(idx) == 0
                  )
		return Create_TY_For_Tree (type_tree, idx); // forward declared
	      else return idx;
	    }
#if defined( KEY) /* bug 8346 */
	    else if (expanding_function_definition &&
	             (TY_is_incomplete(idx) ||
		      (TY_kind(idx)==KIND_POINTER &&
		       TY_is_incomplete(TY_pointed(idx)))))
	      return Create_TY_For_Tree (type_tree, idx);
#endif
	    else return idx;
        }
	return Create_TY_For_Tree (type_tree, TY_IDX_ZERO);
}
#ifdef TARG_ST
// bug fix for OSP_133
// return section name for corresponding ST via st_attr table
struct find_st_attr_section_name {
        ST_IDX st;
        find_st_attr_section_name (const ST *s) : st (ST_st_idx (s)) {}
        BOOL operator () (UINT, const ST_ATTR *st_attr) const {
                return (ST_ATTR_kind (*st_attr) == ST_ATTR_SECTION_NAME &&
                                ST_ATTR_st_idx (*st_attr) == st);
        }
};
#endif

/*
 * either return a previously created ST associated with a
 * var-decl/parm-decl/function_decl, or create a new one.
 */
inline ST *
Get_ST (gs_t decl_tree)
{
	ST *st = DECL_ST(decl_tree);
        if (st != NULL) {
		if (gs_tree_code(decl_tree) == GS_VAR_DECL &&
		    ST_sclass(st) == SCLASS_EXTERN   &&
		    !ST_is_weak_symbol(st) &&
#ifdef TARG_ST
		    // [CL] handle aliases: keep SCLASS_EXTERN
		    !Has_Base_Block(st) &&
		    !ST_emit_symbol(st) &&
#endif

		    !gs_decl_external(decl_tree)        &&
#ifdef TARG_ST
                    gs_tree_static(decl_tree)
#else
		    !gs_decl_initial(decl_tree)
#if 1 // wgen, bug 10324
		     &&
		    gs_tree_static(decl_tree)
#endif
#endif
		    )
		{
#ifdef TARG_ST
		    if (flag_no_common || gs_decl_section_name(decl_tree)
			|| ! gs_decl_common(decl_tree))
#else
		    if (flag_no_common || gs_decl_section_name(decl_tree))
#endif
			Set_ST_sclass (st, SCLASS_UGLOBAL);
		    else {
		      if (Debug_Level >= 2) { 
			  // Bug 6183 
			  // If this symbol was declared extern before and 
			  // later instantiated in this file, then we need to 
			  // create a DST entry now.
		          Create_DST_For_Tree(decl_tree, st);
			}
		      Set_ST_sclass (st, SCLASS_COMMON);
		    }
		}
#ifdef KEY // the earlier definition may not have the complete type
		if (gs_tree_code(decl_tree) == GS_VAR_DECL
#ifdef TARG_ST
                      && ( ST_sclass(st) == SCLASS_EXTERN ||
                                ST_sclass(st) == SCLASS_FSTATIC ||
                                ST_sclass(st) == SCLASS_COMMON ||
                                ST_sclass(st) == SCLASS_UGLOBAL ||
                                ST_sclass(st) == SCLASS_DGLOBAL) &&
                                !ST_is_weak_symbol(st) &&
                                !gs_decl_external(decl_tree) &&
                                gs_tree_static(decl_tree) &&
                                gs_decl_section_name(decl_tree)
#endif

                    ) {
#ifdef TARG_ST
                    ST_ATTR_IDX st_attr_idx;
                        ST_IDX idx = ST_st_idx (st);
                        // search for section name attribute in st_attr table
                        st_attr_idx = For_all_until (St_Attr_Table,
                                        ST_IDX_level (idx),
                                        find_st_attr_section_name(st));
                        // search for section name attribute in st_attr table
                        st_attr_idx = For_all_until (St_Attr_Table,ST_IDX_level (idx),
                                        find_st_attr_section_name(st));
                        if (st_attr_idx){
                                STR_IDX str_index = ST_ATTR_section_name(St_Attr_Table(ST_IDX_level (idx), st_attr_idx));
                                // only when they have different section names
                                if(strcmp(Index_To_Str(str_index),
                                                        gs_tree_string_pointer(gs_decl_section_name (decl_tree))))
                                         Set_ST_ATTR_section_name(St_Attr_Table(ST_IDX_level (idx), st_attr_idx),
                                         Save_Str (gs_tree_string_pointer(gs_decl_section_name (decl_tree))));}
                                // bug fix for OSP_136
                                /* if the old tree node doesn't have section name
                                 * attribute, create one for current tree node */
                        else {
                                DevWarn ("section %s specified for %s",
                                                gs_tree_string_pointer(gs_decl_section_name (decl_tree)), ST_name (st));
                                ST_ATTR&    st_attr = New_ST_ATTR (ST_IDX_level (idx), st_attr_idx);
                                ST_ATTR_Init (st_attr, idx, ST_ATTR_SECTION_NAME,
                                                Save_Str (gs_tree_string_pointer(gs_decl_section_name (decl_tree))));
                        }

			if (!gs_decl_initial(decl_tree))
			    Set_ST_sclass (st, SCLASS_UGLOBAL);
#else
		  TY_IDX ty_idx = Get_TY(gs_tree_type(decl_tree));
		  if (ty_idx && TY_IDX_index(ty_idx) != TY_IDX_index(st->u2.type)) {
		    // Preserve volatile.
		    if (TY_is_volatile(ST_type(st)))
		      Set_TY_is_volatile(ty_idx);
		    st->u2.type = ty_idx;
		  }
#endif
		}
#endif
        }
	else st = Create_ST_For_Tree (decl_tree);
#ifdef TARG_ST
	if (st) set_variable_attributes(decl_tree, st);
#endif
	if ((CURRENT_SYMTAB > GLOBAL_SYMTAB + 1) &&
	    ((gs_tree_code(decl_tree) == GS_VAR_DECL) ||
	     (gs_tree_code(decl_tree) == GS_PARM_DECL)) &&
	    (ST_level(st) < CURRENT_SYMTAB) &&
	    (ST_level(st) > GLOBAL_SYMTAB)) {
		Set_ST_has_nested_ref (st);
		ST *base_st = st;
		while (base_st != ST_base (base_st)) {
			base_st = ST_base (base_st);
			Set_ST_has_nested_ref (base_st);
		}
	}
#ifdef KEY
	if (ST_is_thread_private(st) && CURRENT_SYMTAB != GLOBAL_SYMTAB)
	  Set_PU_has_mp(Get_Current_PU());
#endif
	return st;
}
#ifdef TARG_ST
/*
 * Get_Export_Class_For_Tree (gs_t decl_node, ST_CLASS storage, ST_SCLASS sclass)
 * 
 * Returns the export class for a tree accounting for
 * visibility attribute, visibility specification file and
 * default visibility setting.
 * This function should be called each time a new symbol is
 * created for setting its export class.
 */
extern ST_EXPORT Get_Export_Class_For_Tree (gs_t decl_node, ST_CLASS storage, ST_SCLASS sclass);
#endif

#ifndef TARG_ST
bool is_empty_base_class (gs_t type_tree);
#endif
gs_t next_real_field (gs_t type_tree, gs_t field);
gs_t get_first_real_or_virtual_field (gs_t);
size_t Roundup (size_t offset, int alignment);
size_t Type_Size_Without_Vbases (gs_t type_tree);

#endif
