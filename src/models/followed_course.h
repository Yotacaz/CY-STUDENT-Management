#ifndef FOLLOWED_COURSE_H
#define FOLLOWED_COURSE_H

/// @file followed_course.h
/// @brief Structure and functions to handle followed courses data

#include "../other/dyn_table.h"
#include "../other/utils.h"
#include <stdio.h>

DECLARE_DYN_TABLE(float, Grades)

#ifndef GRADE_MIN
/// @brief Minimum valid grade value
#define GRADE_MIN -0.0001
#endif
#ifndef GRADE_MAX
/// @brief Maximum valid grade value
#define GRADE_MAX 20.0001
#endif

/// @brief Grade value used to represent unvalidated courses
#define GRADE_TO_VALIDATE 9.9999

/// @brief Enum listing all possible courses
/// does not correspond to course id in the CoursesTable, just a convenient way to refer to courses.
/// WARNING : **ENUM MUST BE ORDERED ALPHABETICALLY**
typedef enum _cindex
{
    ALLEMAND,
    ANGLAIS,
    ARTPLASTIQUE,
    BIOLOGIE,
    CHIMIE,
    EPS,
    ECONOMIE,
    ESPAGNOL,
    FRANCAIS,
    GEOGRAPHIE,
    HISTOIRE,
    INFORMATIQUE,
    LATIN,
    MATHEMATIQUES,
    MUSIQUE,
    PHILOSOPHIE,
    PHYSIQUE,
    SCIENCESSOCIALES,
    SOCIOLOGIE,
    TECHNOLOGIE,
    NB_COURSES
} CourseIndex;

/// @brief Bitmask definitions for course categories
#define SCIENCES_MASK                                                                              \
    ((1 << BIOLOGIE) | (1 << CHIMIE) | (1 << ECONOMIE) | (1 << INFORMATIQUE) |                     \
     (1 << MATHEMATIQUES) | (1 << PHYSIQUE) | (1 << TECHNOLOGIE))

/// @brief Bitmask definitions for course categories
#define HUMANITIES_MASK                                                                            \
    ((1 << ALLEMAND) | (1 << ANGLAIS) | (1 << ARTPLASTIQUE) | (1 << ESPAGNOL) | (1 << FRANCAIS) |  \
     (1 << GEOGRAPHIE) | (1 << HISTOIRE) | (1 << LATIN) | (1 << MUSIQUE) | (1 << PHILOSOPHIE) |    \
     (1 << SCIENCESSOCIALES) | (1 << SOCIOLOGIE))

/// @brief Bitmask representing all courses in a year
#define YEAR_MASK (SCIENCES_MASK | HUMANITIES_MASK)

/// @brief The courses followed by a specific student.
typedef struct followed_course
{
    ///@brief dynamic table of grades
    Grades *grades;
    ///@brief average of the followed course
    float average;
} Followed_course;

/// @brief Create a followed course, it's average is initialised to -1 and
/// the Grades are allocated using the init_grades function. If init_grades NULL, this step will be
/// skipped.
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

/// @brief Print a followed course
/// @param fcourse the followed course to print
void print_fcourse(Followed_course *fcourse);

/// @brief Check if a followed course is valid. This function prints invalidity reasons to stderr.
/// @param fcourse the followed course to check
/// @return true if valid, false otherwise
bool followed_course_is_valid(Followed_course *fcourse);

/// @brief Check if a grade is valid
/// @param val the grade value to check
/// @return true if valid, false otherwise
bool grade_is_valid(float val);

#endif