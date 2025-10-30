

#include "followed_course.h"

DEFINE_DYN_TABLE(float, Grades)

Followed_course *init_followed_course()
{
    Followed_course *f_course = malloc(sizeof(Followed_course));
    assert(f_course);
    f_course->average = -1;
    f_course->grades = Grades_init();
    assert(f_course);
    return f_course;
}

void free_followed_course(Followed_course *f_course)
{
    assert(f_course);
    Grades_free(f_course->grades, NULL);
    f_course->grades = NULL;
    free(f_course);
}


void print_fcourse(Followed_course *fcourse)
{
    if (!fcourse)
    {
        printf("Followed course is NULL\n");
        return;
    }
    printf("Average : %.2f\n", fcourse->average);
    Grades_print(fcourse->grades, print_float);
}