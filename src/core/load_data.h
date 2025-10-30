#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include "../models/promotion.h"


#define BUF_LEN 4 * 1024

/// @brief Structure representing a section in the data file
typedef struct sect
{
    const char *section_title;
    const char *section_header;
} Section;

#define SECTION_CONTENT                     \
    {"ETUDIANTS", "numero;prenom;nom;age"}, \
        {"MATIERES", "nom;coef"},           \
        {"NOTES", "id;nom;note"}

/// @brief Load data from a file and create a Promotion structure
/// @param file_path the path to the data file
/// @return the loaded Promotion
Promotion* load_data(char *file_path);

/// @brief Load the students data from a file
/// @param file the file to read from
/// @return the loaded StudentsTab
StudentsTab *load_student_tab_data(FILE *file);

/// @brief Load the courses data from a file
/// @param file the file to read from
/// @return the loaded CoursesTab
CoursesTab *load_courses_data(FILE *file);

/// @brief Load the grades data from a file and update the students' followed courses
/// @param prom the students table
/// @param courses the courses table
/// @param file the file to read from
void load_grades_data(StudentsTab *prom, CoursesTab *courses, FILE *file);

/// @brief Set the file cursor to the beginning of the next section
/// @param section the section to find
/// @param file the file to read from
void set_cursor_to_next_section(const Section section, FILE *file);


#endif