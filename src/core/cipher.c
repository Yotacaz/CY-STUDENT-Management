#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cipher.h"

unsigned char *xor_key(unsigned char rand_key[KEY_SIZE], unsigned char master_key[KEY_SIZE], unsigned char buf[KEY_SIZE],
              size_t key_sizes)
{
    assert(rand_key && master_key);
    assert(key_sizes == KEY_SIZE);
    for (size_t i = 0; i < key_sizes; i++)
    {
        buf[i] = master_key[i] ^ rand_key[i];
    }
    return buf;
}

void xor_file(FILE *input_file, FILE *output_file, unsigned char rand_key[KEY_SIZE], size_t key_size)
{
    assert(key_size == KEY_SIZE);
    assert(!ferror(input_file) && !ferror(output_file));

    unsigned char input_buffer[BUFSIZ];
    unsigned char output_buffer[BUFSIZ];
    size_t bytes_read = 0;
    while ((bytes_read = fread(input_buffer, sizeof(unsigned char), BUFSIZ, input_file)) > 0)
    {
        for (size_t i = 0, k = 0; i < bytes_read; i++, k++)
        {
            if (k >= key_size)
            {
                k = 0;
            }
            output_buffer[i] = input_buffer[i] ^ rand_key[k];
        }
        verify(fwrite(output_buffer, sizeof(unsigned char), bytes_read, output_file) == bytes_read,
               strerror(errno));
    }
}

unsigned char *set_master_key(char *user_str, unsigned char master_key_buf[KEY_SIZE], size_t master_key_len)
{
    assert(master_key_len == KEY_SIZE);
    assert(master_key_buf);
    size_t len = strlen(user_str);
    unsigned char key[16] = {0x01, 0x23, 0x45, 0x57, 0x89, 0xAB, 0xCD, 0xEF,
                    0x10, 0x32, 0x54, 0x75, 0x98, 0xBA, 0xDC, 0xFE};
    assert(sizeof(key) == master_key_len);
    memcpy(master_key_buf, key, master_key_len);
    for (size_t i = 0, k = 0; i < len; i++, k++)
    {
        if (k > 16)
        {
            k = 0;
        }
        master_key_buf[k] ^= user_str[i];
        master_key_buf[(k + 1) % 16] += user_str[i];
        master_key_buf[(k + 15) % 16] *= user_str[i];
    }
    return master_key_buf;
}

unsigned char *get_rand_key(unsigned char buf[KEY_SIZE], size_t key_len)
{
    assert(key_len == KEY_SIZE);
    assert(buf);
    verify(getentropy(buf, KEY_SIZE) == 0, strerror(errno));
    return buf;
}

unsigned char *decipher_key_from_file(FILE *file, unsigned char key_buf[KEY_SIZE], unsigned char master_key[KEY_SIZE],
                             size_t key_sizes)
{
    assert(!ferror(file));
    assert(key_buf);
    verify(fread(key_buf, 1, key_sizes, file) == key_sizes, strerror(errno));
    return xor_key(key_buf, master_key, key_buf, key_sizes);
}

void cipher_key_in_file(FILE *file, unsigned char plain_key[KEY_SIZE], unsigned char master_key[KEY_SIZE],
                        size_t key_sizes)
{
    assert(!ferror(file));
    assert(plain_key);
    assert(key_sizes == KEY_SIZE);
    unsigned char ciphered_key_buf[KEY_SIZE];
    xor_key(plain_key, master_key, ciphered_key_buf, key_sizes);
    verify(fwrite(ciphered_key_buf, 1, key_sizes, file) == key_sizes, strerror(errno));
}

void cipher_file(FILE *input_file, FILE *output_file)
{
    assert(!ferror(input_file) && !ferror(output_file));

    // getting the master key
    char *user_string =
            scan_str_of_len_between(KEY_SIZE, BUF_LEN,
                                    "Please provide user key string (longer key mean better "
                                    "entropy), you will need to remember it for deciphering");
    unsigned char master_key[KEY_SIZE];
    verify(set_master_key(user_string, master_key, KEY_SIZE) == master_key,
           "while getting the master key");

    unsigned char rand_key[KEY_SIZE];
    verify(get_rand_key(rand_key, KEY_SIZE) == rand_key, "while trying to get a random key");

    // writing the output file
    cipher_key_in_file(output_file, rand_key, master_key, KEY_SIZE);
    xor_file(input_file, output_file, rand_key, KEY_SIZE);

    free(user_string);
}

void decipher_file(FILE *input_file, FILE *output_file)
{
    assert(!ferror(input_file) && !ferror(output_file));
    // getting the master key
    char *user_string = scan_str_of_len_between(
            KEY_SIZE, BUF_LEN,
            "Please provide the user key string that you used to cipher the file");
    unsigned char master_key[KEY_SIZE];
    verify(set_master_key(user_string, master_key, KEY_SIZE) == master_key,
           "while getting the master key");

    // getting the plain key that was used to cipher the file
    unsigned char key_buf[KEY_SIZE];
    decipher_key_from_file(input_file, key_buf, master_key, KEY_SIZE);

    // writing to the output file
    xor_file(input_file, output_file, key_buf, KEY_SIZE);

    free(user_string);
}