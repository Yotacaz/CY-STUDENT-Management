#ifndef STUDENTS_H
#define STUDENTS_H
/// @file students.h
/// @brief Structure and functions to handle students data

#include <stdlib.h>
#include <string.h>

#include "course.h"
#include "followed_course.h"
#include "../other/utils.h"

/// @brief Define to print the followed courses when printing a student
// #define PRINT_STUDENT_COURSES

#ifndef AGE_MIN
/// @brief Minimum valid ages for students
#define AGE_MIN 10
#endif
#ifndef AGE_MAX
/// @brief Maximum valid ages for students
#define AGE_MAX 100
#endif

/// @brief Structure representing a student.
/// We suppose here that every student follows the same number of courses (n_courses).
/// In the f_courses table, the courses are stored in the same order as in the CoursesTab of the
/// promotion.
typedef struct student
{
    ///@brief bitmask representing validated courses\n
    /// if bit n is 1, course n (in enum CourseIndex) is validated
    /// else not validated
    /// used to quickly check if a student has validated a set of courses
    /// (see student_has_validated function)
    __uint32_t course_validation_mask;
    ///@brief table of followed courses, **is ordered alphabetically**
    Followed_course **f_courses;
    ///@brief last name
    char *name;
    ///@brief first name
    char *fname;
    ///@brief duplicated information if n_courses followed is constant
    int n_courses;
    ///@brief age of the student
    int age;
    ///@brief general average of the student over all followed courses
    float average;
    ///@brief unique identifier of the student
    unsigned int id;
} Student;

/// @brief Initialise a student, its followed courses are initialised to NULL and n_courses to 0.
/// Additionally, average is initialised to -1
/// @param name the name of the student
/// @param first_name the first name of the student
/// @param student_id the unique identifier of the student
/// @param n_courses the number of courses the student will follow
/// @param age the age of the student
/// @return the allocated student
Student *init_student(char *name, char *first_name, unsigned int student_id, int n_courses,
                      int age);

/// @brief Free a student and all its followed courses
/// @param stu the student to free
void free_student(Student *stu);

/// @brief Add a grade to a student for a specific course, note that the average is not updated
/// @param stu the student
/// @param ctab the courses table (to get the index of the course)
/// @param course_name the name of the course
/// @param grade the grade to add
void add_grade_to_student(Student *stu, CoursesTab *ctab, char *course_name, float grade);

/// @brief Get the general average of a student given its followed courses and the courses table.
/// Does not take into account followed courses with invalid average.
/// @param stu the student
/// @param ctab the courses table
/// @return the general average of the student, -1 if no grades
static inline float get_student_general_avg(Student *stu, CoursesTab *ctab)
{
    float total_grade = 0;
    float total_coef = 0;
    for (int i = 0; i < ctab->size; i++)
    {
        float avg = stu->f_courses[i]->average;
        if (avg > GRADE_MIN && avg < GRADE_MAX)
        {
            float coef = ctab->tab[i]->coef;
            total_grade += stu->f_courses[i]->average * coef;
            total_coef += coef;
        }
    }
    return total_coef > 0 ? total_grade / total_coef : -1;
}

/// @brief Check if an age is valid
/// This function prints invalidity reasons to stderr.
/// @param age the age to check
/// @return true if valid, false otherwise
static inline bool age_is_valid(int age)
{
    if (age < AGE_MIN || age > AGE_MAX)
    {
        fprintf(stderr, BOLD_RED "WARNING : invalid age %d (AGE_MIN=%d, AGE_MAX=%d)\n" RESET, age,
                AGE_MIN, AGE_MAX);
        return false;
    }
    return true;
}

/// @brief Check if a student is valid : -1 as average is allowed (for when not yet calculated).
/// This function prints invalidity reasons to stderr.
/// @param stu the student to check
/// @return true if valid, false otherwise
bool student_is_valid(Student *stu);

/// @brief Print a student and its followed courses
/// @param stu the student to print
void print_student(Student *stu);

/// @brief Print the validation status of a student per field (sciences/humanities)
/// @param stu the student to print
void print_student_validation(Student *stu);

/// @brief Update the course validation bitmask of a student for its followed courses
/// @param stu the student
void update_student_bitmask(Student *stu);

/// @brief Check if a student has validated all courses in the given bitmask
/// @param stu the student
/// @param courses_bitmask the bitmask representing the courses to check
/// @return true if the student has validated all courses in the bitmask, false otherwise
static inline bool student_has_validated(Student *stu, __uint32_t courses_bitmask)
{
    assert(student_is_valid(stu));
    return (stu->course_validation_mask & courses_bitmask) == courses_bitmask;
}

#endif