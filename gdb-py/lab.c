/*

Tycat a basic types.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <stdlib.h>


#define debugger asm volatile ("int3;")


struct Container {
    const char *string;
    char c;
    int integer;
    char **array;
    char fixed_str[20];
};



// A simple linked cell
struct cell {
    int age;
    struct cell *next;
};


// The head of the list
struct list {
    struct cell *top;
};


// Append a new cell into the list
void list_append(struct list *list, int age)
{
    struct cell *cell = malloc(sizeof(struct cell));
    struct cell *it = list->top;

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
    struct Container c = {
        .string = "Hello, my name is Guillaume.",
        .integer = 42,
        .fixed_str = "A short text"
    };

    struct Container *ptr = &c;

    int myint = 1337;


    struct list list;

    list_append(&list, 21);
    list_append(&list, 86);
    list_append(&list, 27);
    list_append(&list, 1337);


    // Break here
    debugger;
}
