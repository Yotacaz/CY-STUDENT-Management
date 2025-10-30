#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void test_utils()
{
    printf("salut");
}

size_t bin_read_string(FILE *file, char buf[], long buf_size)
{
    assert(file);
    assert(buf && buf_size>0);
    size_t n_read = fread(buf, 1, buf_size - 1, file);
    if (n_read == 0)
    {
        fprintf(stderr, "ERROR : nothing read, EOF ?\n");
        return -1;
    }
    buf[n_read] = '\0'; // in case no '\0' is read
    char *end_ptr = memchr(buf, '\0', n_read);
    if (!end_ptr)
    {
        fprintf(stderr, "ERROR : string too long or corrupted\n");
        exit(EXIT_FAILURE);
    }
    size_t str_len = end_ptr - buf;
    long unread_bytes = (long)(n_read - (str_len + 1));
    assert(unread_bytes > -1);
    // put cursor after string
    if(fseek(file, -unread_bytes, SEEK_CUR)!= 0){
        fprintf(stderr, "ERROR : fseek failed\n");
        exit(EXIT_FAILURE);
    } 
    return str_len + 1; // including '\0'
}