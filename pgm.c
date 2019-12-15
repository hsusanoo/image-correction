//
// Created by haita on 12/14/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include "pgm.h"

/**
 *
 * @param filename relative path of the image file to read
 * @param image PGM struct variable where to store the read image
 * @return
 */
PGMimg getPGMImage(const char *filename, PGMimg *image) {

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

    return *image;
}

/**
 * Save PGM Image data to PGM P2 file
 * @param filename Name where to save
 * @param img PGM Image to save
 * @return
 */
int savePGMImage(const char *filename, PGMimg *image) {
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

    printf("Saving image Done ! check it in the images folder!");

    return 1;
};