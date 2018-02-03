//c++
#include <string> //std::string
#include <stdexcept> //std::runtime_error
//c
#include <stdint.h> //uint16_t
#include <jpeglib.h> //jpeg decompression, to install run sudo apt-get install libjpeg-dev
#include <sys/types.h> //low level io
#include <sys/stat.h> //low level io
#include <fcntl.h> //low level io
#include <unistd.h> //low level io
#include <sys/mman.h> //mmap(), munmap()
#include <errno.h> //errno
#include <string.h> //strerror
#include <stddef.h> //NULL
//h
#include "bitmap.h"

/*
 * function for the error handling for the libjpeg library
 * see: https://stackoverflow.com/questions/19857766/error-handling-in-libjpeg
 */
static void jpeg_error_exit(j_common_ptr cinfo) {
    char jpeg_last_error_msg[JMSG_LENGTH_MAX];
    (*( cinfo->err->format_message )) (cinfo, jpeg_last_error_msg);
    throw std::runtime_error(jpeg_last_error_msg);
}

Bitmap::Bitmap(uint16_t width, uint16_t height, uint16_t *buffer) {
    this->width = width;
    this->height = height;
    this->buffer = buffer;
}

Bitmap::Bitmap(std::string jpeg) {
    //open the file descriptor to the jpeg file
    int fd = open(jpeg.c_str(), O_RDONLY);
    if(fd == -1)
        throw std::runtime_error("Bitmap constructor, open jpeg file: " + std::string(strerror(errno)));
    //get size of file in bytes
    struct stat st;
    if(fstat(fd, &st) == -1)
        throw std::runtime_error("Bitmap constructor, get jpeg file size: " + std::string(strerror(errno)));
    //map the file into the memory
    unsigned char *mapping = (unsigned char *) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mapping == MAP_FAILED)
        throw std::runtime_error("Bitmap constructor, mapping of jpeg file: " + std::string(strerror(errno)));

    //structure for the decompression of the mjpeg picture
    struct jpeg_decompress_struct decompress_info;
    //structure for the handling of errors of the decompression
    struct jpeg_error_mgr jerr;
    //throw exception instead of exiting programm on failure
    jerr.error_exit = jpeg_error_exit;
    //create decompression object
    decompress_info.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&decompress_info);

    //set the source of the jpeg to decompress
    jpeg_mem_src(&decompress_info, mapping, st.st_size);
    //get the informations about the jpeg file by reading the header
    int ret = jpeg_read_header(&decompress_info, TRUE);
    if(ret != 1)
        throw std::runtime_error("Bitmap constructor, data of the file is not a jpeg");
    //get the basic properties of the image and allocate a first buffer
    width = decompress_info.output_width;
    height = decompress_info.output_height;
    int bytes_per_pixel = decompress_info.output_components;
    if(bytes_per_pixel != 3)
        throw std::runtime_error("Bitmap constructor, jpeg is not in 24 bit format and therefor not supported");
    unsigned char *rgb24_buffer = new unsigned char[width * height * bytes_per_pixel];

    //make the decompression
    jpeg_start_decompress(&decompress_info);
    int row_stride = decompress_info.output_width * decompress_info.output_components;
    unsigned char *buffer_array[1];
    while(decompress_info.output_scanline < decompress_info.output_height) {
        buffer_array[0] = rgb24_buffer + decompress_info.output_scanline * row_stride;
        jpeg_read_scanlines(&decompress_info, buffer_array, 1);
    }
    jpeg_finish_decompress(&decompress_info);

    //destroy decompression object
    jpeg_destroy_decompress(&decompress_info);
    //unmap the file
    if(munmap(mapping, st.st_size) == -1) {
        delete[] rgb24_buffer;
        throw std::runtime_error("Bitmap constructor, unmap jpeg file: " + std::string(strerror(errno)));
    }
    //close the file
    if(close(fd) == -1) {
        delete[] rgb24_buffer;
        throw std::runtime_error("Bitmap constructor, close jpeg file: " + std::string(strerror(errno)));
    }

    //scale image down to 565 RGB (16 bit)
    buffer = new uint16_t[width * height];
    for(int i = 0; i < width * height; i++) {
        buffer[i] =
            (rgb24_buffer[3 * i] >> 3) << 11 |
            (rgb24_buffer[3 * i + 1] >> 2) << 5 |
            (rgb24_buffer[3 * i + 2] >> 3);
    }
    delete[] rgb24_buffer;
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