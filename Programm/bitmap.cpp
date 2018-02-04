//c++
#include <string> //std::string
#include <stdexcept> //std::runtime_error
//c
#include <stdint.h> //uint16_t
#include <jpeglib.h> //jpeg decompression, to install run sudo apt-get install libjpeg-dev
#include <stdio.h> //FILE, fopen(), fclose()
#include <errno.h> //errno
#include <string.h> //strerror()
//h
#include "bitmap.h"

/* 
 * intermediate buffer to hold parts of the decompressed jpeg
 * required to be declared gobally to be released in case of an error during the decompression
 */
static unsigned char *rgb24_buffer = NULL;

/*
 * function for the error handling for the libjpeg library
 * see: https://stackoverflow.com/questions/19857766/error-handling-in-libjpeg
 */
static void jpeg_error_exit(j_common_ptr cinfo) {
    //release the resources
    if(rgb24_buffer != NULL)
        delete[] rgb24_buffer;
    //throw an exception
    char jpeg_last_error_msg[JMSG_LENGTH_MAX];
    (*( cinfo->err->format_message )) (cinfo, jpeg_last_error_msg);
    throw std::runtime_error(jpeg_last_error_msg);
}

Bitmap::Bitmap(void) {
    width = 0;
    height = 0;
    buffer = NULL;
}

Bitmap::Bitmap(uint16_t width, uint16_t height, uint16_t *buffer) {
    this->width = width;
    this->height = height;
    this->buffer = buffer;
}

//implementation based on https://gist.github.com/PhirePhly/3080633
Bitmap::Bitmap(std::string jpegFile) {
    //open the file containing the jpeg
    FILE *file = fopen(jpegFile.c_str(), "r");
    if(file == NULL)
        throw std::runtime_error("Bitmap constructor, open jpeg file: " + std::string(strerror(errno)));

    //structure for the decompression of the mjpeg picture
    struct jpeg_decompress_struct decompress_info;
    //structure for the handling of errors of the decompression
    struct jpeg_error_mgr jerr;
    //create decompression object
    decompress_info.err = jpeg_std_error(&jerr);
    //throw exception instead of exiting programm on failure, can still be improved since resources are not released properly
    jerr.error_exit = jpeg_error_exit;
    //create decompression object
    jpeg_create_decompress(&decompress_info);
    //set the source of the jpeg to decompress
    jpeg_stdio_src(&decompress_info, file);
    //get the informations about the jpeg file by reading the header
    int ret = jpeg_read_header(&decompress_info, TRUE);
    if(ret != 1)
        throw std::runtime_error("Bitmap constructor, data of the file is not a jpeg");

    //start decompressor
    jpeg_start_decompress(&decompress_info);

    //get the basic properties of the image and allocate a buffer
    width = decompress_info.output_width;
    height = decompress_info.output_height;
    if(decompress_info.output_components != 3) //most pictures are in this format so this shouldn't be a problem
        throw std::runtime_error("Bitmap constructor, jpeg is not in 24 bit format and therefor not supported");
    buffer = new uint16_t[width * height];

    //make the decompression
    int row_stride = decompress_info.output_width * 3;
    rgb24_buffer = new unsigned char[row_stride];
    unsigned char *buffer_array[1];
    buffer_array[0] = rgb24_buffer;
    while(decompress_info.output_scanline < decompress_info.output_height) {
        jpeg_read_scanlines(&decompress_info, buffer_array, 1);
        //scale image down to 565 RGB (16 bit)
        for(int i = 0; i < width; i++) {
            buffer[decompress_info.output_scanline * height + i] =
                (rgb24_buffer[3 * i] >> 3) << 11 |
                (rgb24_buffer[3 * i + 1] >> 2) << 5 |
                (rgb24_buffer[3 * i + 2] >> 3);
        }
    }
    delete[] rgb24_buffer;

    //finish decompressor
    jpeg_finish_decompress(&decompress_info);

    //destroy decompression object
    jpeg_destroy_decompress(&decompress_info);
    //close the file
    if(fclose(file) == EOF)
        throw std::runtime_error("Bitmap constructor, close jpeg file: " + std::string(strerror(errno)));
}

Bitmap::~Bitmap(void) {
    delete[] buffer;
}

uint16_t Bitmap::getWidth(void) {
    return width;
}

uint16_t Bitmap::getHeight(void) {
    return height;
}

uint16_t *Bitmap::getBuffer(void) {
    return buffer;
}