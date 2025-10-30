#ifndef SAVE_BIN_H
#define SAVE_BIN_H

/// @file save_bin.h
/// @brief Functions to save data to binary files, ***WARNING: binary file is NOT portable***.
/// @see load_bin.h (modifications to one file must be reflected in the other)

#include "../models/promotion.h"


/// @brief Saves a promotion to a binary file. Order: courses, students
/// @param prom the promotion to save
/// @param file_path the path to the binary file
void bin_save_promotion(Promotion *prom, char *file_path);

/// @brief Saves a course to a binary file. Order: coef, name
/// @param course the course to save
/// @param file the binary file
void bin_save_course(Course *course, FILE *file);


/// @brief Saves a student to a binary file. Order: id, name, fname, average, n_courses, followed courses
/// @param stu the student to save
/// @param file the binary file
void bin_save_student(Student *stu, FILE *file);

/// @brief Saves a followed course to a binary file. Order: average, grades
/// @param fcourse the followed course to save
/// @param file the binary file
void bin_save_followed_course(Followed_course *fcourse, FILE *file);

#endif