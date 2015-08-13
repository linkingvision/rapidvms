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
    struct arg_int *a    = arg_int1(NULL,NULL,"a","a is <int>");
    struct arg_int *b    = arg_int0(NULL,NULL,"b","b is <int>");
    struct arg_int *c    = arg_int0(NULL,NULL,"c","c is <int>");
    struct arg_int *d    = arg_intn("dD","delta","<int>",0,3,"d can occur 0..3 times");
    struct arg_int *e    = arg_int0(NULL,"eps,eqn","<int>","eps is optional");
    struct arg_int *f    = arg_intn("fF","filler","<int>",0,3,"f can occur 0..3 times");
    struct arg_lit *help = arg_lit0(NULL,"help","print this help and exit");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,d,e,f,help,end};
    int nerrors;
    int exitcode=0;
    int i;
    long sum=0;
    
    /*
    printf("a=%p\n",a);
    printf("b=%p\n",b);
    printf("c=%p\n",c);
    printf("d=%p\n",d);
    printf("e=%p\n",e);
    printf("f=%p\n",f);
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
    
    /* allow missing argument values for the f argument, and set defaults to -1 */
    f->hdr.flag |= ARG_HASOPTVALUE;
    for (i=0; i<f->hdr.maxcount; i++)
        f->ival[i] = -1;
   
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

    /* parsing complete, verify all args sum to zero */
    for (i=0; i<a->count; i++)
        {
        printf("a[%d]=%d\n",i,a->ival[i]);
        sum += a->ival[i];
        }
    for (i=0; i<b->count; i++)
        {
        printf("b[%d]=%d\n",i,b->ival[i]);
        sum += b->ival[i];
        }
    for (i=0; i<c->count; i++)
        {
        printf("c[%d]=%d\n",i,c->ival[i]);
        sum += c->ival[i];
        }
    for (i=0; i<d->count; i++)
        {
        printf("d[%d]=%d\n",i,d->ival[i]);
        sum += d->ival[i];
        }
    for (i=0; i<e->count; i++)
        {
        printf("e[%d]=%d\n",i,e->ival[i]);
        sum += e->ival[i];
        }
    for (i=0; i<f->count; i++)
        {
        printf("f[%d]=%d\n",i,f->ival[i]);
        sum += f->ival[i];
        }
    printf("sum=%ld\n",sum);
    if (sum!=0)
        {
        printf("%s: error - sum=%ld is non-zero\n",argv[0],sum);
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

