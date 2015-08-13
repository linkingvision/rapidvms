#include "../src/argtable2.h"
#include <assert.h>

/* for memory leak debugging */
#ifdef DMALLOC
#include "dmalloc.h"
#endif


 /*
Here we verify that arg_freetable() works proprely, at least by nulling the argtable
array entries when finished.
*/
void test_freetable(void)
    {
    struct arg_lit  *list    = arg_lit0("lL",NULL,                      "list files");
    struct arg_lit  *recurse = arg_lit0("R",NULL,                       "recurse through subdirectories");
    struct arg_int  *repeat  = arg_int0("k","scalar",NULL,              "define scalar value k (default is 3)");
    struct arg_str  *defines = arg_strn("D","define","MACRO",0,20,      "macro definitions");
    struct arg_file *outfile = arg_file0("o",NULL,"<output>",           "output file (default is \"-\")");
    struct arg_lit  *verbose = arg_lit0("v","verbose,debug",            "verbose messages");
    struct arg_lit  *help    = arg_lit0(NULL,"help",                    "print this help and exit");
    struct arg_lit  *version = arg_lit0(NULL,"version",                 "print version information and exit");
    struct arg_file *infiles = arg_filen(NULL,NULL,NULL,1,20,           "input file(s)");
    struct arg_end  *end     = arg_end(20);
    void* argtable[]  = {list,recurse,repeat,defines,outfile,verbose,help,version,infiles,end};

    printf("testing arg_freetable()...\n");
    
    assert( argtable[0]!=NULL );
    assert( argtable[1]!=NULL );
    assert( argtable[2]!=NULL );
    assert( argtable[3]!=NULL );
    assert( argtable[4]!=NULL );
    assert( argtable[5]!=NULL );
    assert( argtable[6]!=NULL );
    assert( argtable[7]!=NULL );
    assert( argtable[8]!=NULL );
    assert( argtable[9]!=NULL );
    
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
    
    assert( argtable[0]==NULL );
    assert( argtable[1]==NULL );
    assert( argtable[2]==NULL );
    assert( argtable[3]==NULL );
    assert( argtable[4]==NULL );
    assert( argtable[5]==NULL );
    assert( argtable[6]==NULL );
    assert( argtable[7]==NULL );
    assert( argtable[8]==NULL );
    assert( argtable[9]==NULL );
    
    printf("arg_freetable() OK\n");
    }

       
/*
Here we verify that arg_nullcheck() works properly at detecting NULL
entries in the argtable array.
In this test, we first build a normal argtable (taken from myprog.c)
and then a bunch of copycat argtable arrays that have NULL entries
dispersed within them. We test arg_nullcheck on each of these
argtable arrays to verify iots behaviour.
*/
void test_nullcheck(void)
    {
    struct arg_lit  *list    = arg_lit0("lL",NULL,                      "list files");
    struct arg_lit  *recurse = arg_lit0("R",NULL,                       "recurse through subdirectories");
    struct arg_int  *repeat  = arg_int0("k","scalar",NULL,              "define scalar value k (default is 3)");
    struct arg_str  *defines = arg_strn("D","define","MACRO",0,20,      "macro definitions");
    struct arg_file *outfile = arg_file0("o",NULL,"<output>",           "output file (default is \"-\")");
    struct arg_lit  *verbose = arg_lit0("v","verbose,debug",            "verbose messages");
    struct arg_lit  *help    = arg_lit0(NULL,"help",                    "print this help and exit");
    struct arg_lit  *version = arg_lit0(NULL,"version",                 "print version information and exit");
    struct arg_file *infiles = arg_filen(NULL,NULL,NULL,1,20,           "input file(s)");
    struct arg_end  *end     = arg_end(20);
    void* argtable[]  = {list,recurse,repeat,defines,outfile,verbose,help,version,infiles,end};
    void* argtable1[] = {list,recurse,repeat,defines,outfile,verbose,help,version,infiles,NULL};
    void* argtable2[] = {list,recurse,repeat,defines,outfile,verbose,help,version,NULL,   NULL};
    void* argtable3[] = {list,recurse,repeat,defines,outfile,verbose,help,NULL,   infiles,end};
    void* argtable4[] = {NULL,recurse,NULL,  NULL,   outfile,NULL,   help,version,infiles,NULL};

    printf("testing arg_nullcheck()...\n");
    assert(arg_nullcheck(argtable)  == 0);
    assert(arg_nullcheck(argtable1) == 1);
    assert(arg_nullcheck(argtable2) == 1);
    assert(arg_nullcheck(argtable3) == 1);
    assert(arg_nullcheck(argtable4) == 1);
    printf("arg_nullcheck() OK\n");
    
    arg_free(argtable);
    }


void test_arg_print_glossary(void)
    {
    struct arg_lit  *list    = arg_lit0("lL",NULL,                      "list files");
    struct arg_lit  *recurse = arg_lit0("R",NULL,                       "recurse through subdirectories");
    struct arg_int  *repeat  = arg_int0("k","scalar",NULL,              "define scalar value k (default is 3)");
    struct arg_str  *defines = arg_strn("D","define","MACRO",0,20,      "macro definitions");
    struct arg_file *outfile = arg_file0("o",NULL,"<output>",           "output file (default is \"-\")");
    struct arg_lit  *verbose = arg_lit0("v","verbose,debug",            "verbose messages");
    struct arg_lit  *help    = arg_lit0(NULL,"help",                    "print this help and exit");
    struct arg_lit  *version = arg_lit0(NULL,"version",                 "print version information and exit");
    struct arg_file *infiles = arg_filen(NULL,NULL,NULL,1,20,           "input file(s)");
    struct arg_end  *end     = arg_end(20);
    void* argtable[]  = {list,recurse,repeat,defines,outfile,verbose,help,version,infiles,end};

    printf("exercising arg_print_glossary()...\n");
    arg_print_glossary(stdout,argtable,NULL);    

    printf("exercising arg_print_glossary_gnu()...\n");
    arg_print_glossary_gnu(stdout,argtable);    

    arg_free(argtable);
    }


void test_printsyntax(void)
    {  
    struct arg_lit *help1 = arg_lit1("h",    NULL, "Syntax help");
    struct arg_end *end1  = arg_end(20);
    void *argtable1[] = { help1, end1 };

    struct arg_lit *help2 = arg_lit1(NULL, "help", "Syntax help");
    struct arg_end *end2  = arg_end(20);
    void *argtable2[] = { help2, end2 };

    printf("testing arg_printsyntax()...\n");
    assert(arg_nullcheck(argtable1)  == 0);
    assert(arg_nullcheck(argtable2)  == 0);

    printf("foo");
    arg_print_syntax(stdout,argtable1,"\n");

    printf("foo");
    arg_print_syntax(stdout,argtable2,"\n");
    
    arg_free(argtable1);
    arg_free(argtable2);
    }


void test_argc_zero(void)
    {
    /* simulate an empty argv[] list as occurs on TI DSP */
    int argc = 0;
    char **argv = NULL;

    /* argument table */
    struct arg_int *a    = arg_int0(NULL,NULL,"a","a is <int>");
    struct arg_int *b    = arg_int0(NULL,NULL,"b","b is <int>");
    struct arg_int *c    = arg_int0(NULL,NULL,"c","c is <int>");
    struct arg_end *end  = arg_end(20);
    void* argtable[] = {a,b,c,end};

    int nerrors;

    printf("testing arg_parse() with argc==0...\n");
    
    /* verify the argtable[] entries were allocated sucessfully */
    assert(arg_nullcheck(argtable) == 0);

    printf("foo");
    arg_print_syntax(stdout,argtable,"\n");
    
    /* Try parsing the empty command line */
    nerrors = arg_parse(argc,argv,argtable);
    assert(nerrors==0);

    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));    
    }


void test_initial_strings(void)
    {
    struct arg_str  *defines = arg_strn("D","define","MACRO",0,3, "macro definitions");
    struct arg_file *outfile = arg_filen("o",NULL,"<output>",0,5, "output file (default is \"-\")");
    struct arg_end  *end     = arg_end(20);
    void* argtable[]  = {defines,outfile,end};
    int i;

    printf("testing initial strings...\n");
    
    /* arg_str: assert initial string values are "" */
    for (i=0; i<defines->hdr.maxcount; i++)
        {
        assert( strcmp(defines->sval[i],"") == 0 ); 
        printf("defines->sval[%d] == \"\" OK\n",i);
        }

    /* arg_file: assert initial string values are "" */
    for (i=0; i<outfile->hdr.maxcount; i++)
        {
        assert( strcmp(outfile->filename[i],"") == 0 );  
        printf("outfile->filename[%d]  == \"\" OK\n",i);
        assert( strcmp(outfile->basename[i],"") == 0 );  
        printf("outfile->basename[%d]  == \"\" OK\n",i);
        assert( strcmp(outfile->extension[i],"") == 0 );  
        printf("outfile->extension[%d] == \"\" OK\n",i);
        }

    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));    
    }


int main(int argc, char **argv)
    {
    test_freetable();
    test_nullcheck();
    test_arg_print_glossary();
    test_printsyntax();
    test_argc_zero();
    test_initial_strings();

    /* close stdin and stdout to stop memcheck whining about their memory not being freed */
    fclose(stdin);
    fclose(stdout);
    
    return 0;
    }
