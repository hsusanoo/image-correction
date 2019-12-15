# Image correction

A program that applies custom [kernels](http://setosa.io/ev/image-kernels/) to a [PGM](http://netpbm.sourceforge.net/doc/pgm.html) image as an input file for correction or applying filters.
The output image will be in the same format.
Only [P2](https://en.wikipedia.org/wiki/Netpbm_format#File_format_description) format is supported.

### Build

This project uses [CMake](https://cmake.org/) build tool. See [how to build using CMake](https://cmake.org/runningcmake/).

### Usage

- Place the file to process under `images/`, name it `sample.pgm` (or change the default name and/or path in `main.c`)
- The output images will be placed under `[your build folder]/output/`, you can change it in `savePGMImage` under `pgm.c`.
