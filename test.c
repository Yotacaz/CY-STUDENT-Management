#include <stdio.h>
#include <stdlib.h>

#include "lib/student_api.h"

void testeee(){
    printf("test\n");
}

int main()
{
    char file_path[] = "data/data.txt";
    CLASS_DATA *prom = API_load_students(file_path);
    char **best_stu = API_get_best_students(prom);
    for (int i = 0; i < SIZE_TOP1; i++)
    {
        // printf("%d : %s\n", i + 1, best_stu[i]);
        free(best_stu[i]);
    }
    free(best_stu);
    API_save_to_binary_file(prom, "data/data.bin");
    CLASS_DATA *restored_prom = API_restore_from_binary_file("data/data.bin");
    // printf("\nRestored promotion:\n");
    // API_display(restored_prom);
    API_unload(restored_prom);
    API_unload(prom);
    return 0;
}
