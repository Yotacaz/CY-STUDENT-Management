#ifndef PROJECT_INFO_H
#define PROJECT_INFO_H

/// @file project_info.h
/// @brief Project information macros

#include <stdio.h>

/// Project version
#define PROJECT_VERSION "0.4.2"

/// Author information
#define AUTHOR "Martin CRISSOT (https://github.com/Yotacaz/)"

/// Project name
#define PROJECT_NAME "CY-STUDENT MANAGEMENT"

/// @brief Macro to print project information
#define PRINT_PROJECT_INFO()                                                                       \
    printf(BOLD_BLU PROJECT_NAME " (" PROJECT_VERSION ") | Author : " AUTHOR "\n" RESET)

#endif