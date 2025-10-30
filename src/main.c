#include <stdlib.h>
#include <stdio.h>

#include "core/load_data.h"

int main()
{
    char file_path[] = "data/data.txt";
    Promotion *prom = load_data(file_path);
    StudentsTab* top = get_top_students(prom->stu_dtab, 10);
    StudentsTab_print(top, print_student);
    // print_promotion(prom);
    StudentsTab_free(top, NULL);    //DO NOT FREE THE STUDENT, JUST THE TAB (stu owned by prom->stu_dtab not top)
    free_promotion(prom);
    return 0;
}