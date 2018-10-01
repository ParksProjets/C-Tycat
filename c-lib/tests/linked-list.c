/*

Tycat a linked list.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctycat.h>


// A simple linked cell
struct cell {
    const char *name;
    int age;
    struct cell *next;
};


// The head of the list
struct list {
    struct cell *top;
};



// Append a new cell into the list
void list_append(struct list *list, const char *name, int age)
{
    struct cell *cell = malloc(sizeof(struct cell));
    struct cell *it = list->top;

    cell->name = name;
    cell->age = age;
    cell->next = NULL;

    if (it == NULL) {
        list->top = cell;
    }
    else {
        while (it->next) it = it->next;
        it->next = cell;
    }
}



int main()
{
    struct list list = {0};

    list_append(&list, "Guillaume", 21);
    list_append(&list, "Jack", 86);
    list_append(&list, "James", 16);

    ctycat_config_setstr("rankdir", "TB");
    ctycat_config_setstr("text-font", "Calibri");
    ctycat(list);
}
