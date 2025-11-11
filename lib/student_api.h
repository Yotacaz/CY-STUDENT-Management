#ifndef STUDENT_API_H
#define STUDENT_API_H

/// @file student_api.h
/// @brief API functions to interact with the student management system


/// @brief Alias for Promotion structure used in the API
typedef void CLASS_DATA;

#ifndef SIZE_TOP1
/// @brief Number of best students (with highest general average) to retrieve
#define SIZE_TOP1 10
#endif
#ifndef SIZE_TOP2
/// @brief Number of best students (with highest average) to retrieve for a selected course
#define SIZE_TOP2 3
#endif

/// @brief Load data from a formatted **text file** and create a Promotion structure
/// Load order : courses, students, grades
/// @param file_path the path to the data file
/// @return the loaded Promotion
CLASS_DATA *API_load_students(char *file_path);

/// @brief Saves a promotion to a binary file. Order: courses, students
/// @param pClass the promotion to save
/// @param file_path the path to the binary file
/// @return always return 1
int API_save_to_binary_file(CLASS_DATA *pClass, char *file_path);

/// @brief Loads a promotion from a binary file. Order: courses, students
/// @param file_path the path to the binary file
/// @return the loaded Promotion
CLASS_DATA *API_restore_from_binary_file(char *file_path);

/// @brief Print a promotion (courses and students)
/// @param prom the promotion to print
void API_display(CLASS_DATA *prom);

/// @brief free a promotion
/// @param pClass promotion to free. \n
/// Warning, all item contained inside pClass must be owned by it
void API_unload(CLASS_DATA *pClass);

/// @brief Get the best students from a promotion
/// @param pClass the promotion
/// @return a dynamic table containing the names of the SIZE_TOP1 (default 10) best students
char **API_get_best_students(CLASS_DATA *pClass);

/// @brief Get the best students from a promotion for a given course
/// @param pClass the promotion
/// @param course the course to rank students in
/// @return a dynamic table containing the names of the SIZE_TOP2 (default 3) best students in
/// course, NULL if the course isn't found.
char **API_get_best_students_in_course(CLASS_DATA *pClass, char *course);

#endif