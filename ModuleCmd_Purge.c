/*****
 ** ** Module Header ******************************************************* **
 ** 									     **
 **   Modules Revision 3.0						     **
 **   Providing a flexible user environment				     **
 ** 									     **
 **   File:		ModuleCmd_Purge.c				     **
 **   First Edition:	1991/10/23					     **
 ** 									     **
 **   Authors:	John Furlan, jlf@behere.com				     **
 **		Jens Hamisch, jens@Strawberry.COM			     **
 ** 									     **
 **   Description:	Removes all loaded modulefiles from the environment. **
 ** 									     **
 **   Exports:		ModuleCmd_Purge					     **
 ** 									     **
 **   Notes:								     **
 ** 									     **
 ** ************************************************************************ **
 ****/

/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1991-1994 by John L. Furlan.                      	     **
 ** see LICENSE.GPL, which must be provided, for details		     **
 ** 									     ** 
 ** ************************************************************************ **/

static char Id[] = "@(#)$Id: ModuleCmd_Purge.c,v 1.11 2011/11/11 15:32:54 rkowen Exp $";
static void *UseId[] = { &UseId, Id };

/** ************************************************************************ **/
/** 				      HEADERS				     **/
/** ************************************************************************ **/

#include "modules_def.h"

/** ************************************************************************ **/
/** 				  LOCAL DATATYPES			     **/
/** ************************************************************************ **/

/** not applicable **/

/** ************************************************************************ **/
/** 				     CONSTANTS				     **/
/** ************************************************************************ **/

/** not applicable **/

/** ************************************************************************ **/
/**				      MACROS				     **/
/** ************************************************************************ **/

/** not applicable **/

/** ************************************************************************ **/
/** 				    LOCAL DATA				     **/
/** ************************************************************************ **/

static	char	module_name[] = __FILE__;

/** ************************************************************************ **/
/**				    PROTOTYPES				     **/
/** ************************************************************************ **/

/** not applicable **/


/*++++
 ** ** Function-Header ***************************************************** **
 ** 									     **
 **   Function:		ModuleCmd_Purge					     **
 ** 									     **
 **   Description:	Execution of the module-command 'purge'		     **
 ** 									     **
 **   First Edition:	1991/10/23					     **
 ** 									     **
 **   Parameters:	Tcl_Interp	*interp		Attached Tcl Interp. **
 **			int		 argc		Number of arguments  **
 **			char 		*argv[]		Argument list	     **
 ** 									     **
 **   Result:		int	TCL_ERROR	Failure			     **
 **				TCL_OK		Successful operation	     **
 ** 									     **
 **   Attached Globals:							     **
 ** 									     **
 ** ************************************************************************ **
 ++++*/

int	ModuleCmd_Purge(	Tcl_Interp	*interp,
                		int		 argc,
                		char		*argv[])
{
    char	*lmodules = NULL,
		*cur_module = NULL,
		*loaded_modules,
		*unload_argv[ MOD_BUFSIZE];
    int		 unload_argc = 0,
    		 status;
    char        *unload_argv_rev[ MOD_BUFSIZE];
    int          reverse;

    /**
     **  Get the list of currently loaded modules from the environment variable
     **  LOADEDMODULES
     **/
    if(!(loaded_modules = EMGetEnv( interp, "LOADEDMODULES"))) {
	    /* nothing to do */
	    return( TCL_OK);		/** ---- EXIT (Nothing to list) ---- **/
    }

    if(!(lmodules = stringer(NULL,0, loaded_modules, NULL))) 
	if( OK != ErrorLogger( ERR_STRING, LOC, NULL))
	    return( TCL_ERROR);		/** -------- EXIT (FAILURE) -------- **/

    /**
     **  Build a NULL terminated list of loaded modules
     **/
    for( cur_module = xstrtok( lmodules, ":");
         cur_module && unload_argc < MOD_BUFSIZE-1;
         cur_module = xstrtok( NULL, ":"))
        unload_argv[ unload_argc++] = cur_module;
    
    unload_argv[ unload_argc] = NULL;
    
    for( reverse=0; reverse<unload_argc; reverse++ ) {
        unload_argv_rev[unload_argc - (reverse + 1)] = unload_argv[reverse]; 
    }   

    unload_argv_rev[ unload_argc] = NULL;
 
    /**
     **  Unload 'em all
     **  We always say the load succeeded.  ModuleCmd_Load will
     **  output any necessary error messages.
     **/
/*  ModuleCmd_Load( interp, 0, unload_argc, unload_argv); */
    ModuleCmd_Load( interp, 0, unload_argc, unload_argv_rev);

    status = TCL_OK;

    /**
     **  Free, what has been allocated and pass the load's result to the caller
     **/
    null_free((void *) &lmodules);

    return( status);

} /** End of 'ModuleCmd_Purge' **/
