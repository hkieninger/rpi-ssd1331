CC = g++
CFLAGS = -std=c++11 -O2
LIBRARIES = -lwiringPi -ljpeg -lpng -lm

all: hello_world rotation picture clock

#link the examples
hello_world: hello_world.o ssd1331.o display.o bitmap.o alpha_bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

rotation: rotation.o ssd1331.o display.o bitmap.o alpha_bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

picture: picture.o ssd1331.o display.o bitmap.o alpha_bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

clock: clock.o ssd1331.o display.o bitmap.o alpha_bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

#compile the examples
hello_world.o: hello_world.cpp ../Programm/ssd1331.h ../Programm/display.h
	$(CC) -c $(CFLAGS) $<

rotation.o: rotation.cpp ../Programm/ssd1331.h ../Programm/display.h ../Programm/bitmap.h
	$(CC) -c $(CFLAGS) $<

picture.o: picture.cpp ../Programm/ssd1331.h ../Programm/display.h ../Programm/alpha_bitmap.h
	$(CC) -c $(CFLAGS) $<

clock.o: clock.cpp ../Programm/ssd1331.h ../Programm/display.h
	$(CC) -c $(CFLAGS) $<

#compile the library
ssd1331.o: ../Programm/ssd1331.cpp ../Programm/ssd1331.h ../Programm/display.h
	$(CC) -c $(CFLAGS) $<

display.o: ../Programm/display.cpp ../Programm/display.h ../Programm/bitmap.h ../Programm/alpha_bitmap.h ../Programm/character.h
	$(CC) -c $(CFLAGS) $<

bitmap.o: ../Programm/bitmap.cpp ../Programm/bitmap.h
	$(CC) -c $(CFLAGS) $<

alpha_bitmap.o: ../Programm/alpha_bitmap.cpp ../Programm/alpha_bitmap.h ../Programm/bitmap.h
	$(CC) -c $(CFLAGS) $<

#remove the object files
clean:
	rm -f *.o
