# for now, install in subdirectory of source directory by default
SET( CMAKE_INSTALL_PREFIX .. )

IF( WIN32 )
	ADD_LIBRARY( argtable2 arg_dbl.c arg_end.c arg_file.c arg_int.c arg_lit.c arg_rem.c arg_str.c argtable2.c argtable2.h getopt.c getopt.h getopt1.c )
ELSE( WIN32 )
	ADD_LIBRARY( argtable2 arg_date.c arg_dbl.c arg_end.c arg_file.c arg_int.c arg_lit.c arg_rem.c arg_rex.c arg_str.c argtable2.c argtable2.h getopt.c getopt.h getopt1.c )
ENDIF( WIN32 )

INSTALL( TARGETS argtable2 ARCHIVE DESTINATION lib)
SET_TARGET_PROPERTIES( argtable2 PROPERTIES DEBUG_POSTFIX d )