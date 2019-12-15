//
// Created by haita on 12/14/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include "pgm.h"

/**
 * Read image from file and store it in second parameter
 * @param filename relative path of the image file to read
 * @param image PGM struct variable where to store the read image
 * @return
 */
void getPGMImage(const char *filename, PGMimg *image) {

    FILE *in_file;
    char ch;                    // reading a single character
    int ch_int;                 // reading an int data
    int type;

    // Attempting open file
    in_file = fopen(filename, "r");
    if (in_file == NULL) {
        fprintf(stderr, "\nError: Unable to open file %s\n\n", filename);
        exit(EXIT_FAILURE);
    }

    // Check image type
    ch = (char) getc(in_file);
    if (ch != 'P') {
        fprintf(stderr, "\nError(1): Not a valid pgm file \n\n");
        exit(EXIT_SUCCESS);
    }

    ch = (char) getc(in_file);
    // convert character to digit (48 == 0)
    type = ch - 48;
    // Check supported format
    if (type != 2) {
        fprintf(stderr,
                "\nError(2): Unsupported format, please chose P2 format only (B/W uncompressed grayscale ASCII).",
                filename);
        exit(EXIT_SUCCESS);
    }
    // Skip to end of current line
    while (getc(in_file) != '\n');

    // Skip comment lines if there is any
    while (getc(in_file) == '#') {
        // Skip till end
        while (getc(in_file) != '\n');
    }
    // Fixing offset when reading width and height line
    while (ch != '\n') {
        fseek(in_file, -2, SEEK_CUR);
        ch = getc(in_file);
    }

    // Height and width
    fscanf(in_file, "%d", &(image->width));
    fscanf(in_file, "%d", &(image->height));
    fscanf(in_file, "%d", &(image->maxVal));

    // Cheking image size
    if ((image->width > MAX) || (image->height > MAX)) {
        fprintf(stderr, "\nError: Image too big, max size is 300x300.");
        exit(EXIT_SUCCESS);
    }

    for (int row = 0; row < image->height; row++) {
        for (int col = 0; col < image->width; col++) {

            fscanf(in_file, "%d", &ch_int);
            image->data[row][col] = ch_int;

        }
    }

    fclose(in_file);
    //logging
    {
        printf("Done reading file.\n\n");
    }
}

/**
 * Save PGM Image data to PGM P2 file
 * @param filename Name where to save
 * @param img PGM Image to save
 * @return
 */
void savePGMImage(const char *filename, const PGMimg *image) {
    FILE *out_file;
    int width, height, data;
    char *filepath = malloc(sizeof(char) * 50);

    //Creating output file path
    mkdir("output");
    snprintf(filepath, sizeof(char) * 50, "output/%s.pgm", filename);

    out_file = fopen(filepath, "w");
    // Attempting creating file
    if (out_file == NULL) {
        fprintf(stderr, "\nError: Unable to create file %s\n\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(out_file, "P2\n");
    fprintf(out_file, "# This image is generated using C program\n");
    fprintf(out_file, "%d %d\n", image->width, image->height);
    fprintf(out_file, "%d\n", image->maxVal);

    // Printing to file
    for (int row = 0; row < image->height; row++) {
        for (int col = 0; col < image->width; col++) {
            fprintf(out_file, "%d ", image->data[row][col]);
        }
        fprintf(out_file, "\n");
    }

    printf("Saving image Done ! check it in the images folder!\n\n");
};

/**
 * Process convolution for input image using a kernel
 * @param image input image to calculate the convolution for
 * @param mode Filter mode, 0 for user custom
 */
void processImage(const PGMimg *image, const int mode) {

    char *filename = malloc(50 * sizeof(char));
    PGMimg *out_image = malloc(sizeof(PGMimg));
    Kernel *kernel = malloc(sizeof(Kernel));

    // checking process mode =====================================
    switch (mode) {
        case SHARPEN: { // sharpen filter
            printf("\nSharpening image ............................\n");
            // Assigning kernel values
            kernel->size = 3;
            kernel->data[0][0] = 0;
            kernel->data[0][1] = -1;
            kernel->data[0][2] = 0;
            kernel->data[1][0] = -1;
            kernel->data[1][1] = 5;
            kernel->data[1][2] = -1;
            kernel->data[2][0] = 0;
            kernel->data[2][1] = -1;
            kernel->data[2][2] = 0;
            break;
        }
        case EDGE: {
            printf("\nEdge detection ............................\n");
            // Assigning kernel values
            kernel->size = 3;
            kernel->data[0][0] = -1;
            kernel->data[0][1] = -1;
            kernel->data[0][2] = -1;
            kernel->data[1][0] = -1;
            kernel->data[1][1] = 8;
            kernel->data[1][2] = -1;
            kernel->data[2][0] = -1;
            kernel->data[2][1] = -1;
            kernel->data[2][2] = -1;
            break;
        }
        case BOX_BLUR: {
            printf("\nBox Blur ............................\n");
            // Assigning kernel values
            kernel->size = 3;
            for (int i = 0; i < kernel->size; i++) {
                for (int j = 0; j < kernel->size; j++) {
                    kernel->data[i][j] = 1 / 9;
                }
            }
            break;
        }
        default: { // user custom filter
            fillCustomKernel(kernel);
        }
    }

    // fixing output image size =====================================
    out_image->width = image->width - kernel->size + 1;
    out_image->height = image->height - kernel->size + 1;
    out_image->maxVal = image->maxVal;

    // Calculating convolution =====================================
    for (int out_row = 0; out_row < out_image->height; out_row++) {
        for (int out_col = 0; out_col < out_image->width; out_col++) {
            out_image->data[out_row][out_col] = 0;
            for (int i = 0; i < kernel->size; i++) {
                for (int j = 0; j < kernel->size; j++) {
                    out_image->data[out_row][out_col] +=
                            (int) (kernel->data[i][j] * (float) image->data[i + out_row][j + out_col]);
                }
            }
            // fixing values out of range
            if (out_image->data[out_row][out_col] > 255)
                out_image->data[out_row][out_col] = 255;
            else if (out_image->data[out_row][out_col] < 0)
                out_image->data[out_row][out_col] = 0;
        }
    }

    // Saving image =====================================
    setbuf(stdout, 0);                  // free buffer
    printf("Output file name : ");
    gets(filename);
    savePGMImage(filename, out_image);

}

/**
 * Creating a user defined kernel
 * @param k input kernel to fill
 */
void fillCustomKernel(Kernel *k) {

    size_t size = 0;
    do {
        printf("You can check custom kernels at : http://setosa.io/ev/image-kernels\n");
        printf("\nEnter kernel size (max = 5) : ");
        scanf("%d", &size);
    } while (size > MAX_KERNEL);
    k->size = size;
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("K(%d,%d) = ", i, j);
            scanf("%f", &(k->data[i][j]));
        }
    }

}