/*

Read a configuration file and use it.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <string.h>
#include <ctycat.h>


int main()
{
    // Read "c-modified.ini" file
    ctycat_read_config("tests/config/c-modified.ini");

    // Print the new values
    printf("The new values are:\n");
    printf("  enable-analyse = %d\n", ctycat_config_get("enable-analyse"));
    printf("  text-font      = \"%s\"\n", ctycat_config_getstr("text-font"));
    printf("  max-string     = %d\n\n", ctycat_config_get("max-string"));


    // Now read a config file with some errors.
    ctycat_read_config("tests/config/c-error.ini");
}

