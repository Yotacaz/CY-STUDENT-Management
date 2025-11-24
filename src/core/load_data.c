#include "load_data.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

StudentsTab *load_student_tab_data(FILE *file)
{
    // "if it work, don't fix it"
    assert(file);
    unsigned int stu_id = 0;
    char name[BUF_LEN];
    char fname[BUF_LEN];
    char format[BUF_LEN];
    int age = -1;
    StudentsTab *stu_dtab = StudentsTab_init();
    // create the string "%u;%BUF_LEN-1[^;];%BUF_LEN-1[^;\n];%d\n" (safe version of
    // "%u;%s;%s;%d\n"):
    int n_char =
            snprintf(format, BUF_LEN, "%%u;%%%d[^;\n];%%%d[^;\n];%%d\n", BUF_LEN - 1, BUF_LEN - 1);
    assert(n_char > 0 && n_char < BUF_LEN); // no error or overflow
    while (fscanf(file, format, &stu_id, name, fname, &age) == 4)
    {
        // we don't know the number of courses yet
        verify(age_is_valid(age), "invalid age while loading student data from text file");
        Student *stu = init_student(name, fname, stu_id, 0, age);
        assert(student_is_valid(stu));
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
    assert(CoursesTab_is_valid(courses, course_is_valid));

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
            assert(course_is_valid(cr));
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

void load_grades_data(Promotion *prom, FILE *file)
{
    // TODO : are the asserts ok in this function ?
    assert(file && promotion_is_valid(prom));
    StudentsTab *stu_dtab = prom->stu_dtab;
    CoursesTab *courses = prom->courses;
    // data format is id;nom;note
    char buf[BUF_LEN];
    while (fgets(buf, BUF_LEN, file) == buf && isdigit(*buf))
    {
        char *p = buf;

        // parse unsigned int
        //! WARNING NO OUT OF UNSIGNED INT RANGE DETECTION
        unsigned int id = 0;    //student id
        while (*p >= '0' && *p <= '9')
        {
            id = id * 10 + (*p - '0');
            p++;
        }
        assert(*p == ';');
        p++;

        // parse string
        char *course_name = p;
        while (*p != ';' && *p != '\n' && *p != '\0')
        {
            p++;
        }
        assert(*p == ';');
        *p = '\0';
        p++;

        // parse grade (float)
        int sign = 1;
        if (*p == '-')
        {
            sign = -1;
            p++;
        }

        float int_part = 0;
        while (*p >= '0' && *p <= '9')
        {
            int_part = int_part * 10 + (*p - '0');
            p++;
        }

        float frac_part = 0;
        float base = 0.1f;
        if (*p == '.')
        {
            p++;
            while (*p >= '0' && *p <= '9')
            {
                frac_part += (*p - '0') * base;
                base *= 0.1f;
                p++;
            }
        }
        float grade = sign * (int_part + frac_part);

        // applying modifications
        Student *stu = student_tab_bsearch(stu_dtab, id);
        assert(stu);
        add_grade_to_student(stu, courses, course_name, grade);
    }
    verify(!ferror(file), "Error occurred while reading grades from text file");
    // updating grades avg :
    calculate_all_student_average(prom);
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
    if (feof(file))
    {
        printf(BOLD_RED "ERROR : feof reached while looking for section title %s. " RESET
                        "buffer : %s\n",
               section.section_title, buf);
        exit(EXIT_FAILURE);
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
    if (feof(file))
    {
        printf(BOLD_RED "ERROR : feof reached while looking for section header %s. " RESET
                        "buffer : %s\n",
               section.section_title, buf);
        exit(EXIT_FAILURE);
    }
    assert(!ferror(file));
}