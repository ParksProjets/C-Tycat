/*

Convert a DOT file into a PNG image.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/utility.h"
#include "private/config.h"
#include "private/ctycat.h"

#include <string.h>
#include <unistd.h>



// Get the PNG name from the temp dir.
static char *pngname_temp()
{
    static char pngname[sizeof("/tmp/ctycatXXXXXX.png")];

    strcpy(pngname, "/tmp/ctycatXXXXXX.png");
    close(mkstemps(pngname, 4));

    return pngname;
}



// Get the PNG name from the configuration.
static char *pngname_config()
{
    char *dot = strrchr(c.png_name, '.');
    if (dot && !strcmp(dot, ".png"))
        return c.png_name;

    static char pngname[255];
    sprintf(pngname, "%s%d.png", c.png_name, _ctycat_number);

    return pngname;
}



// Convert a DOT file into a PNG image.
void _ctycat_dot_to_png(const char *dotname)
{
    // Get the PNG file name.
    char *pngname;

    if (c.save_png)
        pngname = pngname_config();
    else
        pngname = pngname_temp();


    // Convert the DOT file.
    static char command[600];
    sprintf(command, "dot -Tpng %s -o %s", dotname, pngname);
    system(command);

    // Delete the DOT file.
    if (!c.save_dot)
        unlink(dotname);


    // Tycat the PNG file.
    sprintf(command, "tycat %s", pngname);
    if (c.save_png)
        printf("Saved ctycat result in %s\n", pngname);

    system(command);
}
