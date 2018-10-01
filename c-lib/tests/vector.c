/*

Tycat a dynamic vector.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctycat.h>


// An integer vector
struct vector {
    size_t len;
    size_t allocated;
    int *array;
};



// Create a new vector
struct vector *vector_create()
{
    struct vector *vector = malloc(sizeof(struct vector));

    vector->len = 0;
    vector->allocated = 5;
    vector->array = malloc(sizeof(int) * vector->allocated);

    return vector;
}


// Delete a vector
void vector_free(struct vector *vector)
{
    free(vector->array);
    free(vector);
}


// Append a new integer into the vector
void vector_append(struct vector *vector, int number)
{
    vector->len++;

    if (vector->len >= vector->allocated) {
        vector->allocated += 3;
        vector->array = realloc(vector->array, sizeof(int) * vector->allocated);
    }

    vector->array[vector->len - 1] = number;
}



int main()
{
    struct vector *vector = vector_create();

    vector_append(vector, 42);
    vector_append(vector, 13);
    vector_append(vector, 8756);
    vector_append(vector, 1337);
    vector_append(vector, 28);

    ctycat(vector);

    vector_append(vector, 12);
    vector_append(vector, 13);
    vector_append(vector, 12364);
    vector_append(vector, 897465);

    ctycat(vector);

    vector_free(vector);
}
