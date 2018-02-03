#ifndef DISPLAY_H_
#define DISPLAY_H_ 1

//c
#include <stdint.h> //uint16_t, uint8_t

//definiton of common colors in 565 RGB (16 bit)
#define	D_BLACK           0x0000
#define	D_BLUE            0x001F
#define	D_RED             0xF800
#define	D_GREEN           0x07E0
#define D_CYAN            0x07FF
#define D_MAGENTA         0xF81F
#define D_YELLOW          0xFFE0
#define D_WHITE           0xFFFF

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
        uint16_t getWidth(void);

        /*
         * @return: the height of the display in pixel
         */
        uint16_t getHeight(void);

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
         * draws a horizontal line
         * @x: the x starting coordinate
         * @y: the y starting coordinate
         * @len: the length of the line
         * @color: the color of the line
         */
        virtual void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color);

        /*
         * draws a vertical line
         * @x: the x starting coordinate
         * @y: the y starting coordinate
         * @len: the length of the line
         * @color: the color of the line
         */
        virtual void drawVerticalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color);

        /*
         * draws a line
         * @x0: the starting x position
         * @y0: the starting y position
         * @x1: the stoping x position
         * @y1: the stoping y position
         * @color: the color of the line
         */
        virtual void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        
        /*
         * draws the contours of a rectangle
         * @x0: x position of corner0
         * @y0: y position of corner0
         * @x1: x position of corner diagonally to corner0
         * @y1: y position of corner diagonally to corner0
         * @color: the color of the contours
         */
        virtual void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

        /*
         * fills a rectangle
         * @x0: x position of corner0
         * @y0: y position of corner0
         * @x1: x position of corner diagonally to corner0
         * @y1: y position of corner diagonally to corner0
         * @color: the color of the fill
         */
        virtual void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        
        /*
         * draws the contours of an ellipse
         * @x0: the x position of the corner0 of the rectangle containing the ellipse
         * @y0: the y position of the corner0 of the rectangle containing the ellipse
         * @x1: x position of corner diagonally to corner0 of the rectangle containing the ellipse
         * @y1: y position of corner diagonally to corner0 of the rectangle containing the ellipse
         * @color: the color of the contours
         * 
         *                           rectangle
         * if this is corner0 -> +---------------+
         *                       | /-----------\ |
         *                       |/             \|
         *                       |\   ellispe   /|
         *                       | \-----------/ |
         *                       +---------------+ <- then this is corner1
         */
        void drawEllipse(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        
        /*
         * fills an ellipse
         * @x0: the x position of the corner0 of the rectangle containing the ellipse
         * @y0: the y position of the corner0 of the rectangle containing the ellipse
         * @x1: x position of corner diagonally to corner0 of the rectangle containing the ellipse
         * @y1: y position of corner diagonally to corner0 of the rectangle containing the ellipse
         * @color: the color of the fill
         * 
         *                           rectangle
         * if this is corner0 -> +---------------+
         *                       | /-----------\ |
         *                       |/             \|
         *                       |\   ellispe   /|
         *                       | \-----------/ |
         *                       +---------------+ <- then this is corner1
         */
        void fillEllipse(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
        
        /*
         * draws a bitmap
         * @x: the x position of the top left corner of the bitmap
         * @y: the y position of the top left corner of the bitmap
         * @width: the width of the bitmap
         * @height: the height of the bitmap
         * @bitmap: buffer containing the pixel of the bitmap in 565 RGB (16 bit) color
         */
        void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *bitmap);

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