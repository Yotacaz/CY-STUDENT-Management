#include <assert.h>

#include "students.h"

Student *init_student(char *name, char *first_name, unsigned int student_id, int n_courses)
{
    Student *stu = malloc(sizeof(Student));
    assert(stu);
    stu->id = student_id;
    stu->average = -1;
    stu->n_courses = n_courses;
    if (n_courses > 0)
    {
        // IF n_courses is known, it is supposed sufficiently constant so that we don't need to reallocate to many times
        stu->f_courses = (Followed_course **)calloc(n_courses, sizeof(Followed_course *)); // init at NULL for safety
        assert(stu->f_courses);
    }
    else
    {
        stu->f_courses = NULL; // we don't know how many courses a student will follow yet (but will later)
    }
    int name_len = strlen(name);
    int fname_len = strlen(first_name);
    stu->fname = (char *)malloc((fname_len + 1) * sizeof(char));
    stu->name = (char *)malloc((name_len + 1) * sizeof(char));
    assert(stu->fname && stu->name);
    stu->fname = strcpy(stu->fname, first_name);
    stu->name = strcpy(stu->name, name);
    assert(stu->fname && stu->name);
    return stu;
}

void add_grade_to_student(Student *stu, CoursesTab *ctab, char *course_name, float grade)
{
    assert(grade > GRADE_MIN && grade < GRADE_MAX);
    assert(student_is_valid(stu));
    long i = get_course_index(ctab, course_name);
    assert(i > -1);
    Followed_course *fcourse = stu->f_courses[i];
    // recalculating avg supposing that all grades have same coef
    Grades_push(grade, fcourse->grades);
    //!\ to recalculate the average EACH time a grade is added (not recommended for performances + implementation)
    // uncomment the following line :
    // fcourse->average = get_followed_course_avg(fcourse);
    // stu->average = get_student_general_avg(stu, ctab);
}

void print_student(Student *stu)
{
    // assert(n_courses > 0);
    if (stu == NULL)
    {
        printf("Student is NULL");
        exit(EXIT_FAILURE);
    }
    printf(BOLD_RED UNDERLINE"Student %u: %s %s"RESET RED", avg = %.2f\n", stu->id, stu->name, stu->fname, stu->average);

    printf("Courses (%d): \n"RESET, stu->n_courses);
    if (!PRINT_STUDENT_COURSES){
        printf("\t(omitted)\n");
        return;
    }
    for (int i = 0; i < stu->n_courses; i++)
    {
        printf(BOLD_BLU"%d - "RESET, i);
        print_fcourse(stu->f_courses[i]);
    }
}

void free_student(Student *stu)
{
    assert(stu);
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