//Taken from microchips spi_reg_read mesa demo

#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <boost/program_options.hpp>


static int spi_fd = -1;

/* MEBA callouts */
#define TO_SPI(_a_)     (_a_ & 0x00FFFFFF) /* 24 bit SPI address */
#define SPI_NR_BYTES     7                 /* Number of bytes to transmit or receive */
#define SPI_PADDING_MAX 15                 /* Maximum number of optional padding bytes */

/// Max size of SPI transfers
#define SPI_BUFFER_SIZE 1024

/// SPI Transmit Buffer
static uint8_t spi_tx_buf[SPI_BUFFER_SIZE];

/// SPI Receive Buffer
static uint8_t spi_rx_buf[SPI_BUFFER_SIZE];

using namespace std;

int read_sx_register(const uint16_t address, uint8_t* buffer, const uint8_t size) {
    cout << "spi_reg_read -> addr = 0x" 
         << hex 
         << address 
         << dec 
         << ", size = " 
         << (int)size 
         << endl;

    uint8_t buf[4] = {0};
    buf[0] = 0x1D;
    buf[1] = (uint8_t)(address >> 8);
    buf[2] = (uint8_t)(address >> 0);
    buf[3] = 0;

    uint16_t len = 4 + size;
    memset(&spi_tx_buf, 0, len);
    memset(&spi_rx_buf, 0, len);
    
    memcpy(&spi_tx_buf, buf, 4);

    spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)spi_tx_buf,
        .rx_buf = (unsigned long)spi_rx_buf,
        .len = len,
        //.speed_hz = spi_freq,
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = true,
    };

    int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        cout << "Error: " << __LINE__ << "> spi_read: " << strerror(errno) << "\n";
        return -1;
    }

    for(int i = 0; i < len; i++) {
        cout << hex << "0x" << (int)spi_rx_buf[i] << " ";
    }

    cout << endl;

    return 0;
}

int spi_reg_read(const uint32_t addr, uint32_t *const value, uint32_t spi_freq, uint32_t spi_padding)
{

    cout << "spi_reg_read -> addr = 0x" 
         << hex 
         << addr 
         << dec 
         << ", frequency = " 
         << spi_freq 
         << ", spi_padding = "
         << spi_padding
         << endl;

    uint8_t tx[SPI_NR_BYTES + SPI_PADDING_MAX] = { 0 };
    uint8_t rx[sizeof(tx)] = { 0 };
    uint32_t siaddr = TO_SPI(addr);
    int ret;

    memset(tx, 0xff, sizeof(tx));
    tx[0] = (uint8_t)(siaddr >> 16);
    tx[1] = (uint8_t)(siaddr >> 8);
    tx[2] = (uint8_t)(siaddr >> 0);

    spi_ioc_transfer tr = {
        .tx_buf = (unsigned long) tx,
        .rx_buf = (unsigned long) rx,
        .len = SPI_NR_BYTES + spi_padding,
        .speed_hz = spi_freq,
        .delay_usecs = 0,
        .bits_per_word = 8,
    };

    ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        cout << "Error: " << __LINE__ << "> spi_read: " << strerror(errno) << "\n";
        return -1;
    }

    uint32_t rxword =
            (rx[3 + spi_padding] << 24) |
            (rx[4 + spi_padding] << 16) |
            (rx[5 + spi_padding] << 8) |
            (rx[6 + spi_padding] << 0);

    *value = rxword;

    cout << hex 
         << "RX: 0x" << (int)tx[0] 
         << " 0x" << (int)tx[1]
         << " 0x" << (int)tx[2]
         << " 0x" << (int)tx[3]
         << " 0x" << (int)rx[3 + spi_padding]
         << " 0x" << (int)rx[4 + spi_padding]
         << " 0x" << (int)rx[5 + spi_padding]
         << " 0x" << (int)rx[6 + spi_padding]
         << dec
         << "\n";

    return 0;
}

int spi_reg_write(const uint32_t addr, const uint32_t value, uint32_t spi_freq, uint32_t spi_padding)
{
    cout << "spi_reg_write -> addr = 0x" 
         << hex 
         << addr 
         << ", value = 0x" 
         << value
         << dec 
         << ", frequency = " 
         << spi_freq 
         << ", spi_padding = "
         << spi_padding
         << endl;

    uint8_t tx[SPI_NR_BYTES] = { 0 };
    uint8_t rx[sizeof(tx)] = { 0 };
    uint32_t siaddr = TO_SPI(addr);
    uint32_t read_back;
    int ret;

    tx[0] = (uint8_t)(0x80 | (siaddr >> 16));
    tx[1] = (uint8_t)(siaddr >> 8);
    tx[2] = (uint8_t)(siaddr >> 0);
    tx[3] = (uint8_t)(value >> 24);
    tx[4] = (uint8_t)(value >> 16);
    tx[5] = (uint8_t)(value >> 8);
    tx[6] = (uint8_t)(value >> 0);

    spi_ioc_transfer tr = {
        .tx_buf = (unsigned long) tx,
        .rx_buf = (unsigned long) rx,
        .len = sizeof(tx),
        .speed_hz = spi_freq,
        .delay_usecs = 0,
        .bits_per_word = 8,
        .cs_change = true,
    };

    ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        cout << hex 
             << "TX: 0x" << (int)tx[0] 
             << " 0x" << (int)tx[1]
             << " 0x" << (int)tx[2]
             << " 0x" << (int)tx[3]
             << " 0x" << (int)tx[3]
             << " 0x" << (int)tx[4]
             << " 0x" << (int)tx[5]
             << " 0x" << (int)tx[6]
             << " -> ERROR\n"
             << dec;        

        cout << "ERROR:" << __LINE__ << ">" << strerror(errno) << "\n";
        return -1;
    }

    cout << hex 
         << "TX: 0x" << (int)tx[0] 
         << " 0x" << (int)tx[1]
         << " 0x" << (int)tx[2]
         << " 0x" << (int)tx[3]
         << " 0x" << (int)tx[3]
         << " 0x" << (int)tx[4]
         << " 0x" << (int)tx[5]
         << " 0x" << (int)tx[6]
         << dec
         << "\n";  

    spi_reg_read(addr, &read_back, spi_freq, spi_padding);
    if (value != read_back) {
        cout << "Read back error: expect: 0x"
             << hex
             << value
             << " actual: 0x"
             << read_back 
             << dec << "\n";
    }

    return 0;
}

int spi_reg_io_init(const string& spi_dev_name, int spi_padding)
{
    int ret, mode = 0;
    cout << "DEV: "     << spi_dev_name
         << "Padding: " << spi_padding
         << "\n";

    if (spi_padding > SPI_PADDING_MAX) {
        cout << "ERROR:" << __LINE__ << "> invalid padding length\n";
        return -1;
    }
    spi_fd = open(spi_dev_name.c_str(), O_RDWR);
    if (spi_fd < 0) {
        cout << "ERROR:" << __LINE__ << "> could not open device " << strerror(errno) << "\n";
        return -1;
    }

    ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    if (ret < 0) {
        close(spi_fd);
        spi_fd = -1;
        cout << "ERROR:" << __LINE__ << "> Error setting wr-mode\n";
        return -1;
    }

    ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
    if (ret < 0) {
        close(spi_fd);
        spi_fd = -1;
        cout << "ERROR:" << __LINE__ << "> Error setting rd-mode\n";
        return -1;
    }

    return 0;
}

namespace po = boost::program_options;

int main(int argc, char** argv) try
{
    uint32_t freq;
    uint32_t padding;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "help message, was that helpful?")
        ("spi_device", po::value<string>()->required(), "spi_dev device")
        ("target_reg", po::value<string>()->required(), "offset register to read or write")
        ("write", po::value<string>(), "write value in hex")
        ("frequency", po::value<uint32_t>(&freq)->default_value(400000), "frequency for spi channel")
        ("padding", po::value<uint32_t>(&padding)->default_value(1), "padding bytes");

    po::positional_options_description p;
    p.add("spi_device", 1);
    p.add("target_reg", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);    

    if(vm.count("help")) {
        cout << desc << endl;
        return 0;
    }

    auto spi_dev_name = vm["spi_device"].as<string>();
    cout << "opening spi dev = " << spi_dev_name << "\n";

    stringstream interp;
    interp << hex << vm["target_reg"].as<string>();
    cout << interp.str() << "\n";
    uint16_t target_reg = 0;
    interp >> target_reg;  

    cout << "target offset registers = 0x" << hex << target_reg << dec << "\n";


    //initial the spi channel
    auto result = spi_reg_io_init(spi_dev_name, 1);

    uint32_t write_value = 0;
    if(vm.count("write")) {
        interp.clear();
        interp.str(string());
        interp << hex << vm["write"].as<string>();
        interp >> write_value;       
        spi_reg_write(target_reg, write_value, freq, padding);  
    } else {
        uint32_t read_val = 0;
        read_sx_register(target_reg, nullptr, 4);
        //spi_reg_read(target_reg, &read_val, freq, padding);
        cout << "register value = 0x" << hex << read_val << "\n";
    }

    return result;
} catch(po::error& e) {
    cout << "ERROR: " << e.what() << "\n";
}


