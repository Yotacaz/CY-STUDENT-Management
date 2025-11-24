#include "../lib/student_api.h"
#include "../src/core/load_bin.h"
#include "../src/core/load_data.h"
#include "../src/core/save_bin.h"
#include "../src/other/project_info.h"

CLASS_DATA *API_load_students(char *file_path)
{
    assert(file_path);
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur : %s inexistant.\n", file_path);
        exit(EXIT_FAILURE);
    }
    assert(!feof(file));
    const Section sections[] = {SECTION_CONTENT};
    assert(sizeof(sections) / sizeof(sections[0]) == 3);

    // read first section from file
    // Get to the title of the first part
    set_cursor_to_next_section(sections[0], file);
    StudentsTab *stu_dtab = load_student_tab_data(file);

    // read second section from file
    set_cursor_to_next_section(sections[1], file);
    CoursesTab *courses = load_courses_data(file);

    allocate_students_courses(stu_dtab, courses->size); // allocate the proper grades dynamic tables
                                                        // (size supposed const for simplicity)
    // read last section from file
    set_cursor_to_next_section(sections[2], file);
    Promotion *prom = init_promotion(courses, stu_dtab);
    load_grades_data(prom, file);
    fclose(file);
    return prom;
}

int API_save_to_binary_file(CLASS_DATA *pClass, char *file_path)
{
    Promotion *prom = (Promotion *)pClass;
    assert(promotion_is_valid(prom) && file_path);
    FILE *file = fopen(file_path, "wb");
    if (!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    bin_save_prom(prom, file);
    return fclose(file) == 0;
}

CLASS_DATA *API_restore_from_binary_file(char *file_path)
{
    assert(file_path);
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    CoursesTab *cr_dtab = CoursesTab_load_from_bin(file, bin_load_course);
    StudentsTab *stu_dtab = StudentsTab_load_from_bin(file, bin_load_student);
    assert(cr_dtab && stu_dtab);
    Promotion *prom = init_promotion(cr_dtab, stu_dtab);
    fclose(file);
    return prom;
}

void API_display(CLASS_DATA *prom)
{
    assert(prom);
    printf(BOLD_BLU PROJECT_NAME " (" PROJECT_VERSION ") | Author : " AUTHOR "\n" RESET);
    print_promotion(prom);
}

void API_unload(CLASS_DATA *pClass) { free_promotion(pClass, free_student, free_course); }

char **API_get_best_students(CLASS_DATA *pClass)
{
    Promotion *prom = (Promotion *)pClass;
    assert(promotion_is_valid(prom));
    StudentsTab *stu_dtab = get_top_students(prom->stu_dtab, SIZE_TOP1);
    assert(stu_dtab);
    Student **tab = stu_dtab->tab;
    int size = stu_dtab->size;
    return get_students_names_and_fname(stu_dtab->tab, stu_dtab->size);
}

char **API_get_best_students_in_course(CLASS_DATA *pClass, char *course)
{
    assert(course);
    assert(promotion_is_valid(pClass));
    StudentsTab *stu_dtab = get_top_students_in_course(pClass, course, SIZE_TOP2);
    if (!stu_dtab)
    {
        fprintf(stderr, BOLD_RED "Course ID not found\n" RESET);
        return NULL;
    }
    assert(StudentsTab_is_valid(stu_dtab, student_is_valid));
    return get_students_names_and_fname(stu_dtab->tab, stu_dtab->size);
}

int API_set_sorting_mode(CLASS_DATA *pClass, int mode)
{
    assert(promotion_is_valid(pClass));
    Promotion *prom = (Promotion *)pClass;
    switch (mode)
    {
    case STUDENT_ID:
        prom->compare_student = compare_student_id;
        break;
    case ALPHA_FIRST_NAME:
        prom->compare_student = compare_student_fname;
        break;
    case ALPHA_LAST_NAME:
        prom->compare_student = compare_student_name;
        break;
    case AVERAGE:
        prom->compare_student = compare_student_average;
        break;
    case MINIMUM:
        prom->compare_student = compare_student_minimum;
        break;
    default:
        fprintf(stderr, BOLD_RED "WARNING: incorrect sorting mode %d, sorting mode unchanged" RESET,
                mode);
        return 1;
    }
    return 0;
}

char **API_sort_students(CLASS_DATA *pClass)
{
    assert(promotion_is_valid(pClass));
    Promotion *prom = (Promotion *)pClass;
    StudentsTab *stu_dtab = prom->stu_dtab;
    StudentsTab_sort(stu_dtab, prom->compare_student);
    return get_students_names_and_fname(stu_dtab->tab, SIZE_TOP1);
}