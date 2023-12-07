/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/

//#include <bcm2835.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <string.h>

#include <iostream>
#include <memory>


#include "st7789.h"
#include <gpiod.h>
#include "utility.h"

using namespace std;

char buffer[TFT_WIDTH * TFT_HEIGHT * 2];

spi_ioc_transfer spi_xfer;
int spi_fd;

gpiod_chip* chip;
gpiod_line* reset_line;
gpiod_line* dc_line;

int spi_transfer(int fd, const uint8_t* tx, uint8_t* rx, std::size_t len) {

    memset(&spi_xfer, 0, sizeof(spi_ioc_transfer));

    spi_xfer.tx_buf = (unsigned long)tx;
    spi_xfer.len = len;

    auto result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_xfer);
    
    return result;
}

void command(char cmd) {

    //bcm2835_gpio_write(DC, LOW);
    //bcm2835_spi_transfer(cmd);

    gpiod_line_set_value(dc_line, 0);
    spi_transfer(spi_fd, (const uint8_t*)&cmd, nullptr, 1);
}

void data(char cmd) {
    //bcm2835_gpio_write(DC, HIGH);
    //bcm2835_spi_transfer(cmd);

    gpiod_line_set_value(dc_line, 1);
    spi_transfer(spi_fd, (const uint8_t*)&cmd, nullptr, 1);
}

void st7789_begin()
{
    if(chip = gpiod_chip_open_by_name("gpiochip0"); chip == NULL) {
        cout << "Failed to open gpio chip" << endl;
        exit(-1);
    }

    dc_line    = gpiod_chip_get_line(chip, covert_gpio(0, 24)); 
    reset_line = gpiod_chip_get_line(chip, covert_gpio(0, 25)); 
    gpiod_line_request_output(dc_line, "DC", 0);
    gpiod_line_request_output(reset_line, "RESET", 0);
    
    memset(&spi_xfer, 0, sizeof(spi_ioc_transfer));

    uint8_t spi_mode = SPI_MODE_0;
    uint8_t bpw = 8;
    int max_speed = 40000000;

    if(spi_fd = open("/dev/spidev0.0", O_RDWR); spi_fd > 0) {

        auto result = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
        if(result == -1) {
            cout << "failed to set spi mode = " << hex << (int)spi_mode << dec << errno <<  endl;
        }

        result = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bpw);
        if(result == -1) {
            cout << "failed to set bits per word\n";
        }

        result = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed);
        if (result == -1){
            cout << "can't set max speed hz\n";
        }

    } else {
        cout << "could not open device = /dev/spidev0.0" << endl;
        exit(-1);
    }


    //bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);
    //bcm2835_gpio_fsel(DC, BCM2835_GPIO_FSEL_OUTP);
    //bcm2835_spi_begin();
    //bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //The default
    //bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //The default
    //bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);  //The default
    //bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //The default
    //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //the default



    gpiod_line_set_value(reset_line, 1);
    delay_ms(10);
    gpiod_line_set_value(reset_line, 0);
    delay_ms(10);
    gpiod_line_set_value(reset_line, 1);

    //bcm2835_gpio_write(RST, HIGH);
    //delay_ms(10);
    //bcm2835_gpio_write(RST, LOW);
    //delay_ms(10);
    //bcm2835_gpio_write(RST, HIGH);

   
 
  //************* Start Initial Sequence **********// 
  command(0x36); 
  data(0x00);

  command(0x3A); 
  data(0x05);

  command(0xB2);
  data(0x0C);
  data(0x0C);
  data(0x00);
  data(0x33);
  data(0x33);

  command(0xB7); 
  data(0x35);  

  command(0xBB);
  data(0x32);


  command(0xC2);
  data(0x01);

  command(0xC3);
  data(0x15);   

  command(0xC4);
  data(0x20);  

  command(0xC6); 
  data(0x0F);    

  command(0xD0); 
  data(0xA4);
  data(0xA1);

  command(0xE0);
  data(0xD0);
  data(0x08);
  data(0x0E);
  data(0x09);
  data(0x09);
  data(0x05);
  data(0x31);
  data(0x33);
  data(0x48);
  data(0x17);
  data(0x14);
  data(0x15);
  data(0x31);
  data(0x34);

  command(0xE1);
  data(0xD0);
  data(0x08);
  data(0x0E);
  data(0x09);
  data(0x09);
  data(0x05);
  data(0x31);
  data(0x33);
  data(0x48);
  data(0x17);
  data(0x14);
  data(0x15);
  data(0x31);
  data(0x34);

  command(0x21); 

  command(0x11); 
    delay_ms(120);
  command(0x29); 


  
  st7789_clear();


}

void st7789_clear() {
    int i;
    for(i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = 0;
    }
}

void st7789_draw_point(int x, int y, uint16_t hwColor) {
    if(x >= TFT_WIDTH || y >= TFT_HEIGHT)
    {
        return;
    }
    buffer[x * 2 + y * TFT_WIDTH * 2] = hwColor >> 8;
    buffer[x * 2 + y * TFT_WIDTH * 2 + 1] = hwColor;
}

void st7789_char1616(uint8_t x, uint16_t y, uint8_t chChar, uint16_t hwColor) {
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y;

    for (i = 0; i < 32; i ++) {
        chTemp = Font1612[chChar - 0x30][i];
        for (j = 0; j < 8; j ++) {
            if (chTemp & 0x80) {
                st7789_draw_point(x, y, hwColor);
            } else {
                st7789_draw_point(x, y, 0);
            }
            chTemp <<= 1;
            y++;
            if ((y - y0) == 16) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void st7789_char3216(uint8_t x, uint16_t y, uint8_t chChar, uint16_t hwColor) {
    uint8_t i, j;
    uint8_t chTemp = 0, y0 = y; 

    for (i = 0; i < 64; i++) {
        chTemp = Font3216[chChar - 0x30][i];
        for (j = 0; j < 8; j++) {
            if (chTemp & 0x80) {
                st7789_draw_point(x, y, hwColor);
            } else {
                st7789_draw_point(x, y, 0);
            }

            chTemp <<= 1;
            y++;
            if ((y - y0) == 32) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void st7789_char(uint8_t x, uint16_t y, char acsii, char size, char mode, uint16_t hwColor) {
    uint8_t i, j;
    uint16_t y0=y;
    char temp;
    uint8_t ch = acsii - ' ';
    for(i = 0;i<size;i++) {
        if(size == 12)
        {
            if(mode)temp=Font1206[ch][i];
            else temp = ~Font1206[ch][i];
        }
        else 
        {            
            if(mode)temp=Font1608[ch][i];
            else temp = ~Font1608[ch][i];
        }
        for(j =0;j<8;j++)
        {
            if(temp & 0x80) st7789_draw_point(x, y, hwColor);
            else st7789_draw_point(x, y, 0);
            temp <<=1;
            y++;
            if((y-y0)==size)
            {
                y = y0;
                x ++;
                break;
            }
        }
    }
}

void st7789_string(uint8_t x, uint16_t y, const char *pString, uint8_t Size, uint8_t Mode, uint16_t hwColor) {
    while (*pString != '\0') {       
        if (x > (TFT_WIDTH - Size / 2)) {
            x = 0;
            y += Size;
            if (y > (TFT_HEIGHT - Size)) {
                y = x = 0;
            }
        }
        
        st7789_char(x, y, *pString, Size, Mode, hwColor);
        x += Size / 2;
        pString ++;
    }
}

void st7789_mono_bitmap(uint8_t x, uint16_t y, const uint8_t *pBmp, char chWidth, char chHeight, uint16_t hwColor) {
    uint16_t i, j, byteWidth = (chWidth + 7) / 8;
    for(j = 0; j < chHeight; j++) {
        for(i = 0; i <chWidth; i ++) {
            if(*(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                st7789_draw_point(x + i, y + j, hwColor);
            }
        }
    }        
}

void st7789_bitmap24(uint8_t x, uint16_t y, uint8_t *pBmp, char chWidth, int chHeight) {
    uint16_t i, j;
    uint16_t hwColor;
    uint32_t temp;

    for(j = 0; j < chHeight; j++) {
        for(i = 0; i < chWidth; i ++) {
            temp = *(unsigned int*)(pBmp + i * 3 + j * 3 * chWidth);
            hwColor = RGB(((temp >> 16) & 0xFF),
                          ((temp >> 8) & 0xFF),
                           (temp & 0xFF));
            st7789_draw_point(x + i, y + chHeight - 1 - j, hwColor);
        }
    }
}


void st7789_bitmap16(uint8_t x, uint16_t y, const uint8_t *pBmp, char chWidth, int chHeight) {
    uint16_t i, j,temp;
    uint16_t hwColor;


    for(j = 0; j < chHeight; j++) {
        for(i = 0; i < chWidth; i ++) {
            hwColor= (*pBmp++);
            hwColor = hwColor<<8;
            temp = (*pBmp++);
	    hwColor = hwColor|temp;         
            st7789_draw_point(x + i, y + j, hwColor);
        }
    }
}



void st7789_display() {
 
    command(0x2a);
    data(0);
    data(35);
    data((TFT_WIDTH-1) >> 8);
    data(TFT_WIDTH+34);

    command(0x2b);
    data(0);
    data(0);
    data(((TFT_HEIGHT-1) >> 8)& 0xFF);
    data((TFT_HEIGHT-1)& 0xFF);

    command(0x2C);


    //bcm2835_gpio_write(DC, HIGH);
    //bcm2835_spi_transfern(buffer, sizeof(buffer));
    gpiod_line_set_value(dc_line, 1);
    spi_transfer(spi_fd, (uint8_t*)buffer, nullptr, sizeof(buffer));
}

void st7789_clear_screen(uint16_t hwColor) {
    uint16_t i, j;
    for(i = 0; i < TFT_HEIGHT; i++) {
        for(j = 0; j < TFT_WIDTH; j ++) {
            st7789_draw_point(j, i, hwColor);
        }
    }
}

