#include "save.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "../models/promotion.h"

int test_save()
{
    // Create a sample promotion with courses and students
    CoursesTab *courses = CoursesTab_init();
    Course *math = init_course(2.0f, "Mathematics");
    Course *phys = init_course(1.5f, "Physics");
    Course *prog = init_course(2.0f, "Programming");
    CoursesTab_push(math, courses);
    CoursesTab_push(phys, courses);
    CoursesTab_push(prog, courses);

    StudentsTab *students = StudentsTab_init();
    Student *alice = init_student("Smith", "Alice", 1001);
    Student *bob = init_student("Jones", "Bob", 1002);
    StudentsTab_push(alice, students);
    StudentsTab_push(bob, students);

    // Allocate followed courses for each student
    allocate_students_courses(students, courses->size);

    // Add some grades
    add_grade_to_student(alice, courses, "Mathematics", 18.5f);
    add_grade_to_student(alice, courses, "Physics", 16.0f);
    add_grade_to_student(alice, courses, "Programming", 19.0f);
    
    add_grade_to_student(bob, courses, "Mathematics", 15.0f);
    add_grade_to_student(bob, courses, "Physics", 17.5f);
    add_grade_to_student(bob, courses, "Programming", 16.5f);

    // Update averages
    for (int i = 0; i < students->size; i++) {
        Student *s = students->tab[i];
        for (int j = 0; j < s->n_courses; j++) {
            s->f_courses[j]->average = get_followed_course_avg(s->f_courses[j]);
        }
        s->average = get_student_general_avg(s, courses);
    }

    // Create promotion
    Promotion *orig = init_promotion(courses, students);
    printf("Original promotion:\n");
    print_promotion(orig);

    // Save to file
    const char *filename = "test_promotion.bin";
    if (save_promotion(orig, filename) != 0) {
        printf("Error saving promotion\n");
        free_promotion(orig);
        return -1;
    }
    printf("\nSaved promotion to %s\n", filename);

    // Load from file
    Promotion *loaded = load_promotion(filename);
    if (!loaded) {
        printf("Error loading promotion\n");
        free_promotion(orig);
        return -1;
    }
    printf("\nLoaded promotion:\n");
    print_promotion(loaded);

    // Clean up
    free_promotion(orig);
    free_promotion(loaded);
    printf("\nTest completed successfully!\n");
    return 0;
}

/* Binary file layout (all integers stored as 32-bit little-endian via native fwrite):
 * 4 bytes: magic "PROM"
 * int32: version (1)
 * int32: n_courses
 * for each course:
 *   int32 name_len (including terminating '\0')
 *   name_len bytes: name (with '\0')
 *   float coef
 * int32: n_students
 * for each student:
 *   uint32 id
 *   int32 name_len
 *   name_len bytes: name (with '\0')
 *   int32 fname_len
 *   fname_len bytes: first name (with '\0')
 *   float average
 *   // followed course data for each course in order
 *   for i in 0..n_courses-1:
 *     float fcourse_average
 *     int32 n_grades
 *     n_grades floats
 */

#define SAVE_MAGIC "PROM"
#define SAVE_VERSION 1

static int write_string(FILE *f, const char *s)
{
    int32_t len = (int32_t)(strlen(s) + 1);
    if (fwrite(&len, sizeof(len), 1, f) != 1)
        return -1;
    if (fwrite(s, 1, len, f) != (size_t)len)
        return -1;
    return 0;
}

static char *read_string(FILE *f)
{
    int32_t len;
    if (fread(&len, sizeof(len), 1, f) != 1)
        return NULL;
    if (len <= 0)
        return NULL;
    char *buf = malloc((size_t)len);
    if (!buf)
        return NULL;
    if (fread(buf, 1, len, f) != (size_t)len)
    {
        free(buf);
        return NULL;
    }
    return buf;
}

int save_promotion(struct promotion *prom, const char *filename)
{
    if (!prom || !filename)
        return -1;

    FILE *f = fopen(filename, "wb");
    if (!f)
        return -1;

    if (fwrite(SAVE_MAGIC, 1, 4, f) != 4)
        goto err;
    int32_t version = SAVE_VERSION;
    if (fwrite(&version, sizeof(version), 1, f) != 1)
        goto err;

    // courses
    int32_t n_courses = prom->courses ? prom->courses->size : 0;
    if (fwrite(&n_courses, sizeof(n_courses), 1, f) != 1)
        goto err;
    for (int i = 0; i < n_courses; i++)
    {
        Course *c = prom->courses->tab[i];
        if (write_string(f, c->name) != 0)
            goto err;
        if (fwrite(&c->coef, sizeof(c->coef), 1, f) != 1)
            goto err;
    }

    // students
    int32_t n_students = prom->stu_dtab ? prom->stu_dtab->size : 0;
    if (fwrite(&n_students, sizeof(n_students), 1, f) != 1)
        goto err;
    for (int i = 0; i < n_students; i++)
    {
        Student *s = prom->stu_dtab->tab[i];
        uint32_t id = s->id;
        if (fwrite(&id, sizeof(id), 1, f) != 1)
            goto err;
        if (write_string(f, s->name) != 0)
            goto err;
        if (write_string(f, s->fname) != 0)
            goto err;
        if (fwrite(&s->average, sizeof(s->average), 1, f) != 1)
            goto err;

        // followed courses
        int32_t n_courses_local = s->n_courses;
        // Expect n_courses_local == n_courses, but write based on student's value
        for (int j = 0; j < n_courses_local; j++)
        {
            Followed_course *fc = s->f_courses[j];
            float favg = fc->average;
            if (fwrite(&favg, sizeof(favg), 1, f) != 1)
                goto err;
            int32_t n_grades = fc->grades ? fc->grades->size : 0;
            if (fwrite(&n_grades, sizeof(n_grades), 1, f) != 1)
                goto err;
            for (int k = 0; k < n_grades; k++)
            {
                float grade = fc->grades->tab[k];
                if (fwrite(&grade, sizeof(grade), 1, f) != 1)
                    goto err;
            }
        }
    }

    fclose(f);
    return 0;

err:
    fclose(f);
    return -1;
}

struct promotion *load_promotion(const char *filename)
{
    if (!filename) {
        fprintf(stderr, "No filename provided\n");
        return NULL;
    }
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    char magic[4];
    if (fread(magic, 1, 4, f) != 4) {
        fprintf(stderr, "Could not read magic number\n");
        goto fail;
    }
    if (memcmp(magic, SAVE_MAGIC, 4) != 0) {
        fprintf(stderr, "Invalid magic number\n");
        goto fail;
    int32_t version;
    if (fread(&version, sizeof(version), 1, f) != 1)
        goto fail;
    if (version != SAVE_VERSION)
        goto fail;

    int32_t n_courses;
    if (fread(&n_courses, sizeof(n_courses), 1, f) != 1)
        goto fail;
    CoursesTab *ctab = CoursesTab_init();
    for (int i = 0; i < n_courses; i++)
    {
        char *name = read_string(f);
        if (!name)
            goto fail_ctab;
        float coef;
        if (fread(&coef, sizeof(coef), 1, f) != 1)
        {
            free(name);
            goto fail_ctab;
        }
        Course *c = init_course(coef, name);
        free(name);
        CoursesTab_push(c, ctab);
    }

    int32_t n_students;
    if (fread(&n_students, sizeof(n_students), 1, f) != 1)
        goto fail_ctab;
    StudentsTab *stab = StudentsTab_init();

    // first pass: read and create students basic info
    for (int i = 0; i < n_students; i++)
    {
        uint32_t id;
        if (fread(&id, sizeof(id), 1, f) != 1)
            goto fail_stab;
        char *name = read_string(f);
        if (!name)
            goto fail_stab;
        char *fname = read_string(f);
        if (!fname)
        {
            free(name);
            goto fail_stab;
        }
        float average;
        if (fread(&average, sizeof(average), 1, f) != 1)
        {
            free(name);
            free(fname);
            goto fail_stab;
        }

        Student *s = init_student(name, fname, id);
        free(name);
        free(fname);
        s->average = average;
        // postpone followed courses allocation until we have all students
        StudentsTab_push(s, stab);

        // skip ahead over the followed courses data for now by reading and storing in-memory
        // We'll re-read these after allocation by rewinding to the current position is complicated.
        // Instead, we read and buffer the followed course raw data in memory.
    }

    // After creating students, allocate f_courses for each student
    allocate_students_courses(stab, n_courses);

    // Now fill the followed course data: iterate students and read their per-course data
    for (int i = 0; i < n_students; i++)
    {
        Student *s = stab->tab[i];
        for (int j = 0; j < n_courses; j++)
        {
            float favg;
            if (fread(&favg, sizeof(favg), 1, f) != 1)
                goto fail_fill;
            int32_t n_grades;
            if (fread(&n_grades, sizeof(n_grades), 1, f) != 1)
                goto fail_fill;
            Followed_course *fc = s->f_courses[j];
            fc->average = favg;
            for (int k = 0; k < n_grades; k++)
            {
                float grade;
                if (fread(&grade, sizeof(grade), 1, f) != 1)
                    goto fail_fill;
                Grades_push(grade, fc->grades);
            }
        }
    }

    // Build promotion and return
    Promotion *prom = init_promotion(ctab, stab);
    fclose(f);
    return prom;

fail_fill:
    // cleanup students' partially built grades
    for (int i = 0; i < stab->size; i++)
    {
        Student *s = stab->tab[i];
        for (int j = 0; j < s->n_courses; j++)
        {
            if (s->f_courses && s->f_courses[j])
            {
                Grades_free(s->f_courses[j]->grades, NULL);
                s->f_courses[j]->grades = NULL;
            }
        }
    }
fail_stab:
    StudentsTab_free(stab, free_student);
fail_ctab:
    CoursesTab_free(ctab, free_course);
fail:
    fclose(f);
    return NULL;
}
