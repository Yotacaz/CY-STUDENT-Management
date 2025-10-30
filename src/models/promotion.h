#ifndef PROMOTION_H
#define PROMOTION_H
#include "students.h"
// #include "course.h"

/// @file promotion.h
/// @brief Structure and functions to handle promotion data

DECLARE_DYN_TABLE(Student *, StudentsTab)

/// @brief Structure representing a promotion containing students and courses dynamic tables.
typedef struct promotion
{
    StudentsTab *stu_dtab;
    CoursesTab *courses;
} Promotion;

// Function prototypes

/// @brief compare two students by their IDs. To be used in qsort
/// @param a first student
/// @param b second student
/// @return -1 if a<b, 0 if a==b, 1 if a>b
int compare_student_key(const void *a, const void *b);

/// @brief compare two students by their IDs. To be used in qsort
/// @param s1 first student
/// @param s2 second student
/// @return -1 if s1<s2, 0 if s1==s2, 1 if s1>s2
int compare_student_id(const void *s1, const void *s2);

/// @brief Search for a student in a StudentsTab by its ID using binary search
/// @param prom the StudentsTab to search in
/// @param searched_id the ID to search for
/// @return the found Student or NULL if not found
Student *student_tab_bsearch(StudentsTab *prom, unsigned int searched_id);

/// @brief Allocate the followed courses for each student in the StudentsTab
/// @param prom the StudentsTab
/// @param n_courses the number of courses to allocate for each student
void allocate_students_courses(StudentsTab *prom, int n_courses);

/// @brief Initialise a promotion given its courses and students tables.
/// @param ctab Dynamic table of the courses. Can be NULL.
/// @param stu_dtab Dynamic table of the students. Can be NULL.
/// @return
Promotion *init_promotion(CoursesTab *ctab, StudentsTab *stu_dtab);

/// @brief Free a promotion and its contents using the provided free functions.
/// If free_course_f or free_student_f is NULL, that mean we don't want to free the relevant data
/// @param prom the promotion to free
/// @param free_student_f function to free a student
/// @param free_course_f function to free a course
void free_promotion(Promotion *prom, void (*free_student_f)(Student *), void (*free_course_f)(Course *));

/// @brief Print a promotion (courses and students)
/// @param prom the promotion to print
void print_promotion(Promotion *prom);

/// @brief Get the top students in a promotion based on their overall average
/// @param stu_dtab the students table
/// @param top_max_size the maximum number of top students to return
/// @return a StudentsTab containing the top students
StudentsTab *get_top_students(StudentsTab *stu_dtab, int top_max_size);

/// @brief Get the top students in a specific course within a promotion
/// @param prom the promotion
/// @param course_name the name of the course
/// @param top_max_size the maximum number of top students to return
/// @return a StudentsTab containing the top students in the specified course
StudentsTab *get_top_students_in_course(Promotion *prom, char *course_name, int top_max_size);

// print promotion is in the macro

#endif