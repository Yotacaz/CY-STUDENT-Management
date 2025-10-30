#include "load_bin.h"

Promotion *bin_load_promotion(char *file_path)
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

Course *bin_load_course(FILE *file)
{
    assert(file);
    float coef = -1;
    char name[BUF_LEN];
    assert(fread(&(coef), sizeof(float), 1, file) == 1);
    assert(bin_read_string(file, name, BUF_LEN) > 0);
    Course *cr = init_course(coef, name);
    assert(cr);
    return cr;
}

Student *bin_load_student(FILE *file)
{
    assert(file);
    unsigned int id = 0;
    char name[BUF_LEN];
    char fname[BUF_LEN];
    float avg = -1;
    int n_courses = -1;
    assert(fread(&(id), sizeof(unsigned int), 1, file) == 1);
    assert(bin_read_string(file, name, BUF_LEN) > 0);
    assert(bin_read_string(file, fname, BUF_LEN) > 0);
    assert(fread(&(avg), sizeof(float), 1, file) == 1);
    assert(fread(&(n_courses), sizeof(int), 1, file) == 1);
    Student *stu = init_student(name, fname, id, n_courses);
    assert(stu);
    stu->average = avg;
    for (int i = 0; i < n_courses; i++)
    {
        stu->f_courses[i] = bin_load_followed_course(file);
        assert(stu->f_courses[i]);
    }
    return stu;
}

Followed_course *bin_load_followed_course(FILE *file)
{
    assert(file);
    float avg = -1;
    assert(fread(&(avg), sizeof(float), 1, file) == 1);
    Followed_course *fcourse = init_followed_course(NULL);
    assert(fcourse);
    fcourse->average = avg;
    fcourse->grades = Grades_load_from_bin(file, NULL);
    assert(fcourse->grades);
    return fcourse;
}