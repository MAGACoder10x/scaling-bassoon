#include "spi.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#include <iostream>
#include <stdexcept>
#include <string.h>

using namespace std;

spi::spi(const string& spidev, uint32_t mode, uint8_t bits, uint32_t speed) :
mode_(mode),
speed_(speed),
bits_(bits)
{

    memset(&xfer_, 0, sizeof(spi_ioc_transfer));

    cout << "setting mode = " << mode_ << endl;

    if(fd_ = open(spidev.c_str(), O_RDWR); fd_ > 0) {

        int request = mode;
        auto result = ioctl(fd_, SPI_IOC_WR_MODE, &mode_);
        if(result == -1) {
            cout << "failed to set spi mode" << endl;
        }

        result = ioctl(fd_, SPI_IOC_RD_MODE32, &mode_);
        if(result == -1) {
            cout << "failed to read spi mode" << endl;
        } else {
            cout << hex << "mode = 0x" << mode_ << endl;
        }

        if(mode != mode_) {
            cout << "device does not support spi mode" << endl;
        }

        result = ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits);
        if(result == -1) {
            cout << "failed to set bits per word";
        }

        result = ioctl(fd_, SPI_IOC_RD_BITS_PER_WORD, &bits);
        if(result == -1) {
            cout << "failed to set bits per word";
        }

        result = ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if (result == -1){
            cout << "can't set max speed hz\n";
        }

        result = ioctl(fd_, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
        if (result == -1) {
            cout << "can't get max speed hz\n";
        }

    } else {
        cout << "could not open device = " << spidev << endl;
    }

}

void spi::write(spi_buffer data, uint16_t delay) {
    spi_buffer rx_data(data.size());
    transfer(data.data(), rx_data.data(), data.size(), delay);
}

void spi::write(uint8_t* data, unsigned int len) {
    spi_buffer rx_data(len);
    transfer(data, rx_data.data(), len, 0);
}

bool spi::transfer(const uint8_t* tx, uint8_t* rx, std::size_t len, uint16_t delay) {

    xfer_.len = len;
    xfer_.tx_buf = (unsigned long)tx;

#if 0
    spi_ioc_transfer xfer = {
        .tx_buf = (unsigned long)tx,
        //.rx_buf = (unsigned long)rx,
        .len = len,
        //.speed_hz = speed_,       
        //.delay_usecs = delay,
        //.bits_per_word = bits_
    };
#endif

    if(auto result = ioctl(fd_, SPI_IOC_MESSAGE(1), &xfer_); result < 1) {
        return false;
    } else {
        return true;
    }
}

void spi::write_byte(uint8_t data, uint16_t delay) {
    //spi_buffer rx_data(1);
    uint8_t rx = 0;
    transfer(&data, &rx, 1, 0);  
}

void spi::read(spi_buffer& data, uint16_t delay) {
    spi_buffer tx_data(data.size());

    transfer(tx_data.data(), data.data(), data.size(), delay);
}

void spi::read_byte(uint8_t& data, uint16_t delay) {
    uint8_t tx_buff;
    transfer(&tx_buff, &data, 1, delay);  
}

spi::~spi() {
    close(fd_);
}