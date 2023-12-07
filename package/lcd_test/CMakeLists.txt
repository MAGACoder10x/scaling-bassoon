cmake_minimum_required(VERSION 3.26)

project(lcd_test DESCRIPTION "Test for SPI LCD display" LANGUAGES C)

set(CMAKE_CXX_FLAGS "-Wall -O3")
set(CMAKE_CC_FLAGS "-Wall -O3")

include_directories(.)

add_executable(tft)

target_sources(tft PRIVATE
                tft.c
                st7789.c)           

target_link_libraries(tft bcm2835)

install(TARGETS tft DESTINATION bin)


add_executable(display)
target_sources(display PRIVATE display.c)
install(TARGETS display DESTINATION bin)