CC = g++
CFLAGS = -std=c++11
LIBRARIES = -lwiringPi -ljpeg

test: test.o ssd1331.o display.o bitmap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARIES) 

test.o: test.cpp ssd1331.h display.h bitmap.h
	$(CC) -c $(CFLAGS) $<

ssd1331.o: ssd1331.cpp ssd1331.h display.h
	$(CC) -c $(CFLAGS) $<

display.o: display.cpp display.h bitmap.h
	$(CC) -c $(CFLAGS) $<

bitmap.o: bitmap.cpp bitmap.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o test
