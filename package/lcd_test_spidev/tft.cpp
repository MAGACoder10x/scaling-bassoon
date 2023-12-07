/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-TFTM019-1
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with: 
Works with Raspberry pi
****************************************************/

//#include <bcm2835.h>

#include <stdio.h>
#include <time.h>
#include "st7789.h"



#include "utility.h"

char value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int main(int argc, char **argv) try {
    time_t now;
    struct tm *timenow;

    //if(!bcm2835_init())
   // {
    //    return -1;
    //}
  
    st7789_begin();

    
    st7789_mono_bitmap(21, 80, mono_bmp, 128, 128, GREEN);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic1, 170, 320);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic2, 170, 320);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic3, 170, 320);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic4, 170, 320);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic5, 170, 320);
    st7789_display();
    delay_ms(2000);

    st7789_bitmap16(0, 0, pic6, 170, 320);
    st7789_display();
    delay_ms(2000);


    st7789_clear();
    printf("TFT example. Press Ctrl + C to exit.\n");

    uint16_t i=100;
    while(1)
    {
        time(&now);
        timenow = localtime(&now);

        st7789_mono_bitmap(0, 2, Signal816, 16, 8, GOLDEN); 
        st7789_mono_bitmap(30, 2, Msg816, 16, 8, GOLDEN); 
        st7789_mono_bitmap(60, 2, Bluetooth88, 8, 8, GOLDEN); 
        st7789_mono_bitmap(90, 2, GPRS88, 8, 8, GOLDEN); 
        st7789_mono_bitmap(120, 2, Alarm88, 8, 8, GOLDEN); 
        st7789_mono_bitmap(150, 2, Bat816, 16, 8, GOLDEN); 

        st7789_string(5, 304, "MUSIC", 16, 0, WHITE); 
     	
        st7789_string(135, 304, "MENU", 16, 1, GOLDEN); 

        st7789_string(45, 240, "EastRising", 16, 1, RED); 

        st7789_char1616(0, 80, value[timenow->tm_hour / 10],RED);
        st7789_char1616(20,80, value[timenow->tm_hour % 10],RED);
        st7789_char1616(40, 80, ':' , WHITE);
        st7789_char1616(60, 80, value[timenow->tm_min / 10], GREEN);
        st7789_char1616(80, 80, value[timenow->tm_min % 10], GREEN);
        st7789_char1616(120, 80, value[timenow->tm_sec / 10],  BLUE);
        st7789_char1616(140, 80, value[timenow->tm_sec % 10],  BLUE);

        st7789_string(5, 140, "www.buydisplay.com", 16, 1, i); 
	i+=1000; if(i>=65536)i=1000;
    
        st7789_display();
      
    }

    //bcm2835_spi_end();
    //bcm2835_close();
    
    return 0;
} catch(std::exception& ex) {
    std::cout << "exception in main " << ex.what() << std::endl;
}

