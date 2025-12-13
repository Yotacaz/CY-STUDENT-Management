#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

size_t bin_read_string(FILE *file, char buf[], long buf_size)
{
    assert(file);
    assert(buf && buf_size > 0);
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
    if (fseek(file, -unread_bytes, SEEK_CUR) != 0)
    {
        fprintf(stderr, "ERROR : fseek failed\n");
        exit(EXIT_FAILURE);
    }
    return str_len + 1; // including '\0'
}

char *scan_str_of_len_between(size_t min_str_len, size_t max_str_len, char *prompt_msg)
{
    assert(min_str_len > 0ul);
    assert(min_str_len < max_str_len);
    assert(prompt_msg != NULL);

    char *buf = malloc(sizeof(char) * (max_str_len + 3));
    // we want an extra two char, one for reading an extra char from stdin to
    // ensure the text isn't too long, the other for the \0
    verify(buf, "malloc error");
    buf[min_str_len + 2] = '\0';
    bool incorrect_len = true; // to check if everything was successfully scanned
    size_t len = 0;
    do
    {
        printf("%s (between %zu and %zu characters) : \n", prompt_msg, min_str_len, max_str_len);
        verify(fgets(buf, max_str_len + 3, stdin) != NULL,
               "fgets failed"); // read wished_str_len + 1 char + \n + \0

        len = strlen(buf) - 1; // do not count the '\n' (or the buffer char if buffer
                               // couldn't collect everything)
        if (len >= min_str_len && len <= max_str_len && buf[len] == '\n')
        {
            incorrect_len = false;
        }
        else if (len < min_str_len)
        {

            // buf[len] == '\n'
            buf[len] = '\0';
            printf(RED "Error : too few characters (%zu received, expected between %zu and %zu; "
                       "string scanned : "
                       "'%s')\n" RESET,
                   len, min_str_len, max_str_len, buf);
        }
        else
        {
            if (buf[len] != '\n')
            {
                int c = 0;
                while ((c = getchar()) != '\n' && c != EOF)
                {
                    // Clear the remaining characters in the input buffer
                }
            }
            buf[len] = '\0';
            printf(RED "Error : too many characters (%zu received, expected between %zu and %zu; "
                       "string scanned : "
                       "'%s')\n" RESET,
                   len, min_str_len, max_str_len, buf);
        }

    } while (incorrect_len);
    buf[len] = '\0';             // Remove the newline character
    buf = realloc(buf, len + 1); //+1 for '\0'
    verify(buf, "realloc error");
    return buf;
}

void print_str_tab(char **tab, int len)
{
    assert(tab && len > 0);
    for (int i = 0; i < len; i++)
    {
        printf("%d - %s,\n", i, tab[i]);
    }
}

void free_tab_with_alloc(char **tab, int len)
{
    assert(tab && len > 0);
    for (int i = 0; i < len; i++)
    {
        free(tab[i]);
    }
    free(tab);
}