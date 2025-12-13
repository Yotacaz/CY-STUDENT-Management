#ifndef CIPHER_H
#define CIPHER_H

#include "../other/utils.h"

/// @file cipher.h
/// @brief Functions for file ciphering and deciphering

///@brief Size of the keys used for ciphering/deciphering
#define KEY_SIZE 16UL

/// Function prototypes

/// @brief XOR two keys to produce a third key
/// @param rand_key the random key
/// @param master_key the master key
/// @param buf buffer to store the resulting key
/// @param key_sizes size of the keys (must be KEY_SIZE)
/// @return the resulting key stored in buf
unsigned char *xor_key(unsigned char rand_key[KEY_SIZE], unsigned char master_key[KEY_SIZE], unsigned char buf[KEY_SIZE],
              size_t key_sizes);

/// @brief XOR the contents of an input file with a random key and write to an output file.
/// @param input_file the input file to read from
/// @param output_file the output file to write to
/// @param rand_key the random key to use for XOR operation
/// @param key_size size of the key (must be KEY_SIZE)
void xor_file(FILE *input_file, FILE *output_file, unsigned char rand_key[KEY_SIZE], size_t key_size);

/// @brief Set the master key based on a user-provided string
/// @param user_str the user-provided string
/// @param master_key_buf buffer to store the master key
/// @param master_key_len length of the master key (must be KEY_SIZE)
/// @return the master key stored in master_key_buf
unsigned char *set_master_key(char *user_str, unsigned char master_key_buf[KEY_SIZE], size_t master_key_len);

/// @brief Generate a random key
/// @param buf buffer to store the random key
/// @param key_len length of the key (must be KEY_SIZE)
/// @return the random key stored in buf
unsigned char *get_rand_key(unsigned char buf[KEY_SIZE], size_t key_len);

/// @brief Decipher a key from a file using the master key
/// @param file the file to read the ciphered key from
/// @param key_buf buffer to store the deciphered key
/// @param master_key the master key to use for deciphering
/// @param key_sizes size of the key (must be KEY_SIZE)
/// @return the deciphered key stored in key_buf
unsigned char *decipher_key_from_file(FILE *file, unsigned char key_buf[KEY_SIZE], unsigned char master_key[KEY_SIZE],
                             size_t key_sizes);

/// @brief Cipher a key and write it to a file using the master key
/// @param file the file to write the ciphered key to
/// @param plain_key the plain key to cipher
/// @param master_key the master key to use for ciphering
/// @param key_sizes size of the key (must be KEY_SIZE)
void cipher_key_in_file(FILE *file, unsigned char plain_key[KEY_SIZE], unsigned char master_key[KEY_SIZE],
                        size_t key_sizes);

/// @brief Cipher the contents of an input file and write to an output file
/// @param input_file the input file to read from
/// @param output_file the output file to write to
void cipher_file(FILE *input_file, FILE *output_file);

/// @brief Decipher the contents of an input file and write to an output file
/// @param input_file the input file to read from
/// @param output_file the output file to write to
void decipher_file(FILE *input_file, FILE *output_file);

#endif