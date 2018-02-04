//c++
#include <string> //std::string
#include <stdexcept> //std::runtime_error
//c
#include <stdint.h> //uint16_t
#include <stdlib.h> //malloc(), free()
#include <stdio.h> //FILE
#include <errno.h> //errno
#include <string.h> //strerror()
#include <png.h> //png decompression
//h
#include "alpha_bitmap.h"

AlphaBitmap::AlphaBitmap(uint16_t width, uint16_t height, uint16_t *buffer, bool *apha) : Bitmap(width, height, buffer) {
    this->alpha = alpha;
}

//implementation based on https://gist.github.com/niw/5963798
AlphaBitmap::AlphaBitmap(std::string pngFile) {
    FILE *file = fopen(pngFile.c_str(), "r");
    if(file == NULL)
        throw std::runtime_error("AlphaBitmap constructor, open png file: " + std::string(strerror(errno)));

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png == NULL)
        throw std::runtime_error("AlphaBitmap constructor, png create read struct failure");
    
    png_infop info = png_create_info_struct(png);
    if(info == NULL)
        throw std::runtime_error("AlphaBitmap constructor, png create info struct failure");
    
    if(setjmp(png_jmpbuf(png)))
        throw std::runtime_error("AlphaBitmap constructor, png read failure");

    png_init_io(png, file);

    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    //allocate temporary buffer
    png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    png_destroy_read_struct(&png, &info, NULL);

    if(fclose(file) == EOF)
        throw std::runtime_error("AlphaBitmap constructor, close png file: " + std::string(strerror(errno)));

    //allocate the definitive buffers
    buffer = new uint16_t[width * height];
    alpha = new bool[width * height];

    //compress picture to 565 RGB (16 bit) and boolean alpha
    //free the allocated temporary buffer
    for(int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for(int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            buffer[y * height + x] =
                (px[0] >> 3) << 11 |
                (px[1] >> 2) << 5 |
                (px[2] >> 3);
            alpha[y * height + x] = px[3];
        }
        free(row);
    }
    free(row_pointers);
}

AlphaBitmap::~AlphaBitmap(void) {
    delete[] alpha;
}

bool *AlphaBitmap::getAlpha(void) {
    return alpha;
}