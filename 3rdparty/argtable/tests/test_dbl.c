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
    struct arg_dbl *a    = arg_dbl1(NULL,NULL,"a","a is <double>");
    struct arg_dbl *b    = arg_dbl0(NULL,NULL,"b","b is <double>");
    struct arg_dbl *c    = arg_dbl0(NULL,NULL,"c","c is <double>");
    struct arg_dbl *d    = arg_dbln("dD","delta","<double>",0,3,"d can occur 0..3 times");
    struct arg_dbl *e    = arg_dbl0(NULL,"eps,eqn","<double>","eps is optional");
    struct arg_lit *help = arg_lit0(NULL,"help","print this help and exit");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,d,e,help,end};
    int nerrors;
    int exitcode=0;
    int i;
    double sum=0;
    
    /*
    printf("a=%p\n",a);
    printf("b=%p\n",b);
    printf("c=%p\n",c);
    printf("d=%p\n",d);
    printf("e=%p\n",e);
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

    /* parsing complete, verify all args sum to zero */
    for (i=0; i<a->count; i++)
        {
        printf("a[%d]=%f\n",i,a->dval[i]);
        sum += a->dval[i];
        }
    for (i=0; i<b->count; i++)
        {
        printf("b[%d]=%f\n",i,b->dval[i]);
        sum += b->dval[i];
        }
    for (i=0; i<c->count; i++)
        {
        printf("c[%d]=%f\n",i,c->dval[i]);
        sum += c->dval[i];
        }
    for (i=0; i<d->count; i++)
        {
        printf("d[%d]=%f\n",i,d->dval[i]);
        sum += d->dval[i];
        }
    for (i=0; i<e->count; i++)
        {
        printf("e[%d]=%f\n",i,e->dval[i]);
        sum += e->dval[i];
        }
    printf("sum=%f\n",sum);
    if (sum<-1.0e-6 || sum>1.0e-6)
        {
        printf("%s: error - sum=%f is non-zero\n",argv[0],sum);
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

