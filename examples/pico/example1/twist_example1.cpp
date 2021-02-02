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

	printf("Starting TWIST Test\n\n");

    // Init the system 

    if(!myTwist.begin()){
    	printf("Error starting the qwiic twist device. Aboarting\n");
  		return 0;
    }

    while (1) {

        printf("Count: %d\n", myTwist.getCount());

        printf("Is Pressed: %s\n", (myTwist.isPressed() ? "Yes" : "No"));

        sleep_ms(500);

    }

    return 0;
}
