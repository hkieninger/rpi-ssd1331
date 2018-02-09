# rpi-ssd1331
Library for the raspberry pi to control a display with the ssd1331 driver. <br/> <br/>


### Dependencies
The library requires libjpeg, libpng and wiringPi installed. WiringPi and libpng are normally installed by default. To install libjpeg run: <br/> 
`sudo apt-get install libjpeg-dev`. <br/> <br/>


### Functionalities
Until now only basic functionalitys are implemented, but there are still a lot of hardware commands of the ssd1331 to add. <br/>
#### Following functions are implemented: <br/>
- rotation of axis by 90 degree
- draw point
- draw line
- draw rectangle
- fill rectangle
- draw ellipse
- fill ellipse
- draw bitmap loaded from jpeg
- draw bitmap (with alpha value) loaded from png
- draw char
- draw string
- clear screen
- turn screen on and off <br/> <br/>

#### Drawing chars and strings
you can create your own fonts for the chars and strings <br/>
therefor you can use the LibreOffice Calc file [Font_Creator.ods](Material/Font_Creator.ods) design your font and copy the automatically generated hex code at the correct place in the [character.h](Programm/character.h) header file <br/>

### Test Programms
Until now there are 2 test programms. <br/>
- test: run `make test` to compile, function dependent of commit
- clock: run `make clock` to compile, function is a simple clock
