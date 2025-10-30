#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "course.h"

DEFINE_DYN_TABLE(Course *, CoursesTab)

Course *init_course(float coef, char *course_name)
{
    Course *crs = (Course *)malloc(sizeof(Course));
    assert(crs);
    assert(course_name);
    crs->coef = coef;
    size_t len = strlen(course_name);
    crs->name = (char *)malloc(sizeof(char) * (len + 1));
    assert(crs->name);
    crs->name = strcpy(crs->name, course_name);
    assert(crs->name);
    crs->name[len] = '\0';
    return crs;
}

void free_course(Course *course)
{
    assert(course);
    free(course->name);
    course->name = NULL;
    free(course);
}

int compare_courses(const void *c1, const void *c2)
{
    const Course *const *pc1 = c1;  //make qsort happy
    const Course *const *pc2 = c2;
    return strcmp((*pc1)->name, (*pc2)->name);
}

int get_course_index(CoursesTab *courses, char *searched_name)
{
    // we could use bsearch but it would give us a pointer and not an index (cleaner this way)
    assert(courses && searched_name);
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
        else    // courses->tab[i] > searched_name
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