#include <arpa/inet.h>
#include "save_bin.h"

void bin_save_promotion(Promotion *prom, char *file_path)
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
}

void bin_save_course(Course *course, FILE *file)
{
    assert(course && file);
    assert(fwrite(&(course->coef), sizeof(float), 1, file) == 1);
    size_t len = strlen(course->name) + 1;
    assert(fwrite(course->name, sizeof(char), len, file) == len);
}

void bin_save_student(Student *stu, FILE *file)
{
    assert(stu && file);
    assert(fwrite(&(stu->id), sizeof(unsigned int), 1, file) == 1);
    size_t name_len = strlen(stu->name);
    assert(fwrite(stu->name, sizeof(char), name_len + 1, file) == (name_len + 1));
    size_t fname_len = strlen(stu->fname);
    assert(fwrite(stu->fname, sizeof(char), fname_len + 1, file) == (fname_len + 1));
    assert(fwrite(&(stu->average), sizeof(float), 1, file) == 1);
    assert(fwrite(&(stu->n_courses), sizeof(int), 1, file) == 1);
    for (int i = 0; i < stu->n_courses; i++)
    {
        bin_save_followed_course(stu->f_courses[i], file);
    }
}

void bin_save_followed_course(Followed_course *fcourse, FILE *file)
{
    assert(fcourse && file);
    assert(fwrite(&(fcourse->average), sizeof(float), 1, file) == 1);
    Grades_save_to_bin(fcourse->grades, file, NULL);
}