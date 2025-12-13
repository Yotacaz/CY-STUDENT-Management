#include "save_bin.h"

void bin_save_prom(Promotion *prom, FILE *file)
{
    assert(promotion_is_valid(prom) && file);
    CoursesTab_save_to_bin(prom->courses, file, bin_save_course);
    StudentsTab_save_to_bin(prom->stu_dtab, file, bin_save_student);
}

void bin_save_course(Course *course, FILE *file)
{
    assert(file);
    assert(course_is_valid(course));
    verify(fwrite(&(course->coef), sizeof(float), 1, file) == 1,
           "couldn't save course coefficient (float) while saving to binary");
    size_t len = strlen(course->name) + 1;
    verify(fwrite(course->name, sizeof(char), len, file) == len,
           "couldn't save course name (string) while saving to binary");
}

void bin_save_student(Student *stu, FILE *file)
{
    assert(file && student_is_valid(stu));
    verify(fwrite(&(stu->id), sizeof(unsigned int), 1, file) == 1,
           "couldn't save student id (unsigned int) while saving to binary");
    size_t name_len = strlen(stu->name);
    verify(fwrite(stu->name, sizeof(char), name_len + 1, file) == (name_len + 1),
           "couldn't save student name (char *) while saving to binary");
    size_t fname_len = strlen(stu->fname);
    verify(fwrite(stu->fname, sizeof(char), fname_len + 1, file) == (fname_len + 1),
           "couldn't save student first name (char *) while saving student to binary");
    verify(fwrite(&(stu->average), sizeof(float), 1, file) == 1,
           "couldn't save student average (float) while saving to binary");
    verify(fwrite(&(stu->n_courses), sizeof(int), 1, file) == 1,
           "couldn't save student number of followed courses (int) while saving to binary");
    verify(fwrite(&(stu->age), sizeof(int), 1, file) == 1,
           "couldn't save student age (int) while saving to binary");
    for (int i = 0; i < stu->n_courses; i++)
    {
        bin_save_followed_course(stu->f_courses[i], file);
    }
}

void bin_save_followed_course(Followed_course *fcourse, FILE *file)
{
    assert(file && followed_course_is_valid(fcourse));
    verify(fwrite(&(fcourse->average), sizeof(float), 1, file) == 1,
           "couldn't save followed course average (float) while saving followed course to binary");
    Grades_save_to_bin(fcourse->grades, file, NULL);
}