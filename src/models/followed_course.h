#ifndef FOLLOWED_COURSE_H
#define FOLLOWED_COURSE_H

/// @file followed_course.h
/// @brief Structure and functions to handle followed courses data

#include <stdio.h>
#include "../other/dyn_table.h"
#include "../other/utils.h"

DECLARE_DYN_TABLE(float, Grades)

#define GRADE_MIN -0.0001
#define GRADE_MAX 20.0001

/// @brief The courses followed by a specific student.
typedef struct followed_course
{
    Grades *grades;
    float average;
} Followed_course;

/// @brief Create a followed course, it's average is initialised to -1 and
/// the Grades are allocated using the init_grades function. If init_grades NULL, this step will be skipped.
/// @param init_grades The function used to allocate
/// @return The created followed course
Followed_course *init_followed_course(Grades *(*init_grades)());

/// @brief Free a followed course
/// @param f_course the followed course to free
void free_followed_course(Followed_course *f_course);

/// @brief Get the average of a followed course given its grades
/// @param fcourse the followed course
/// @return the average of the followed course, -1 if no grades
static inline float get_followed_course_avg(Followed_course *fcourse)
{
    assert(fcourse);
    float total = 0;
    int n_elem = fcourse->grades->size;
    for (int i = 0; i < n_elem; i++)
    {
        total += fcourse->grades->tab[i];
    }
    return n_elem > 0 ? total / n_elem : -1;
}

/// @brief Print a float value with 2 decimals
/// Used as a callback for Grades_print
/// @param val the float to print
static inline void print_float(float val)
{
    printf("%.2f", val);
}

/// @brief Print a followed course
/// @param fcourse the followed course to print
void print_fcourse(Followed_course *fcourse);

#endif