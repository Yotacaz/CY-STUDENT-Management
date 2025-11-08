#include "../lib/student_api.h"

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

    allocate_students_courses(stu_dtab, courses->size); // allocate the proper grades dynamic tables (size supposed const for simplicity)
    // read last section from file
    set_cursor_to_next_section(sections[2], file);
    load_grades_data(stu_dtab, courses, file);
    Promotion *prom = init_promotion(courses, stu_dtab);
    fclose(file);
    return prom;
}

int API_save_to_binary_file(CLASS_DATA *prom, char *file_path)
{
    assert(prom && file_path);
    FILE *file = fopen(file_path, "wb");
    if (!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    CoursesTab_save_to_bin(prom->courses, file, bin_save_course);
    StudentsTab_save_to_bin(prom->stu_dtab, file, bin_save_student);
    fclose(file);
    return 1;
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

void API_unload(CLASS_DATA *pClass)
{
    free_promotion(pClass, free_student, free_course);
}

char **API_get_best_students(CLASS_DATA *pClass) {
    verify(pClass, "in API_get_best_students, pClass is NULL");
    StudentsTab* stu_dtab = get_top_students(pClass->stu_dtab, SIZE_TOP1);
    assert(stu_dtab);
    // Student ** tab = stu_dtab->tab;
    // int size = stu_dtab->size;
    // char **names = malloc(sizeof(char*)* size);
    // verify(names, "malloc error");
    // for (int i = 0; i < size; i++)
    // {
    //     // char*  name = tab[i];

    // }
    return NULL;
}