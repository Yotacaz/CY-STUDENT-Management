#include <stdlib.h>
#include <stdio.h>

#include "core/load_data.h"
#include "core/save_bin.h"
#include "core/load_bin.h"

int main()
{
    char file_path[] = "data/data.txt";
    Promotion *prom = load_data(file_path);

    // saving top scoring students
    Promotion *top_prom = init_promotion(prom->courses, NULL);
    top_prom->stu_dtab = get_top_students(prom->stu_dtab, 10);
    assert(top_prom->stu_dtab);
    StudentsTab_print(top_prom->stu_dtab, print_student);

    bin_save_promotion(top_prom, "data/top.bin");
    // only free the promotion structure and the data it own (CoursesTab and actual students data owned by prom)
    StudentsTab_free(top_prom->stu_dtab, NULL);
    free_promotion(top_prom, NULL, NULL);

    // Loading top scoring students
    printf("\nLoaded from top.bin : \n");
    Promotion *top_prom_loaded = bin_load_promotion("data/top.bin");
    StudentsTab_print(top_prom_loaded->stu_dtab, print_student);

    free_promotion(top_prom_loaded, free_student, free_course);
    free_promotion(prom, free_student, free_course);
    return 0;
}