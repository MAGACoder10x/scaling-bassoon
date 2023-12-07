cmake_minimum_required(VERSION 3.26)

project(lcd_test DESCRIPTION "Test for SPI LCD display" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -O3")
set(CMAKE_CC_FLAGS "-Wall -O3")

#find_package(Boost REQUIRED COMPONENTS program_options)

include_directories(.)

add_executable(lcd_test_spidev)

target_sources(lcd_test_spidev PRIVATE
                tft.cpp
                st7789.cpp
                spi.cpp
                utility.cpp)           

target_link_libraries(lcd_test_spidev gpiod stdc++ c) # ${Boost_LIBRARIES})


install(TARGETS lcd_test_spidev DESTINATION bin)