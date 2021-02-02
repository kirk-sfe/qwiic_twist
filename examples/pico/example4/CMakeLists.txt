
# Setup the build environment
#

cmake_minimum_required(VERSION 3.12) 

include(pico_sdk_import.cmake)

project(twist_example4)

include(sparkfun_sdk_import.cmake)

# init the build environment
pico_sdk_init()

add_executable(twist_example4
   	twist_example4.cpp
)

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(twist_example4 pico_stdlib sparkfun_qwiic_twist)

#enable USB output, disable uart output
pico_enable_stdio_usb(twist_example4 1)
pico_enable_stdio_uart(twist_example4 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(twist_example4)
