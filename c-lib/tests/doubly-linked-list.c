/*

Tycat a doubly-linked list.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctycat.h>


// A doubly-linked cell
struct cell {
    const char *name;
    int age;
    struct cell *next;
    struct cell *prev;
};


// The head of the list
struct list {
    struct cell *top;
    struct cell *end;
};



// Append a new cell into the list
void list_append(struct list *list, const char *name, int age)
{
    struct cell *cell = malloc(sizeof(struct cell));
    struct cell *it = list->top;

    cell->name = name;
    cell->age = age;
    cell->next = cell->prev = NULL;

    if (it == NULL) {
        list->top = cell;
    }
    else {
        while (it->next) it = it->next;
        it->next = cell;
        cell->prev = it;
    }

    list->end = cell;    
}



int main()
{
    struct list list = {0};

    list_append(&list, "Guillaume", 21);
    list_append(&list, "Jack", 86);
    list_append(&list, "James", 16);

    ctycat(list);
}
