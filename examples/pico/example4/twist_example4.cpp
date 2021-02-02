/**
 * Copyright (c) 2020 SparkFun Electronics
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "qwiic_twist.h"

TWIST myTwist;


int main() {

     // setup stdio. Output device is controlled in CMakeLists.txt file
    stdio_init_all();

	printf("Starting TWIST Example 4 - Connect Colors\n\n");

    // Init the system 

    if(!myTwist.begin()){
    	printf("Error starting the qwiic twist device. Aboarting\n");
  		return 0;
    }

    myTwist.setColor(255/2, 0, 255/2); //Set Red and Blue LED brightnesses to half of max.

    myTwist.connectRed(-10); //Red LED will go down 10 in brightness with each encoder tick
    myTwist.connectBlue(10); //Blue LED will go up 10 in brightness with each encoder tick

    while (1) {

        printf("Count: %d\n", myTwist.getCount());

        printf("Is Pressed: %s\n", (myTwist.isPressed() ? "Yes" : "No"));

        sleep_ms(300);

    }

    return 0;
}
