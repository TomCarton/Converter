// main.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../libraries/image/image.h"


#define UPPERCASE(C) (((C) >= 'a' && (C) <= 'z') ? ((C) - ' ') : (C))

static int stricmp(const char *s1, const char *s2)
{
    while (*s2 != 0 && UPPERCASE(*s1) == UPPERCASE(*s2)) s1++, s2++;
    return (int)(UPPERCASE(*s1) - UPPERCASE(*s2));
}

// pathLastComponentName: return the last component of a given path
static char *pathLastComponentName(const char *path)
{
    char *lname = strrchr(path, '/');
    lname = lname == NULL ? (char *)path : lname + 1;

    char *name = malloc(strlen(lname) + 1);
    strcpy(name, lname);

    char *rname = strrchr(name, '.');
    if (rname)
    {
        *rname = '\0';
    }

    return name;
}

// pathExtension: return the extension of the file at a given path
static char *pathExtension(const char *path)
{
    char *ext = strrchr(path, '.');
    if (ext)
    {
        char *extension = malloc(strlen(ext) + 1);
        strcpy(extension, ext + 1);

        return extension;
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    char infile[PATH_MAX] = "";
    char outfile[PATH_MAX] = "";


    if (argc < 2)
    {
        goto usage;
    }

    // read parameters
    for (unsigned int i = 1; i < argc; ++i)
    {
        // output
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (argc > i + 1)
            {
                strcpy(outfile, argv[i + 1]);

                memmove(argv + i + 1, argv + i + 2, (argc - i - 2) * sizeof *argv);
                --argc;
                continue;
            }
        }
        // help
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            goto usage;
        }
        // input
        else if (strlen(infile) < 1)
        {
            strcpy(infile, argv[i]);
        }
        else
        {
            fprintf(stderr, "unknown parameter '%s'\n", argv[i]);
            return 1;
        }
    }

    if (strlen(infile) < 1)
    {
        fprintf(stderr, "no input file provided\n");
        return 1;
    }


    Image *image = NULL;
    Error err = kErrorNone;

    char *ext = strrchr(infile, '.');
    if (ext)
    {
        if (stricmp(ext, ".pi1") == 0 || stricmp(ext, ".pc1") == 0)
        {
            err = ImageLoadDegas(infile, &image);
        }
    }

    if (image)
    {
        printf("- %d x %d image\n", image->width, image->height);
    }
    else if (err != kErrorNone)
    {
        fprintf(stderr, "error reading input file '%s' (0x%04X)\n", infile, err);
        return 1;
    }
    else
    {
        fprintf(stderr, "input file '%s' has not been recognized\n", infile);
        return 1;
    }


    if (strlen(outfile) == 0)
    {
        strcpy(outfile, infile);
        ext = strrchr(outfile, '.');
        if (ext == NULL)
        {
            ext = outfile + strlen(outfile);
        }

        strcpy(ext, ".png");
    }

    printf("output to '%s'\n", outfile);

    ext = strrchr(outfile, '.');
    if (ext)
    {
        if (stricmp(ext, ".tga") == 0)
        {
            ImageSaveTGA(outfile, image);
        }
        else if (stricmp(ext, ".png") == 0)
        {
            ImageSavePNG(outfile, image);
        }
    }

    return 0;

usage:

    fprintf(stderr, "\n");
    char *command = pathLastComponentName(argv[0]);

    fprintf(stderr, "   usage:\n");
    fprintf(stderr, "     %s path [-h/--help] | [-o/--output output]\n\n", command);
    fprintf(stderr, "   parameters:\n");
    fprintf(stderr, "     -h, --help     - display this\n");
    fprintf(stderr, "     -o, --output   - output file [default: \"[input].png\"]\n\n");

    fprintf(stderr, "   example:\n");
    fprintf(stderr, "     ./%s image.pi1 -o image.tga\n", command);

    fprintf(stderr, "\n");

    free(command);

    return 0;
}
