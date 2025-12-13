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
    ///@brief dynamic table of students
    StudentsTab *stu_dtab;
    ///@brief dynamic table of courses
    CoursesTab *courses;
    ///@brief compare function to sort students tab
    int (*compare_student)(const void *, const void *);
} Promotion;

// Function prototypes

/// @brief compare two students by their IDs. To be used in bsearch for searching in a growing order
/// @param key pointer to first student
/// @param b double pointer to second student
/// @return -1 if key<b, 0 if key==b, 1 if key>b
int compare_student_to_key(const void *key, const void *b);

/// @brief compare two students by their IDs. To be used in qsort
/// @param s1 first student
/// @param s2 second student
/// @return -1 if s1<s2, 0 if s1==s2, 1 if s1>s2
int compare_student_id(const void *s1, const void *s2);

/// @brief compare two students by their first names (alphabetical). To be used in qsort
/// @param a pointer to first student
/// @param b pointer to second student
/// @return negative if a<b, 0 if a==b, positive if a>b
int compare_student_fname(const void *a, const void *b);

/// @brief compare two students by their last names (alphabetical). To be used in qsort
/// @param a pointer to first student
/// @param b pointer to second student
/// @return negative if a<b, 0 if a==b, positive if a>b
int compare_student_name(const void *a, const void *b);

/// @brief compare two students by their general average. To be used in qsort
/// @param a pointer to first student
/// @param b pointer to second student
/// @return negative if a<b, 0 if a==b, positive if a>b
int compare_student_average(const void *a, const void *b);

/// @brief compare two students by their minimum grade among followed courses. To be used in qsort
/// @param a pointer to first student
/// @param b pointer to second student
/// @return negative if a<b, 0 if a==b, positive if a>b
int compare_student_minimum(const void *a, const void *b);

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
void free_promotion(Promotion *prom, void (*free_student_f)(Student *),
                    void (*free_course_f)(Course *));

/// @brief Print a promotion (courses and students)
/// @param prom the promotion to print
void print_promotion(Promotion *prom);

/// @brief Check if a promotion is valid. **WARNING **: does not check for unicity of student ids,
/// for this use students_id_are_sorted_and_unique. This function prints invalidity reasons to
/// stderr.
/// @param prom the promotion to check
/// @return true if valid, false otherwise
bool promotion_is_valid(Promotion *prom);

/// @brief Check if the students IDs in a StudentsTab are sorted in growing order and unique
/// @param stu_dtab the StudentsTab to check
/// @return true if sorted and unique, false otherwise
bool students_id_are_sorted_and_unique(StudentsTab *stu_dtab);

/// @brief Get the top students in a promotion based on their overall average
/// @param stu_dtab the students table
/// @param top_max_size the maximum number of top students to return
/// @return a StudentsTab containing the top students
StudentsTab *get_top_students(StudentsTab *stu_dtab, int top_max_size);

/// @brief Get the top students in a specific course within a promotion
/// @param prom the promotion
/// @param course_name the name of the course
/// @param top_max_size the maximum number of top students to return
/// @return a StudentsTab containing the top students in the specified course, NULL if course not
/// found
StudentsTab *get_top_students_in_course(Promotion *prom, char *course_name, int top_max_size);

/// @brief Calculate and update the overall average for all students in the promotion
/// and set validation bitmask to check if the student validate a followed course
/// @param prom the promotion
void evaluate_all_student_average(Promotion *prom);

/// @brief Get the names and first names of students in a StudentsTab
/// @param tab array of Student pointers
/// @param n number of students in the array
/// @return an array of strings containing the names and first names of the students
char **get_students_names_and_fname(Student **tab, int n);

#endif