#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"

int main() {

    PGMimg *image;
    // Initializing image
    image = malloc(sizeof(PGMimg));
    getPGMImage("../images/sample.pgm", image);

    printf(" width  = %d\n", image->width);
    printf(" height = %d\n", image->height);
    printf(" maxVal = %d\n", image->maxVal);


    printf(" a(1,1)  = %d\n", image->data[0][0]);
    printf(" a(2,3)  = %d\n", image->data[2][3]);
    printf(" a(3,5)  = %d\n", image->data[3][5]);

    savePGMImage("output1", image);

    return 0;
}
