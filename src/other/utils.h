#ifndef UTILS_H
#define UTILS_H
/// @file utils.h
/// @brief Various utility functions
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

///@brief buffer length for reading strings from files (binary or text)
#define BUF_LEN 4 * 1024

// normal text colors

///@brief Red text color for terminal output
#define RED "\x1B[0;31m"
///@brief Green text color for terminal output
#define GRN "\x1B[0;32m"
///@brief Yellow text color for terminal output
#define YEL "\x1B[0;33m"
///@brief Blue text color for terminal output
#define BLU "\x1B[0;34m"
///@brief Magenta text color for terminal output
#define MAG "\x1B[0;35m"
///@brief Cyan text color for terminal output
#define CYN "\x1B[0;36m"
///@brief White text color for terminal output
#define WHT "\x1B[0;37m"

// background colors

///@brief Red background color for terminal output
#define BG_RED "\x1B[41m"
///@brief Green background color for terminal output
#define BG_GRN "\x1B[42m"
///@brief Yellow background color for terminal output
#define BG_YEL "\x1B[43m"
///@brief Blue background color for terminal output
#define BG_BLU "\x1B[44m"
///@brief Magenta background color for terminal output
#define BG_MAG "\x1B[45m"
///@brief Cyan background color for terminal output
#define BG_CYN "\x1B[46m"
///@brief White background color for terminal output
#define BG_WHT "\x1B[47m"

// bold text colors
// (some terminals may not support bold colors)

///@brief Bold red text color for terminal output
#define BOLD_RED "\x1B[1;31m"
///@brief Bold green text color for terminal output
#define BOLD_GRN "\x1B[1;32m"
///@brief Bold yellow text color for terminal output
#define BOLD_YEL "\x1B[1;33m"
///@brief Bold blue text color for terminal output
#define BOLD_BLU "\x1B[1;34m"
///@brief Bold magenta text color for terminal output
#define BOLD_MAG "\x1B[1;35m"
///@brief Bold cyan text color for terminal output
#define BOLD_CYN "\x1B[1;36m"
///@brief Bold white text color for terminal output
#define BOLD_WHT "\x1B[1;37m"

///@brief Underline text style for terminal output
#define UNDERLINE "\x1B[4m"

///@brief Reset text formatting for terminal output
#define RESET "\x1B[0m"

/// @brief Read a null-terminated string from a binary file into a buffer, ensuring it fits within
/// the buffer size.
/// @param file the binary file
/// @param buf the buffer to store the string
/// @param buf_size the size of the buffer
/// @return the length of the read string (including the null terminator) or -1 on error
size_t bin_read_string(FILE *file, char buf[], long buf_size);

/// @brief Verify a condition and print an error message and exit if the condition is false
/// @param condition the condition to verify
/// @param err_msg the error message to print if the condition is false
static inline void verify(bool condition, char *err_msg)
{
    if (!condition)
    {
        fprintf(stderr, BOLD_RED "ERREUR : %s\n" RESET, err_msg);
        exit(EXIT_FAILURE);
    }
}

/// @brief Print a float value with 2 decimals
/// Used as a callback for Grades_print
/// @param val the float to print
static inline void print_float(float val) { printf("%.2f", val); }

#endif