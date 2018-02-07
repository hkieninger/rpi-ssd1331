//c
#include <wiringPi.h> //wiringPiSetup()
#include <time.h> //time(), localtime()
#include <unistd.h> //sleep
#include <math.h> //PI, sinf(), cosf()
//h
#include "display.h" //D_WHITE, D_BLACK, D_RED, D_GREEN, D_BLUE
#include "ssd1331.h" //SSD1331

#define CS 10
#define DC 24
#define RST 25

int main(void) {
    wiringPiSetup();
    SSD1331 display(CS, DC, RST);
    display.begin();

    //determine the dimensions of the clock
    int radius = display.getHeight() - 10;
    int centerX = display.getWidth() / 2;
    int centerY = display.getHeight() / 2;
    //draw the clock
    display.drawEllipse(
        centerX - radius,
        centerY - radius,
        centerX + radius,
        centerY + radius,
        D_WHITE);
    //start the clock
    time_t t = 0;
    struct tm lastNow = *localtime(&t);
    uint16_t xSecond = centerX, ySecond = centerY;
    uint16_t xMinute = centerX, yMinute = centerY;
    uint16_t xHour = centerX, yHour = centerY;
    while(true) {
        time(&t);
        struct tm *now = localtime(&t);
        if(lastNow.tm_sec < now->tm_sec) {
            //draw the second hand
            display.drawLine(centerX, centerY, xSecond, ySecond, D_BLACK);
            float angle = 2 * M_PI / 60 * now->tm_sec;
            xSecond = (uint16_t) (centerX + (radius - 1) * sinf(angle));
            ySecond = (uint16_t) (centerY - (radius - 1) * cosf(angle));
            display.drawLine(centerX, centerY, xSecond, ySecond, D_RED);
        }
        if(lastNow.tm_min < now->tm_min) {
            //draw the minute hand
            display.drawLine(centerX, centerY, xMinute, yMinute, D_BLACK);
            float angle = 2 * M_PI / 60 * now->tm_min;
            xMinute = (uint16_t) (centerX + (2 * radius / 3) * sinf(angle));
            yMinute = (uint16_t) (centerY - (2 * radius / 3) * cosf(angle));
            display.drawLine(centerX, centerY, xMinute, yMinute, D_GREEN);
        }
        if(lastNow.tm_hour < now->tm_hour) {
            //draw the hour hand
            display.drawLine(centerX, centerY, xHour, yHour, D_BLACK);
            float angle = 2 * M_PI / 24 * now->tm_hour;
            xHour = (uint16_t) (centerX + (radius / 3) * sinf(angle));
            yHour = (uint16_t) (centerY - (radius / 3) * cosf(angle));
            display.drawLine(centerX, centerY, xHour, yHour, D_BLUE);
        }
        lastNow = *now;
        //do some other stuff: in this case sleep
        sleep(1);
    }

    display.end();
}