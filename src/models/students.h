#ifndef STUDENTS_H
#define STUDENTS_H
#include <stdlib.h>
#include <string.h>

/// @file students.h
/// @brief Structure and functions to handle students data

#include "course.h"
#include "followed_course.h"

/// @brief Set to true to print the followed courses when printing a student
#define PRINT_STUDENT_COURSES false 

/// @brief Structure representing a student.
/// We suppose here that every student follows the same number of courses (n_courses).
/// In the f_courses table, the courses are stored in the same order as in the CoursesTab of the promotion.
typedef struct student
{
    Followed_course **f_courses; // table of followed courses
    char *name;
    char *fname;
    int n_courses; // duplicated information if n_courses followed is constant
    float average;
    unsigned int id;
} Student;

/// @brief Initialise a student, its followed courses are initialised to NULL and n_courses to 0.
/// Additionally, average is initialised to -1
/// @param name the name of the student
/// @param first_name the first name of the student
/// @param student_id the unique identifier of the student
/// @return the allocated student
Student *init_student(char *name, char *first_name, unsigned int student_id, int n_courses);

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

/// @brief Check if a student is valid (not NULL and has its fields allocated)
/// @param stu the student to check
/// @return true if valid, false otherwise
static inline bool student_is_valid(Student *stu)
{
    return stu != NULL && stu->fname && stu->name && stu->f_courses;
}

/// @brief Print a student and its followed courses
/// @param stu the student to print
void print_student(Student *stu);

#endif