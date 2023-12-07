#if !defined(_SPI_H_)
#define _SPI_H_

#include <string>
#include <vector>


#include <linux/spi/spidev.h>

class spi {
public:

    spi(const std::string& spi_dev, uint32_t mode = 0, uint8_t bits = 8, uint32_t speed = 500000);
    ~spi();

    using spi_buffer = std::vector<uint8_t>;

    void write(spi_buffer data, uint16_t delay = 0);

    void write(uint8_t* data, unsigned int len);

    void write_byte(uint8_t data, uint16_t delay = 0);

    void read(spi_buffer& data, uint16_t delay = 0);

    void read_byte(uint8_t& data, uint16_t delay = 0);


    bool transfer(const uint8_t* tx, uint8_t* rx, std::size_t len, uint16_t delay);
    
    spi& cpol(bool set) {
        mode_ |= SPI_CPOL;
        return *this;
    }

    spi& cpha(bool set) {
        mode_ |= SPI_CPHA;
        return *this;
    }

private:
    


    int fd_;
    uint32_t mode_;
    uint32_t speed_;
    uint8_t bits_;
    spi_ioc_transfer xfer_;

};


#endif