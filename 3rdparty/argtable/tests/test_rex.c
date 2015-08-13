/*********************************************************************
This file is part of the argtable2 library.
Copyright (C) 1998-2001,2003-2011 Stewart Heitmann
sheitmann@users.sourceforge.net

The argtable2 library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

#include "../src/argtable2.h"
#include <stdlib.h>
#include <regex.h>


int main(int argc, char **argv)
    {
    struct arg_rex *a    = arg_rex0("a",  NULL,   "hello",    NULL ,              0, "blah blah");
    struct arg_rex *b    = arg_rex1(NULL, "beta", "[Ww]orld", NULL ,              0, "blah blah");
    struct arg_rex *c    = arg_rexn(NULL, NULL,   "goodbye",  NULL , 1,5, REG_ICASE, "blah blah");
    struct arg_rex *d    = arg_rex0(NULL, NULL,   "any.*",    NULL ,      REG_ICASE, "blah blah");
    struct arg_lit *help = arg_lit0(NULL,"help","print this help and exit");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,d,help,end};
    int nerrors;
    int exitcode=0;
    int i;
    
    /* print the command line */
    for (i=0; i<argc; i++)
        printf("%s ",argv[i]);
    printf("\n");
    
    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
        {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",argv[0]);
        exitcode=1;
        goto exit;
        }
    
    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
        {
        printf("Usage: %s ", argv[0]);
        arg_print_syntax(stdout,argtable,"\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        exitcode=0;
        goto exit;
        }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
        {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,"syntax error");
        exitcode=1;
        goto exit;
        }

    for (i=0; i<a->count; i++)
        printf("a->sval[%d] = \"%s\"\n", i, a->sval[i] );
    for (i=0; i<b->count; i++)
        printf("b->sval[%d] = \"%s\"\n", i, b->sval[i] );
    for (i=0; i<c->count; i++)
        printf("c->sval[%d] = \"%s\"\n", i, c->sval[i] );
    for (i=0; i<d->count; i++)
        printf("d->sval[%d] = \"%s\"\n", i, d->sval[i] );
        
    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0])); 
    
    printf("exitcode=%d\n\n",exitcode);
    return exitcode;
    }

