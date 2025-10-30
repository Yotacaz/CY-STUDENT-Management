#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/// DECLARE_DYN_TABLE should be put in the header file where we would want to declare
/// a dynamic table structure that reallocate itself as needed.
/// @param Type the type of the elements stored in the dynamic table
/// @param Name the name of the dynamic table structure
#define DECLARE_DYN_TABLE(Type, Name)                                                 \
    typedef struct Name                                                               \
    {                                                                                 \
        Type *tab;                                                                    \
        int capacity;                                                                 \
        int size;                                                                     \
    } Name;                                                                           \
    Name *Name##_init();                                                              \
    void Name##_push(Type value, Name *table);                                        \
    void Name##_free(Name *tab, void (*free_elem)(Type));                             \
    void Name##_sort(Name *tab, int (*compare_function)(const void *, const void *)); \
    void Name##_print(Name *tab, void (*print_elem)(Type));

/// DEFINE_DYN_TABLE should be put in the C file where we would want to define
/// the functions of a dynamic table structure that reallocate itself as needed.
/// @param Type the type of the elements stored in the dynamic table
/// @param Name the name of the dynamic table structure
#define DEFINE_DYN_TABLE(Type, Name)                                                               \
    Name *Name##_init()                                                                            \
    {                                                                                              \
        Name *td = malloc(sizeof(Name));                                                           \
        assert(td);                                                                                \
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
            assert(new_tab != NULL);                                                               \
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
    void Name##_print(Name *table,                                                                 \
                      void (*print_elem)(Type))                                                    \
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
            fflush(stdout);                                                                        \
            print_elem(table->tab[i]);                                                             \
        }                                                                                          \
        putchar('\n');                                                                             \
    }

#endif