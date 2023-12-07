tft:tft.o st7789.o
	gcc -Wall -o tft tft.o st7789.o -lbcm2835
st7789.o:st7789.c st7789.h
	gcc -Wall -c st7789.c -lbcm2835
tft.o:tft.c st7789.h 
	gcc -Wall -c tft.c -lbcm2835
clean:
	rm st7789.o tft.o tft
