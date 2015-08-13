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

int main(int argc, char **argv)
    {
    struct arg_date *a   = arg_date1(NULL, NULL, "%H:%M",    NULL,      "time 23:59");
    struct arg_date *b   = arg_date0("b",  NULL, "%Y-%m-%d", NULL,      "date YYYY-MM-DD");
    struct arg_date *c   = arg_daten(NULL,"date","%D",       NULL, 1,2, "MM/DD/YYYY");
    struct arg_lit *help = arg_lit0(NULL,"help","print this help and exit");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,help,end};
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
        printf("Usage: %s", argv[0]);
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
        {
        char buff[200];
        strftime(buff, sizeof(buff), "%x", a->tmval+i);
        printf("a->tmval[%d] = \"%s\"\n", i, buff );
        }
    for (i=0; i<b->count; i++)
        {
        char buff[200];
        strftime(buff, sizeof(buff), "%x", b->tmval+i);
        printf("b->tmval[%d] = \"%s\"\n", i, buff );
        }
    for (i=0; i<c->count; i++)
        {
        char buff[200];
        strftime(buff, sizeof(buff), "%x", c->tmval+i);
        printf("c->tmval[%d] = \"%s\"\n", i, buff );
        }

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0])); 
    
    printf("exitcode=%d\n\n",exitcode);
    return exitcode;
    }

