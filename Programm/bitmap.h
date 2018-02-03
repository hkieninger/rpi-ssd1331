#ifndef BITMAP_H_
#define BITMAP_H_ 1

//c++
#include <string>
//c
#include <stdint.h>

/*
 * class to manage Bitmaps in 565 RGB (16 bit)
 */
class Bitmap {
    uint16_t width, height;
    uint16_t *buffer;
    public:
        /*
         * constructor for bitmap from buffer
         * @width: the width of the bitmap
         * @height: the height of the bitmap
         * @buffer: the buffer containing the 565 RGB (16 bit) pixel data
         *          it must be allocated with new before, since it will be deleted by the destructor
         */
        Bitmap(uint16_t width, uint16_t height, uint16_t *buffer);

        /*
         * constructor for bitmap from jpeg file
         * the jpeg must be in RGB 24 bit format
         * @throw: std::runtime_error
         * @jpeg: the path to the jpeg file
         */
        Bitmap(std::string jpeg);

        /*
         * destructor, deletes the buffer from the heap
         */
        ~Bitmap(void);

        /*
         * @return: the width of the bitmap
         */
        uint16_t getWidth(void);

        /*
         * @return: the height of the bitmap
         */
        uint16_t getHeight(void);

        /*
         * @return: the width of the bitmap
         */
        uint16_t *getBuffer(void);
};

#endif /* BITMAP_H_ */