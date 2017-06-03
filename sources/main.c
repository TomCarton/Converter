// main.c

#include <stdlib.h>
#include <stdio.h>

#include "../libraries/image/image.h"


int main(int argc, char *argv[])
{

    Image *image = NULL;

    printf("%s load image \"%s\"\n", argv[0], argv[1]);

    Error err = ImageLoadDegas(argv[1], &image);

    printf("result = 0x%04X\n", err);

    if (err == kErrorNone)
    {
        printf("image = %d x %d\n", image->width, image->height);

        ImageSaveTGA("result.tga", image);
    }
    
    return 0;
}
