#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "course.h"

DEFINE_DYN_TABLE(Course *, CoursesTab)

Course *init_course(float coef, char *course_name)
{
    assert(course_name && coef_is_valid(coef));
    Course *crs = (Course *)malloc(sizeof(Course));
    verify(crs, "malloc error");
    crs->coef = coef;
    size_t len = strlen(course_name);
    crs->name = (char *)malloc(sizeof(char) * (len + 1));
    verify(crs->name, "malloc error");
    strcpy(crs->name, course_name);
    crs->name[len] = '\0';
    return crs;
}

void free_course(Course *course)
{
    assert(course_is_valid(course));
    free(course->name);
    course->name = NULL;
    free(course);
}

int compare_courses(const void *c1, const void *c2)
{
    const Course *const *pc1 = c1; // make qsort happy
    const Course *const *pc2 = c2;
    return strcmp((*pc1)->name, (*pc2)->name);
}

int get_course_index(CoursesTab *courses, char *searched_name)
{
    // we could use bsearch but it would give us a pointer and not an index (cleaner this way)
    assert(CoursesTab_is_valid(courses, course_is_valid) && searched_name);
    int left = 0;
    int right = courses->size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(courses->tab[mid]->name, searched_name);
        if (cmp == 0) // found
        {
            return mid;
        }
        else if (cmp < 0) // courses->tab[i] < searched_name
        {
            left = mid + 1;
        }
        else // courses->tab[i] > searched_name
        {
            right = mid - 1;
        }
    }
    return -1;
}

void print_course(Course *course)
{
    if (course == NULL)
    {
        printf("Course is NULL\n");
        return;
    }
    assert(course->name);
    printf("%s: %.2f, \n", course->name, course->coef);
}

bool course_is_valid(Course *course)
{
    if (!course)
    {
        fprintf(stderr, BOLD_RED "ERROR : course is NULL\n" RESET);
        return false;
    }
    if (!coef_is_valid(course->coef))
    {
        return false;
    }
    if (!course->name)
    {
        fprintf(stderr, BOLD_RED "ERROR : course name is NULL\n" RESET);
        return false;
    }
    return true;
}

bool coef_is_valid(float coef)
{
    if (coef < COEF_MIN || coef > COEF_MAX)
    {
        fprintf(stderr,
                BOLD_RED "ERROR : grade value %f is invalid"
                         "(COEF_MIN = %f , COEF_MAX = %f) \n" RESET,
                coef, COEF_MIN, COEF_MAX);
        return false;
    }
    return true;
}