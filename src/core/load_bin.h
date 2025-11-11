#ifndef LOAD_BIN_H
#define LOAD_BIN_H

/// @file load_bin.h
/// @brief Functions to load data from binary files, ***WARNING: binary file is NOT portable***
/// @see save_bin.h (modifications to one file must be reflected in the other)

#include "../models/promotion.h"
#include "../other/utils.h"

/// @brief Loads a course from a binary file. Order: coef, name
/// @param file the binary file
/// @return the loaded Course
Course *bin_load_course(FILE *file);

/// @brief Loads a student from a binary file. Order: id, name, fname, average, n_courses, followed
/// courses
/// @param file the binary file
/// @return the loaded Student
Student *bin_load_student(FILE *file);

/// @brief Loads a followed course from a binary file. Order: average, grades
/// @param file the binary file
/// @return the loaded Followed_course
Followed_course *bin_load_followed_course(FILE *file);

#endif