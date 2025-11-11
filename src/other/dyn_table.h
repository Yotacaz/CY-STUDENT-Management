#ifndef TABLE_H
#define TABLE_H

/// @file dyn_table.h
/// @brief Macros to declare and define dynamic tables in C.
/// A dynamic table is a structure that can grow as needed when elements are added.
/// It contains a pointer to the elements, the current size, and the capacity.

#include <arpa/inet.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// DECLARE_DYN_TABLE should be put in the header file where we would want to declare
/// a dynamic table structure that reallocate itself as needed.
/// @param Type the type of the elements stored in the dynamic table
/// @param Name the name of the dynamic table structure
/// Generates the structure and the function prototypes.
/// Usage example:
/// DECLARE_DYN_TABLE(Student*, StudentsTab)
/// Functions generated:
/// - StudentsTab* StudentsTab_init();
/// - void StudentsTab_push(Student* value, StudentsTab* table) : adds an element to the dynamic
/// table
/// - void StudentsTab_free(StudentsTab* tab, void (*free_elem)(Student*)) :
/// frees the dynamic table and its elements using the provided free function\n
/// - void StudentsTab_sort(StudentsTab* tab, int (*compare_function)(const void*, const void*)) :
/// sorts the dynamic table using qsort and the provided comparison function\n
/// - void StudentsTab_print(StudentsTab* tab, void (*print_elem)(Student*)) :
/// prints the dynamic table using the provided print function\n
/// - void StudentsTab_save_to_bin(StudentsTab* tab, FILE* file, void (*save_elem)(Student*,
/// FILE*)): saves the dynamic table to a binary file using the provided save function\n
/// - StudentsTab* StudentsTab_load_from_bin(FILE* file, Student* (*load_elem)(FILE*)) :
/// loads the dynamic table from a binary file using the provided load function
#define DECLARE_DYN_TABLE(Type, Name)                                                              \
    typedef struct Name                                                                            \
    {                                                                                              \
        Type *tab;                                                                                 \
        int capacity;                                                                              \
        int size;                                                                                  \
    } Name;                                                                                        \
                                                                                                   \
    Name *Name##_init();                                                                           \
    void Name##_push(Type value, Name *table);                                                     \
    void Name##_free(Name *tab, void (*free_elem)(Type));                                          \
    void Name##_sort(Name *tab, int (*compare_function)(const void *, const void *));              \
    void Name##_print(Name *tab, void (*print_elem)(Type));                                        \
    void Name##_save_to_bin(Name *tab, FILE *file, void (*save_elem)(Type, FILE *));               \
    Name *Name##_load_from_bin(FILE *file, Type (*load_elem)(FILE *));                             \
    bool Name##_is_valid(Name *dtab, bool (*elem_is_valid)(Type));

/// DEFINE_DYN_TABLE should be put in the C file where we would want to define
/// the functions of a dynamic table structure that reallocate itself as needed.
/// @param Type the type of the elements stored in the dynamic table
/// @param Name the name of the dynamic table structure
#define DEFINE_DYN_TABLE(Type, Name)                                                               \
    Name *Name##_init()                                                                            \
    {                                                                                              \
        Name *td = malloc(sizeof(Name));                                                           \
        verify(td, "malloc error");                                                                \
        td->capacity = 0;                                                                          \
        td->size = 0;                                                                              \
        td->tab = NULL;                                                                            \
        return td;                                                                                 \
    }                                                                                              \
    void Name##_push(Type value, Name *table)                                                      \
    {                                                                                              \
        assert(table);                                                                             \
        if (table->size >= table->capacity)                                                        \
        {                                                                                          \
            table->capacity = table->capacity * 2 + 1;                                             \
            Type *new_tab = (Type *)realloc(table->tab, table->capacity * sizeof(Type));           \
            verify(new_tab != NULL, "realloc error");                                              \
            table->tab = new_tab;                                                                  \
        }                                                                                          \
        table->tab[table->size] = value;                                                           \
        table->size += 1;                                                                          \
    }                                                                                              \
                                                                                                   \
    void Name##_free(Name *tab, void (*free_elem)(Type))                                           \
    {                                                                                              \
        if (tab == NULL)                                                                           \
        {                                                                                          \
            fprintf(stderr, BOLD_RED "WARNING : dynamic table struct %s is NULL\n" RESET, #Name);  \
            return;                                                                                \
        }                                                                                          \
        if (free_elem != NULL)                                                                     \
        {                                                                                          \
            for (int i = 0; i < tab->size; i++)                                                    \
            {                                                                                      \
                free_elem(tab->tab[i]);                                                            \
            }                                                                                      \
        }                                                                                          \
        free(tab->tab);                                                                            \
        tab->tab = NULL;                                                                           \
        free(tab);                                                                                 \
    }                                                                                              \
                                                                                                   \
    void Name##_sort(Name *tab, int (*compare_function)(const void *, const void *))               \
    {                                                                                              \
        assert(tab);                                                                               \
        qsort(tab->tab, tab->size, sizeof(Type), compare_function);                                \
    }                                                                                              \
                                                                                                   \
    void Name##_print(Name *table, void (*print_elem)(Type))                                       \
    {                                                                                              \
        if (!table)                                                                                \
        {                                                                                          \
            printf("NULL %s table\n", #Name);                                                      \
            return;                                                                                \
        }                                                                                          \
        printf("%s <%s> [size=%d, capacity=%d] : \n", #Name, #Type, table->size, table->capacity); \
        for (int i = 0; i < table->size; i++)                                                      \
        {                                                                                          \
            printf(" [%d] : ", i);                                                                 \
            print_elem(table->tab[i]);                                                             \
        }                                                                                          \
        putchar('\n');                                                                             \
    }                                                                                              \
    void Name##_save_to_bin(Name *dtab, FILE *file, void (*save_elem)(Type, FILE *))               \
    {                                                                                              \
        assert(dtab && file);                                                                      \
        verify(fwrite(&(dtab->capacity), sizeof(int), 1, file) == 1,                               \
               "couldn't save dynamic table capacity (int) while saving to binary");               \
        verify(fwrite(&(dtab->size), sizeof(int), 1, file) == 1,                                   \
               "couldn't save dynamic table size (int) while saving to binary");                   \
        if (save_elem == NULL)                                                                     \
        {                                                                                          \
            /*We assume that if save_elem is NULL, the table does not contain any pointer*/        \
            verify(fwrite(dtab->tab, sizeof(Type), dtab->size, file) == (size_t)dtab->size,        \
                   "couldn't save dynamic table content while saving to binary");                  \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            /*Otherwise, we need to do the save of the table manually*/                            \
            for (int i = 0; i < dtab->size; i++)                                                   \
            {                                                                                      \
                save_elem(dtab->tab[i], file);                                                     \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
    Name *Name##_load_from_bin(FILE *file, Type (*load_elem)(FILE *))                              \
    {                                                                                              \
        assert(file);                                                                              \
        Name *dtab = Name##_init();                                                                \
        assert(dtab);                                                                              \
        verify(fread(&(dtab->capacity), sizeof(int), 1, file) == 1,                                \
               "couldn't load dynamic table capacity (int) while loading from binary");            \
        verify(fread(&(dtab->size), sizeof(int), 1, file) == 1,                                    \
               "couldn't load dynamic table size (int) while loading from binary");                \
                                                                                                   \
        dtab->tab = (Type *)malloc(dtab->capacity * sizeof(Type));                                 \
        assert(dtab->tab);                                                                         \
        if (load_elem == NULL)                                                                     \
        {                                                                                          \
            /*We assume that if load_elem is NULL, the table does not contain any pointer*/        \
            verify(fread(dtab->tab, sizeof(Type), dtab->size, file) == (size_t)dtab->size,         \
                   "couldn't load dynamic table content while loading from binary");               \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            /*Otherwise, we need to do the load of the table manually*/                            \
            for (int i = 0; i < dtab->size; i++)                                                   \
            {                                                                                      \
                dtab->tab[i] = load_elem(file);                                                    \
            }                                                                                      \
        }                                                                                          \
        return dtab;                                                                               \
    }                                                                                              \
    bool Name##_is_valid(Name *dtab, bool (*elem_is_valid)(Type))                                  \
    {                                                                                              \
        if (!dtab)                                                                                 \
        {                                                                                          \
            fprintf(stderr, BOLD_RED "WARNING : dtab of type <%s*> is NULL\n" RESET, #Name);       \
            return false;                                                                          \
        }                                                                                          \
        int size = dtab->size;                                                                     \
        int cap = dtab->capacity;                                                                  \
        Type *tab = dtab->tab;                                                                     \
        if (size < 0 || cap < size)                                                                \
        {                                                                                          \
            fprintf(stderr,                                                                        \
                    BOLD_RED "WARNING : dtab of type <%s*> has invalid size (%d) and/or "          \
                             "capacity(%d)\n" RESET,                                               \
                    #Name, size, cap);                                                             \
            return false;                                                                          \
        }                                                                                          \
        if (size > 0)                                                                              \
        {                                                                                          \
            if (!tab)                                                                              \
            {                                                                                      \
                fprintf(stderr,                                                                    \
                        BOLD_RED                                                                   \
                        "WARNING : dtab of type <%s*> has invalid table (of type %s*)\n" RESET,    \
                        #Name, #Type);                                                             \
                return false;                                                                      \
            }                                                                                      \
            if (elem_is_valid)                                                                     \
            {                                                                                      \
                for (int i = 0; i < size; i++)                                                     \
                {                                                                                  \
                    if (!elem_is_valid(tab[i]))                                                    \
                    {                                                                              \
                        fprintf(stderr,                                                            \
                                BOLD_RED "WARNING : table entry %d (type %s*) of dtab (type %s*) " \
                                         "is invalid\n" RESET,                                     \
                                i, #Type, #Name);                                                  \
                        return false;                                                              \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        else if (tab) /*and size ==0*/                                                             \
        {                                                                                          \
            fprintf(stderr,                                                                        \
                    BOLD_RED "WARNING : table (type %s*) of dtab (type %s*) is supposed null if "  \
                             "table size is 0\n" RESET,                                            \
                    #Type, #Name);                                                                 \
            return false;                                                                          \
        }                                                                                          \
        return true;                                                                               \
    }

#endif