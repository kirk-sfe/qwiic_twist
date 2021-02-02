
# Setup the build environment
#

cmake_minimum_required(VERSION 3.12) 

include(pico_sdk_import.cmake)

project(twist_example1)

include(sparkfun_sdk_import.cmake)

# init the build environment
pico_sdk_init()

add_executable(twist_example1
   	twist_example1.cpp
)

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(twist_example1 pico_stdlib sparkfun_qwiic_twist)

#enable USB output, disable uart output
pico_enable_stdio_usb(twist_example1 1)
pico_enable_stdio_uart(twist_example1 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(twist_example1)
