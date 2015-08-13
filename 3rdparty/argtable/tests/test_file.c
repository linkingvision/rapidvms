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

/* for memory leak debugging */
#ifdef DMALLOC
#include "dmalloc.h"
#endif


int main(int argc, char **argv)
    {
    struct arg_file *a   = arg_file1(NULL,NULL,"<file>","filename to test");
    struct arg_str *b    = arg_str1(NULL,NULL,"<filename>","expected filename to be parsed from <file>");
    struct arg_str *c    = arg_str1(NULL,NULL,"<basename>","expected basename to be parsed from <file>");
    struct arg_str *d    = arg_str1(NULL,NULL,"<extension>","expected extension to be parsed from <file>");
    struct arg_lit *help = arg_lit0(NULL,"help","print this help and exit");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,d,help,end};
    int nerrors;
    int exitcode=0;
    int i;
    long sum=0;
    
    /*
    printf("a=%p\n",a);
    printf("b=%p\n",b);
    printf("c=%p\n",c);
    printf("d=%p\n",d);
    printf("help=%p\n",help);
    printf("end=%p\n",end);
    printf("argtable=%p\n",argtable);
    */
    
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
        arg_print_errors(stdout,end,argv[0]);
        exitcode=1;
        goto exit;
        }

    /* parsing complete, verify strings b,c,d match the parsing from a */
    printf("a[0].filename=\"%s\"\n",  a->filename[0] );
    printf("a[0].basename=\"%s\"\n",  a->basename[0] );
    printf("a[0].extension=\"%s\"\n", a->extension[0]);
    printf("b[0].sval=\"%s\"\n",      b->sval[0] );
    printf("c[0].sval=\"%s\"\n",      c->sval[0] );
    printf("d[0].sval=\"%s\"\n",      d->sval[0] );

    if (strcmp(a->filename[0],b->sval[0])!=0)
        {
        printf("%s: error - a[0].filename did not match b[0].sval\n", argv[0]);
        exitcode=1;
        goto exit;
        }

    if (strcmp(a->basename[0],c->sval[0])!=0)
        {
        printf("%s: error - a[0].basename did not match c[0].sval\n", argv[0]);
        exitcode=1;
        goto exit;
        }

    if (strcmp(a->extension[0],d->sval[0])!=0)
        {
        printf("%s: error - a[0].filename did not match d[0].sval\n", argv[0]);
        exitcode=1;
        goto exit;
        }
        
    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));    
    
    printf("%s: exitcode=%d\n\n",argv[0],exitcode);

    /* close stdin and stdout to stop memcheck whining about their memory not being freed */
    fclose(stdin);
    fclose(stdout);
   
    return exitcode;
    }

