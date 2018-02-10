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

### Examples
While the library classes are in the [Programm Folder](Programm/), you will find some example programms in the [Examples folder](Examples/). To compile the examples switch to the Examples folder (with `cd Examples`) and run `make`. <br/>
There are four example programms:
- [hello_world](Examples/hello_world.cpp): prints a hello world string on the display
- [picture](Examples/picture.cpp): draws a green background, loads a png and draws it on the background
- [rotation](Examples/rotation.cpp): rotates the screen four times by 90 degree and draws each time a picture loaded from a jpeg
- [clock](Examples/clock.cpp): runs a clock on the screen
