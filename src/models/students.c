#include <assert.h>

#include "students.h"
#include "../other/utils.h"

Student *init_student(char *name, char *first_name, unsigned int student_id, int n_courses, int age)
{
    assert(name);
    assert(first_name);
    assert(age_is_valid(age));
    assert(n_courses > -1);
    Student *stu = malloc(sizeof(Student));
    verify(stu, "malloc error");
    stu->id = student_id;
    stu->average = -1;
    stu->age = age;
    stu->n_courses = n_courses;
    stu->course_validation_mask = 0;
    if (n_courses > 0)
    {
        // IF n_courses is known, it is supposed sufficiently constant so that we don't need to
        // reallocate to many times
        stu->f_courses = (Followed_course **)calloc(
                n_courses, sizeof(Followed_course *)); // init at NULL for safety
        verify(stu->f_courses, "calloc error");
    }
    else
    {
        stu->f_courses =
                NULL; // we don't know how many courses a student will follow yet (but will later)
    }
    int name_len = strlen(name);
    int fname_len = strlen(first_name);
    stu->fname = (char *)malloc((fname_len + 1) * sizeof(char));
    stu->name = (char *)malloc((name_len + 1) * sizeof(char));
    verify(stu->fname && stu->name, "malloc error");
    strcpy(stu->fname, first_name);
    strcpy(stu->name, name);
    return stu;
}

void add_grade_to_student(Student *stu, CoursesTab *ctab, char *course_name, float grade)
{
    assert(grade_is_valid(grade));
    assert(student_is_valid(stu));
    long i = get_course_index_in_table(ctab, course_name);
    verify(i > -1, "course not found in courses table");
    Followed_course *fcourse = stu->f_courses[i];
    // recalculating avg supposing that all grades have same coef
    Grades_push(grade, fcourse->grades);
    //!\ to recalculate the average EACH time a grade is added (not recommended for performances +
    //! implementation)
    // uncomment the following line :
    // fcourse->average = get_followed_course_avg(fcourse);
    // stu->average = get_student_general_avg(stu, ctab);
}
// #define PRINT_STUDENT_COURSES
void print_student(Student *stu)
{
    assert(student_is_valid(stu));
    printf(BOLD_CYN UNDERLINE "Student %u: %s %s" RESET CYN ", age = %d, avg = %.2f\n", stu->id,
           stu->name, stu->fname, stu->age, stu->average);

    printf("Courses (%d): " RESET, stu->n_courses);
#ifndef PRINT_STUDENT_COURSES
    printf("\t(omitted)\n");
    return;
#endif

    for (int i = 0; i < stu->n_courses; i++)
    {
        printf(BOLD_BLU "\n%d - " RESET, i);
        print_fcourse(stu->f_courses[i]);
    }
}

void print_student_validation(Student *stu)
{
    assert(student_is_valid(stu));
    printf(BOLD_CYN UNDERLINE "Student %u: %s %s" RESET CYN ", age = %d, avg = %.2f\n", stu->id,
           stu->name, stu->fname, stu->age, stu->average);
    bool science_is_validated = student_has_validated(stu, SCIENCES_MASK);
    bool humanities_is_validated = student_has_validated(stu, HUMANITIES_MASK);
    if (science_is_validated && humanities_is_validated)
    {
        printf( "OO - every course is validated\n");
    }
    else if (!science_is_validated && !humanities_is_validated)
    {
        printf(RED "XX - humanities and science not validated\n" RESET);
    }
    else if (!science_is_validated)
    {
        printf( "O"RED"X - "RESET"humanities validated,"RED" science not validated\n" RESET);
    }
    else
    {
        printf( RED"X"RESET"O"RED" - humanities not validated,"RESET" science validated\n");
    }
}

void free_student(Student *stu)
{
    assert(student_is_valid(stu));
    free(stu->name);
    free(stu->fname);
    stu->fname = stu->name = NULL;
    for (int i = 0; i < stu->n_courses; i++)
    {
        free_followed_course(stu->f_courses[i]);
        stu->f_courses[i] = NULL;
    }
    free(stu->f_courses);
    stu->f_courses = NULL;
    free(stu);
}

bool student_is_valid(Student *stu)
{
    if (!stu)
    {
        fprintf(stderr, BOLD_RED "WARNING : student is NULL\n" RESET);
        return false;
    }
    // TODO Should I check string length ?
    else if (!stu->fname)
    {
        fprintf(stderr, BOLD_RED "WARNING : stu first name is NULL\n" RESET);
        return false;
    }
    else if (!stu->name)
    {
        fprintf(stderr, BOLD_RED "WARNING : stu name is NULL\n" RESET);
        return false;
    }
    else if (stu->average != -1 &&
             !grade_is_valid(stu->average)) // stu->average is -1 until later calculation (accepted
                                            // as special value)
    {
        fprintf(stderr, BOLD_RED "WARNING : invalid student average\n" RESET);
        return false;
    }
    int n_courses = stu->n_courses;
    if (n_courses < 0)
    {
        fprintf(stderr, BOLD_RED "WARNING : number of courses %d invalid\n" RESET, n_courses);
        return false;
    }
    else if (n_courses != 0)
    {
        if (!stu->f_courses)
        {
            fprintf(stderr, BOLD_RED "WARNING : student followed courses table is NULL\n" RESET);
            return false;
        }
        for (int i = 0; i < n_courses; i++)
        {
            if (!followed_course_is_valid(stu->f_courses[i]))
            {
                return false;
            }
        }
        if ((stu->course_validation_mask & ~((1U << n_courses) - 1)) !=
            0) // check that no invalid bits are set
        {
            // note : could have use YEAR_MASK instead of (1U << n_courses) - 1)
            fprintf(stderr,
                    BOLD_RED "WARNING : student course validation bitmask has invalid "
                             "bits set (n_courses = %d, bitmask = 0x%X, valid bits mask = "
                             "0x%X,YEAR_MASK : 0x%X)\n" RESET,
                    n_courses, stu->course_validation_mask, (1U << n_courses) - 1, YEAR_MASK);
            return false;
        }
    }
    else if (stu->f_courses)
    {
        fprintf(stderr, BOLD_RED "WARNING : number of courses 0, followed course table should "
                                 "therefore be NULL\n" RESET);
        return false;
    }
    return true;
}

void update_student_bitmask(Student *stu)
{
    assert(student_is_valid(stu));
    Followed_course **tab = stu->f_courses;
    for (int i = 0; i < stu->n_courses; i++)
    {
        // set i-th bit to 0 or 1 depending on if course is validated
        if (tab[i]->average >= GRADE_TO_VALIDATE)
        {
            stu->course_validation_mask |= 1 << i;
        }
        else
        {
            stu->course_validation_mask &= ~(1 << i);
        }
    }
}