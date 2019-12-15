//
// Created by haita on 12/14/2019.
//

#ifndef IMAGE_CORRECTION_PGM_H
#define IMAGE_CORRECTION_PGM_H
#define MAX 300
#define MAX_KERNEL 5
#define CUSTOM 0
#define SHARPEN 1
#define EDGE 2
#define BOX_BLUR 3

typedef struct {
    unsigned int maxVal;
    unsigned int width;
    unsigned int height;
    int data[MAX][MAX];
} PGMimg;

typedef struct {
    size_t size;
    float data[MAX_KERNEL][MAX_KERNEL];
} Kernel;

// Prototypes =====================================

void getPGMImage(const char *, PGMimg *);

void savePGMImage(const char *, const PGMimg *);

void processImage(const PGMimg *image, int mode);

void fillCustomKernel(Kernel *k);

#endif //IMAGE_CORRECTION_PGM_H
