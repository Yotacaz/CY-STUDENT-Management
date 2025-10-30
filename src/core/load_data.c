#include <stdio.h>
#include <errno.h>
#include "load_data.h"

Promotion *load_data(char *file_path)
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

StudentsTab *load_student_tab_data(FILE *file)
{
    //!\ WARNING : code PARTLY duplicated
    // "if it work, don't fix it"
    assert(file);
    unsigned int stu_id = 0;
    char name[BUF_LEN];
    char fname[BUF_LEN];
    char format[BUF_LEN];
    int age = -1;
    StudentsTab *stu_dtab = StudentsTab_init();
    // create the string "%u;%BUF_LEN-1[^;];%BUF_LEN-1[^;\n];%d\n" (safe version of "%u;%s;%s;%d\n"):
    int n_char = snprintf(format, BUF_LEN, "%%u;%%%d[^;\n];%%%d[^;\n];%%d\n", BUF_LEN - 1, BUF_LEN - 1);
    assert(n_char > 0 && n_char < BUF_LEN); // no error or overflow
    while (fscanf(file, format, &stu_id, name, fname, &age) == 4)
    {
        Student *stu = init_student(name, fname, stu_id);
        assert(stu);
        StudentsTab_push(stu, stu_dtab);
    }
    assert(!feof(file) && !ferror(file));
    StudentsTab_sort(stu_dtab, compare_student_id);
    return stu_dtab;
}

CoursesTab *load_courses_data(FILE *file)
{
    // That one didn't work, had to fix it :(
    assert(file);
    CoursesTab *courses = CoursesTab_init();
    assert(courses);

    char line[BUF_LEN];
    char course_name[BUF_LEN];
    float coef;
    // create the string format "%BUF_LEN-1[^;\n];%f\n" (safer version of "%s;%f\n"):
    char format[BUF_LEN];
    int n_char = snprintf(format, BUF_LEN, "%%%d[^;\n];%%f", BUF_LEN - 1);
    assert(n_char > 0 && n_char < BUF_LEN); // no error or overflow
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0'; // delete final \n left by fgets

        if (sscanf(line, format, course_name, &coef) == 2)
        {
            Course *cr = init_course(coef, course_name);
            assert(cr);
            CoursesTab_push(cr, courses);
        }
        else
        {
            // Put the cursor at the last line (so it can be read by other functions)
            fseek(file, -((long)strlen(line) + 1), SEEK_CUR);
            break;
        }
    }
    assert(!feof(file) && !ferror(file));
    CoursesTab_sort(courses, compare_courses);
    return courses;
}

void load_grades_data(StudentsTab *stu_dtab, CoursesTab *courses, FILE *file)
{
    //!\ WARNING : code PARTLY duplicated
    // "if it work, don't fix it"
    assert(file && stu_dtab && courses);
    unsigned int stu_id = 0;
    char course_name[BUF_LEN];
    float grade = -1;
    char format[BUF_LEN];
    // data format is id;nom;note
    // create the string format "%u;%BUF_LEN-1[^;\n];%f" (safer version of "%u;%s;%f"):
    int n_char = snprintf(format, BUF_LEN, "%%u;%%%d[^;\n];%%f", BUF_LEN - 1);
    assert(n_char > 0 && n_char < BUF_LEN); // no error or overflow
    int ret = 0;
    while ((ret = fscanf(file, format, &stu_id, course_name, &grade)) == 3)
    {
        Student *stu = student_tab_bsearch(stu_dtab, stu_id);
        add_grade_to_student(stu, courses, course_name, grade);
    }
    assert(!ferror(file));
    // calculating grades avg :
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

void set_cursor_to_next_section(const Section section, FILE *file)
{
    assert(file);
    char buf[BUF_LEN];
    while (fgets(buf, BUF_LEN, file))
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, section.section_title) == 0)
        {
            break; // section found
        }
    }
    assert(!feof(file)); // not found
    while (fgets(buf, BUF_LEN, file))
    {

        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, section.section_header) == 0)
        {
            break; // section found
        }
    }
    assert(!feof(file) && !ferror(file));
}