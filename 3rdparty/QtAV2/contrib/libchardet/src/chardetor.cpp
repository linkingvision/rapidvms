#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "chardet.h"
#define CHARSET_MAX 1024

int main(int argc, char **argv)
{
    int i = 0, fd = 0;
    struct stat st;
    char *s = NULL, charset[CHARSET_MAX];
    chardet_t pdet = NULL;

    if(argc < 2)
    {
        fprintf(stderr, "Usage:%s file ...\n", argv[0]);
        _exit(-1);
    }

    if(chardet_create(&pdet) == 0)
    {
        for(i = 1; i < argc; i++)
        {
            if((fd = open(argv[i], O_RDONLY, 0644)) > 0)
            {
                if(fstat(fd, &st) == 0 && st.st_size > 0 
                        && (s = (char *)calloc(1, st.st_size + 1)))
                {
                    if(read(fd, s, st.st_size) > 0 
                        && chardet_handle_data(pdet, s, st.st_size) == 0)
                    {
                        chardet_data_end(pdet);
                        chardet_get_charset(pdet, charset, CHARSET_MAX);
                        fprintf(stdout, "charset of %s:%s\n", argv[i], charset);
                        chardet_reset(pdet);
                    }
                    free(s);
                    s = NULL;
                }
                close(fd);
            }
        }
        chardet_destroy(pdet);
    }
}
