#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../other/utils.h"
#include "followed_course.h"

DEFINE_DYN_TABLE(float, Grades)

Followed_course *init_followed_course(Grades *(*init_grades)())
{
    Followed_course *f_course = malloc(sizeof(Followed_course));
    verify(f_course, "malloc error");
    f_course->average = -1;
    if (init_grades != NULL)
    {
        f_course->grades = Grades_init();
        assert(Grades_is_valid(f_course->grades, grade_is_valid));
    }
    return f_course;
}

void free_followed_course(Followed_course *f_course)
{
    assert(followed_course_is_valid(f_course));
    Grades_free(f_course->grades, NULL);
    f_course->grades = NULL;
    free(f_course);
}

void print_fcourse(Followed_course *fcourse)
{
    assert(followed_course_is_valid(fcourse));
    printf("Average : %.2f\n", fcourse->average);
    Grades_print(fcourse->grades, print_float);
}

bool followed_course_is_valid(Followed_course *fcourse)
{
    if (!fcourse)
    {
        fprintf(stderr, BOLD_RED "ERROR : fcourse is NULL\n" RESET);
        return false;
    }
    if (fcourse->average != -1 &&
        !grade_is_valid(
                fcourse->average)) // average is = -1 until calculation (with non empty grades tab)
    {
        fprintf(stderr, BOLD_RED "^ Invalid followed course average\n" RESET);
        return false;
    }
    return Grades_is_valid(fcourse->grades, grade_is_valid);
}

bool grade_is_valid(float val)
{
    if (val < GRADE_MIN || val > GRADE_MAX)
    {
        fprintf(stderr,
                BOLD_RED "ERROR : grade value %f is invalid"
                         "(GRADE_MIN = %f , GRADE_MAX = %f) \n" RESET,
                val, GRADE_MIN, GRADE_MAX);
        return false;
    }
    return true;
}