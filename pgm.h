//
// Created by haita on 12/14/2019.
//

#ifndef IMAGE_CORRECTION_PGM_H
#define IMAGE_CORRECTION_PGM_H
#define MAX 300

typedef struct {
    int maxVal;
    int width;
    int height;
    int data[MAX][MAX];
} PGMimg;

// Prototypes

PGMimg getPGMImage(const char *filename, PGMimg *image);

int savePGMImage(const char* filename,PGMimg *img);

#endif //IMAGE_CORRECTION_PGM_H
