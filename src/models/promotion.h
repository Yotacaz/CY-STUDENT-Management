#ifndef PROMOTION_H
#define PROMOTION_H
#include "students.h"
// #include "course.h"

DECLARE_DYN_TABLE(Student *, StudentsTab)

typedef struct promotion
{
    StudentsTab* stu_dtab;
    CoursesTab * courses;
}Promotion;

int compare_student_key(const void *a, const void *b);
int compare_student_id(const void *s1, const void *s2);
Student *student_tab_bsearch(StudentsTab *prom, unsigned int searched_id);
void allocate_students_courses(StudentsTab *prom, int n_courses);

Promotion *init_promotion(CoursesTab* ctab, StudentsTab* stu_dtab);
void free_promotion(Promotion *prom);
void print_promotion(Promotion *prom);
StudentsTab *get_top_students(StudentsTab *stu_dtab, int top_max_size);
StudentsTab *get_top_students_in_course(Promotion *prom, char *course_name, int top_max_size);


//print promotion is in the macro

#endif