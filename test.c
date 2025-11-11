#include <stdio.h>
#include <stdlib.h>

#include "lib/student_api.h"

int main()
{
    char file_path[] = "data/data.txt";
    CLASS_DATA *prom = API_load_students(file_path);
    
    char **best_stu = API_get_best_students(prom);
    for (int i = 0; i < SIZE_TOP1; i++)
    {
        printf("%d : %s\n", i + 1, best_stu[i]);
        free(best_stu[i]);
    }
    free(best_stu);
    API_unload(prom);
    return 0;
}
