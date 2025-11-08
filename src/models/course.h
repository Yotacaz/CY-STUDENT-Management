#ifndef COURSE_H
#define COURSE_H

/// @file course.h
/// @brief Structure and functions to handle courses data

#include "../other/dyn_table.h"
#include "../other/utils.h"

#ifndef COEF_MIN
#define COEF_MIN 0.0f
#endif

#ifndef COEF_MAX
#define COEF_MAX 1000.0f
#endif

/// @brief Structure containing the name and the coefficient of a course. 
///Should be stored in a dynamic table inside a promotion
typedef struct course
{
    char * name;  
    float coef;
}Course;

DECLARE_DYN_TABLE(Course*, CoursesTab)

/// @brief allocate and initialise a course
/// @param coef coef of the course
/// @param course_name name of the course
/// @return the allocated value
Course * init_course(float coef, char * course_name);

/// @brief Free a course
/// @param course the course to free
void free_course(Course* course);

/// @brief compare two courses by their names. Should be used for qsort
/// @param c1 first course
/// @param c2 2nd course
/// @return the result of strcmp(name1, name2)
int compare_courses(const void *c1, const void *c2);



/// @brief Get a course index given a sorted courses tab and the searched name. Use binary search
/// @param courses the table
/// @param searched_name the name to search
/// @return the index of the Course in the courses table
int get_course_index(CoursesTab *tab, char *searched_name);

/// @brief print a course
/// @param course course to print
void print_course(Course *course);

/// @brief Check if a course is valid. This function prints invalidity reasons to stderr.
/// @param course the course to check
/// @return true if valid, false otherwise
bool course_is_valid(Course* course);

/// @brief Check if a coefficient is valid. This function prints invalidity reasons to stderr.
/// @param coef the coefficient value to check, must be between COEF_MIN and COEF_MAX
/// @return true if valid, false otherwise
bool coef_is_valid(float coef);

#endif