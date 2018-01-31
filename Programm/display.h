#ifndef DISPLAY_H_
#define DISPLAY_H_ 1

//c
#include <stdint.h> //uint16_t, uint8_t

class Display {
    uint16_t width, height;
    public:
        /* 
         * constructor for a display
         * should be called by derivating classes to initialise the width and the height 
         * @width: the width of the display
         * @height: the height of the display 
         */
        Display(uint16_t width, uint16_t height);

        /*
         * @return: the width of the display in pixel
         */
        uint16_t width(void);

        /*
         * @return: the height of the display in pixel
         */
        uint16_t height(void);

        /*
         * set the color for a pixel on the display
         * @x: the x coordinate of the pixel
         * @y: the y coordinate of the pixel
         * @color: the color to set
         */
        virtual void drawPoint(uint16_t x, uint16_t y, uint16_t color) = 0;

        /*
         * clears the screen, sets all the pixel back to the default color (probably black)
         */
        virtual void clear(void);

        /*
         * draws a line
         * @x0: the starting x position
         * @y0: the starting y position
         * @x1: the stoping x position
         * @y1: the stoping y position
         * @color: the color of the line
         */
        virtual void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        
        /*virtual void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        virtual void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        void drawCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
        void fillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
        void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char& bitmap);*/

        /*
         * calculates the 16 bit version of a 24 bit rgb color
         * @red: red component of the rgb (24 bit) color
         * @green: green component of the rgb (24 bit) color
         * @blue: blue component of the rgb (24 bit) color
         * @return: the 16 bit version of the color
         */
        static uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);
};

#endif /* DISPLAY_H_ */