#include <stdlib.h>
#include <stdio.h>

#include "../lib/student_api.h"

int main()
{
    char file_path[] = "data/data.txt";
    Promotion *prom = API_load_students(file_path);
    assert(promotion_is_valid(prom));

    // saving top scoring students
    Promotion *top_prom = init_promotion(prom->courses, NULL);
    top_prom->stu_dtab = get_top_students(prom->stu_dtab, 10);
    assert(promotion_is_valid(top_prom));
    StudentsTab_print(top_prom->stu_dtab, print_student);
    API_save_to_binary_file(top_prom, "data/top.bin");
    
    // only free the promotion structure and the data it own (CoursesTab and actual students data owned by prom)
    StudentsTab_free(top_prom->stu_dtab, NULL);
    top_prom->stu_dtab = NULL;
    free_promotion(top_prom, NULL, NULL);
    top_prom = NULL;

    // Loading top scoring students from bin file
    Promotion *top_prom_loaded = API_restore_from_binary_file("data/top.bin");
    assert(promotion_is_valid(top_prom_loaded));
    printf("\nLoaded from top.bin : \n");
    StudentsTab_print(top_prom_loaded->stu_dtab, print_student);

    free_promotion(top_prom_loaded, free_student, free_course);
    free_promotion(prom, free_student, free_course);
    return 0;
}