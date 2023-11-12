cmake_minimum_required(VERSION 3.26)
project(spi_reg_read)

find_package(Boost REQUIRED program_options)

add_executable(spi_reg_read spi_reg_read.cpp)
install(TARGETS spi_reg_read DESTINATION local/bin)
if(Boost_FOUND) 
	include_directories(${Boost_INCLUDE_DIRS})
	target_link_libraries(spi_reg_read ${Boost_LIBRARIES})
else()
	message(FATAL_ERROR "Boost not found")
endif()
