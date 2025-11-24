#include "promotion.h"
#include <float.h>

DEFINE_DYN_TABLE(Student *, StudentsTab)

Promotion *init_promotion(CoursesTab *ctab, StudentsTab *stu_dtab)
{
    Promotion *prom = (Promotion *)malloc(sizeof(Promotion));
    verify(prom, "malloc error");
    prom->courses = ctab;
    prom->stu_dtab = stu_dtab;
    return prom;
}

// to use in qsort
int compare_student_id(const void *a, const void *b)
{
    const Student *const *pa = a;
    const Student *const *pb = b;
    const Student *s1 = *pa;
    const Student *s2 = *pb;
    return (s1->id > s2->id) - (s1->id < s2->id);
}

// to use in bsearch
int compare_student_key(const void *a, const void *b)
{
    // for a weird bug i encountered
    const Student *key = a;
    const Student *const *elem = b;
    return (key->id > (*elem)->id) - (key->id < (*elem)->id);
}

Student *student_tab_bsearch(StudentsTab *stu_dtab, unsigned int searched_id)
{
    assert(StudentsTab_is_valid(stu_dtab, student_is_valid));
    Student tmp = {.id = searched_id}; // dummy student
    Student **res = (Student **)bsearch(&tmp, stu_dtab->tab, stu_dtab->size, sizeof(Student *),
                                        compare_student_key);
    return res ? *res : NULL;
}

void allocate_students_courses(StudentsTab *stu_dtab, int n_courses)
{
    assert(StudentsTab_is_valid(stu_dtab, student_is_valid) && n_courses > 0);
    for (int i = 0; i < stu_dtab->size; i++)
    {
        Student *stu = stu_dtab->tab[i];
        stu->n_courses = n_courses;
        stu->f_courses = (Followed_course **)malloc(n_courses * sizeof(Followed_course *));
        assert(stu->f_courses);
        for (int j = 0; j < n_courses; j++)
        {
            stu->f_courses[j] = init_followed_course(Grades_init);
            assert(stu->f_courses[j]);
        }
    }
}

void print_promotion(Promotion *prom)
{
    assert(promotion_is_valid(prom));
    CoursesTab_print(prom->courses, print_course);
    printf(BOLD_BLU "\n-------------\n" RESET);
    StudentsTab_print(prom->stu_dtab, print_student);
}

void free_promotion(Promotion *prom, void (*free_student_f)(Student *),
                    void (*free_course_f)(Course *))
{
    assert(prom); // don't check its content
    // If free_course_f or free_student_f is NULL, that mean we don't want to free them
    if (free_course_f)
    {
        assert(CoursesTab_is_valid(prom->courses, course_is_valid));
        CoursesTab_free(prom->courses, free_course_f);
    }
    if (free_student_f)
    {
        assert(StudentsTab_is_valid(prom->stu_dtab, student_is_valid));
        StudentsTab_free(prom->stu_dtab, free_student_f);
    }
    prom->courses = NULL;
    prom->stu_dtab = NULL;
    free(prom);
}

bool promotion_is_valid(Promotion *prom)
{
    if (!prom)
    {
        fprintf(stderr, BOLD_RED "WARNING : promotion is NULL\n" RESET);
        return false;
    }
    return StudentsTab_is_valid(prom->stu_dtab, student_is_valid) &&
           CoursesTab_is_valid(prom->courses, course_is_valid);
}

bool students_id_are_sorted_and_unique(StudentsTab *stu_dtab)
{
    assert(StudentsTab_is_valid(stu_dtab, student_is_valid));
    for (int i = 1; i < stu_dtab->size; i++)
    {
        if (stu_dtab->tab[i - 1]->id >= stu_dtab->tab[i]->id)
        {
            fprintf(stderr,
                    BOLD_RED "WARNING : student id entry n°%d (id = %u) and %d (id = %u)"
                             " of student table aren't in strict growing order\n" RESET,
                    i, stu_dtab->tab[i]->id, i - 1, stu_dtab->tab[i - 1]->id);
            return false;
        }
    }
    return true;
}

StudentsTab *get_top_students(StudentsTab *stu_dtab, int top_max_size)
{
    assert(StudentsTab_is_valid(stu_dtab, student_is_valid) && top_max_size > 0);

    StudentsTab *top = StudentsTab_init();
    if (stu_dtab->size == 0)
    {
        return top;
    }
    float worst_in_top = -FLT_MAX; // worst average of the student in the top 10
    for (int i = 0; i < stu_dtab->size; i++)
    {
        Student *stu = stu_dtab->tab[i];
        if (top->size < top_max_size)
        {
            // Augment table size by one
            StudentsTab_push(stu, top);
        }
        else if (stu->average <= worst_in_top)
        {
            continue;
        }
        // shift every student whose position is inferior to stu
        int j = top->size - 2;
        while (j > -1 && stu->average > top->tab[j]->average)
        {
            top->tab[j + 1] = top->tab[j];
            j--;
        }
        top->tab[j + 1] = stu;
        worst_in_top = top->tab[top->size - 1]->average;
    }

    return top;
}

StudentsTab *get_top_students_in_course(Promotion *prom, char *course_name, int top_max_size)
{
    assert(promotion_is_valid(prom) && course_name && top_max_size > 0);

    int course_id = get_course_index(prom->courses, course_name);
    if (course_id < 0)
    {
        return NULL;
        // exit(EXIT_FAILURE);
    }

    StudentsTab *top = StudentsTab_init();
    StudentsTab *dtab = prom->stu_dtab;
    if (dtab->size == 0)
    {
        return top;
    }

    float worst_in_top = -FLT_MAX;

    for (int i = 0; i < dtab->size; i++)
    {
        Student *stu = dtab->tab[i];
        float avg = stu->f_courses[course_id]->average;

        if (top->size < top_max_size)
        {
            StudentsTab_push(stu, top);
        }
        else if (avg <= worst_in_top)
        {
            continue;
        }

        int j = top->size - 2;
        while (j > -1 && avg > top->tab[j]->f_courses[course_id]->average)
        {
            top->tab[j + 1] = top->tab[j];
            j--;
        }
        top->tab[j + 1] = stu;
        worst_in_top = top->tab[top_max_size - 1]->f_courses[course_id]->average;
    }
    return top;
}

void calculate_all_student_average(Promotion *prom) {
    assert(promotion_is_valid(prom));
    CoursesTab *courses = prom->courses;
    StudentsTab* stu_dtab = prom->stu_dtab;
    for (int i = 0; i < stu_dtab->size; i++)
    {
        Student *stu = stu_dtab->tab[i];
        for (int j = 0; j < courses->size; j++)
        {
            Followed_course *fcourse = stu->f_courses[j];
            fcourse->average = get_followed_course_avg(fcourse);
            assert(fcourse->average > GRADE_MIN && fcourse->average < GRADE_MAX);
        }
        stu->average = get_student_general_avg(stu, courses);
        assert(stu->average > GRADE_MIN && stu->average < GRADE_MAX);
    }
}