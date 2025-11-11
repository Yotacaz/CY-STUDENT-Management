#include "load_bin.h"

Course *bin_load_course(FILE *file)
{
    assert(file);
    float coef = -1;
    char name[BUF_LEN];
    verify(fread(&(coef), sizeof(float), 1, file) == 1,
           "couldn't load course coefficient (float) while loading course from binary");
    verify(bin_read_string(file, name, BUF_LEN) > 0,
           "couldn't load course name (string) while loading course from binary");
    Course *cr = init_course(coef, name);
    assert(course_is_valid(cr));
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
    int age = -1;
    verify(fread(&(id), sizeof(unsigned int), 1, file) == 1,
           "couldn't load student id (unsigned int) while loading student from binary");
    verify(bin_read_string(file, name, BUF_LEN) > 0,
           "couldn't load student name (string) while loading student from binary");
    verify(bin_read_string(file, fname, BUF_LEN) > 0,
           "couldn't load student first name (string) while loading student from binary");
    verify(fread(&(avg), sizeof(float), 1, file) == 1,
           "couldn't load student average (float) while loading student from binary");
    verify(fread(&(n_courses), sizeof(int), 1, file) == 1,
           "couldn't load student number of courses (int) while loading student from binary");
    verify(fread(&(age), sizeof(int), 1, file) == 1,
           "couldn't load student age (int) while loading student from binary");
    Student *stu = init_student(name, fname, id, n_courses, age);
    assert(stu);
    stu->average = avg;
    for (int i = 0; i < n_courses; i++)
    {
        stu->f_courses[i] = bin_load_followed_course(file);
        assert(stu->f_courses[i]);
    }
    assert(student_is_valid(stu));
    return stu;
}

Followed_course *bin_load_followed_course(FILE *file)
{
    assert(file);
    float avg = -1;
    verify(fread(&(avg), sizeof(float), 1, file) == 1,
           "couldn't load followed course average (float) while loading followed course from "
           "binary");
    Followed_course *fcourse = init_followed_course(NULL);
    assert(fcourse);
    fcourse->average = avg;
    fcourse->grades = Grades_load_from_bin(file, NULL);
    assert(fcourse->grades);
    assert(followed_course_is_valid(fcourse));
    return fcourse;
}