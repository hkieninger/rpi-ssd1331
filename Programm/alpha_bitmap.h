#ifndef ALPHA_BITMAP_H_
#define ALPHA_BITMAP_H_ 1

//c++
#include <string> //std::string
//c
#include <stdint.h> //uint16_t
//h
#include "bitmap.h" //Bitmap

class AlphaBitmap: public Bitmap {
    bool *alpha;
    public:
        /*
         * constructor for bitmap from buffer
         * @width: the width of the bitmap
         * @height: the height of the bitmap
         * @buffer: the buffer containing the 565 RGB (16 bit) pixel data
         *          it must be allocated with new before, since it will be deleted by the destructor
         * @alpha: the buffer containing the alpha values of the pixel data
         *         each pixel has an alpha value. If the value is false the pixel is invisible and if alpha is true the pixel is visible.
         *         the buffer must be allocated with new before, since it will be deleted by the destructor
         */
        AlphaBitmap(uint16_t width, uint16_t height, uint16_t *buffer, bool *alpha);

        /*
         * constructor for bitmap from png file
         * @throw: std::runtime_error
         * @png: the path to the png file
         */
        AlphaBitmap(std::string pngFile);

        /*
         * destructor, deletes the two buffers from the heap
         */
        ~AlphaBitmap(void);

        /*
         * @return: the buffer containing the alpha values
         */
        bool *getAlpha(void);
};

#endif /* ALPHA_BITMAP_H_ */