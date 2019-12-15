#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"

int main() {

    PGMimg *image;
    // Initializing image
    image = malloc(sizeof(PGMimg));
    getPGMImage("../images/sample.pgm", image);

    processImage(image, SHARPEN);
    processImage(image, EDGE);
    processImage(image, BOX_BLUR);
    processImage(image, CUSTOM);

    return 0;
}
