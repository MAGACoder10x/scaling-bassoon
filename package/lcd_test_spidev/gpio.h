#include <gpiod.h>

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

class gpio {
public:
    gpio()=delete;
    gpio(const char* chip_name);
    ~gpio();

    bool gpio_aquire(unsigned int gpio_number, const char* name);

    bool gpio_set_output(std::string gpio_name, unsigned int initial);

    bool gpio_set_input(std::string gpio_name);

    bool gpio_input_read_value(std::string gpio_name, unsigned int& value);

    bool gpio_output_set_value(std::string gpio_name, unsigned int value);

private:
    
    std::string chip_name_;
    gpiod_chip* chip_;

    std::vector<std::pair<std::string, gpiod_line*>> lines_;
};


gpio::gpio(const char* chip_name) : 
chip_name_{chip_name} {
    std::cout << "creating gpio! " << chip_name << std::endl;

    if(chip_ = gpiod_chip_open_by_name(chip_name); chip_ == NULL) {
        throw std::runtime_error("failed to open chip");
    }

    auto num_lines = gpiod_chip_num_lines(chip_);
    std::cout << "number of lines = " << num_lines << std::endl;
    lines_.reserve(num_lines);
}

bool gpio::gpio_aquire(unsigned int gpio_number, const char* name) {
    gpiod_line* line = gpiod_chip_get_line(chip_, gpio_number); 
    if(line != nullptr) {
        lines_.push_back(std::make_pair(name, line));
        return true;
    } else {
        return false;
    }
}

bool gpio::gpio_set_output(std::string gpio_name, unsigned int initial) {
    //std::cout << "gpioname = " << gpio_name << " , inital value = " << initial << std::endl;
    for(auto iter = lines_.begin(); iter != lines_.end(); iter++) {
        auto [name, line] = *iter;
        if(name == gpio_name) {
            gpiod_line_request_output(line, name.c_str(), initial);
            return true;
        }
    }

    return false;
}

bool gpio::gpio_set_input(std::string gpio_name) {
    for(auto iter = lines_.begin(); iter != lines_.end(); iter++) {
        auto [name, line] = *iter;
        if(name == gpio_name) {
            gpiod_line_request_input(line, name.c_str());
            return true;
        }
    }

    return false;
}


bool gpio::gpio_input_read_value(std::string gpio_name, unsigned int& value) {
    for(auto iter = lines_.begin(); iter != lines_.end(); iter++) {
        auto [name, line] = *iter;
        if(name == gpio_name) {
            value = gpiod_line_get_value(line);
            return true;
        }
    }

    return false;
}

bool gpio::gpio_output_set_value(std::string gpio_name, unsigned int value) {
    for(auto iter = lines_.begin(); iter != lines_.end(); iter++) {
        auto [name, line] = *iter;
        if(name == gpio_name) {
            gpiod_line_set_value(line, value);
            return true;
        }
    }

    return false;
}

gpio::~gpio() {
    for(auto [name, line] : lines_) {
        gpiod_line_release(line);
    }

    gpiod_chip_close(chip_);
}


