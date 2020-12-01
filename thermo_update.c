// File to implement the functionality of updating the display of the thermometer. Contains 2 helper functions plus the principal thermo_update() function.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thermo.h"


// Called to update the thermometer display.  Makes use of
// set_temp_from_ports() and set_display_from_temp() to access
// temperature sensor then set the display. Checks these functions and
// if they indicate an error, makes not changes to the display.
// Otherwise modifies THERMO_DISPLAY_PORT to set the display.
// 
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
int thermo_update() {
    temp_t temp;
    if(set_temp_from_ports(&temp) == 1) { // set the temperature / check for failure
        return 1;
    }
    
    if(set_display_from_temp(temp, &THERMO_DISPLAY_PORT) == 1) { // update display / check for failure
        return 1;
    }

    // Success!
    return 0;
}



// Uses the two global variables (ports) THERMO_SENSOR_PORT and
// THERMO_STATUS_PORT to set the temp structure. If THERMO_SENSOR_PORT
// is above its maximum trusted value, associated with +50.0 deg C,
// does not alter temp and returns 1.  Otherwise, sets fields of temp
// based on converting sensor value to degrees and checking whether
// Celsius or Fahrenheit display is in effect. Returns 0 on successful
// set. This function DOES NOT modify any global variables but may
// access global variables.

// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.
int set_temp_from_ports(temp_t *temp) {
    
    int unitsmask = 0b1;

    if (THERMO_SENSOR_PORT > 64000 || THERMO_SENSOR_PORT < 0) { 
        // Out of bounds values. Won't change temp.
        return 1;
    }

    if (THERMO_SENSOR_PORT % 64 >= 32) { // Round up.
        temp->tenths_degrees = 1 + (THERMO_SENSOR_PORT/64) - 500; // Since all int divisions round down, we must add 1 to round up. Take away 500 tenths of a degree to get real temp.
    } else { // Round down.
        temp->tenths_degrees = (THERMO_SENSOR_PORT/64) - 500; // C does the work of rounding down for us. Take away 500 tenths of a degree to get real temp.
    }

    if (THERMO_STATUS_PORT & unitsmask) { // Check if the first bit is 1. (denotes fahrenheit vs celsius)
        temp->is_fahrenheit = 1;
        temp->tenths_degrees = (temp->tenths_degrees * 9) / 5 + 320; // Convert value to fahrenheit.
    } else {
        temp -> is_fahrenheit = 0; // Leave the temp value alone
    }

    // Success!
    return 0;
}


// Alters the bits of integer pointed to by display to reflect the
// temperature in struct arg temp.  If temp has a temperature value
// that is below minimum or above maximum temperature allowable or if
// an improper indication of celsius/fahrenheit is given, does nothing
// and returns 1. Otherwise, calculates each digit of the temperature
// and changes bits at display to show the temperature according to
// the pattern for each digit.  This function DOES NOT modify any
// global variables but may access global variables.
int set_display_from_temp(temp_t temp, int *display) {

        // Check for incorrect unit values ------------v    If temp is in fahrenheit, check that it's between 122 and -58 ------------------v     If temp is in celsius, check that it's between 50 and -50 -----------------------v
    if (temp.is_fahrenheit > 1 || temp.is_fahrenheit < 0 || (temp.is_fahrenheit && (temp.tenths_degrees > 1220 || temp.tenths_degrees < -580)) || (!temp.is_fahrenheit && (temp.tenths_degrees > 500 || temp.tenths_degrees < -500))) {
        return 1;
    }
                    // 0-------v  1-------v  2-------v  3-------v  4-------v  5-------v  6-------v  7-------v  8-------v  9-------v
    int maskarr[10] = {0b1111110, 0b0001100, 0b0110111, 0b0011111, 0b1001101, 0b1011011, 0b1111011, 0b0001110, 0b1111111, 0b1011111};
              //(-)-------v
    int negmask = 0b0000001;
             // C--v
    int Cmask = 0b01; 
             // F--v
    int Fmask = 0b10;

    *display = 0; // Set display to 0b00...00

    int count = 0; // Keeps track of what place we should put the negative sign at if needed. Also counts how many #'s the temp contains.
    int temparr[4]; // An array to keep track of which values go where. 0 = tenths, 1 = ones, 2 = tens.
    
    temparr[0] = abs(temp.tenths_degrees)%10; // take the tenths place #
    count += 1; // Houston, we have a number!

    if (abs(temp.tenths_degrees) < 10) {
        temparr[1] = 0; // We need to add a leading 0 if the absolute value is between 0 and 1
        count +=1;
    }

    if (abs(temp.tenths_degrees) >= 10) {
        temparr[1] = (abs(temp.tenths_degrees)/10)%10; // Truncate and take the ones place
        count +=1;
    }

    if (abs(temp.tenths_degrees) >= 100) {
        temparr[2] = (abs(temp.tenths_degrees)/100)%10; // Truncate and take the tens place
        count +=1;
    }

    if (abs(temp.tenths_degrees) >= 1000) {
        temparr[3] = (abs(temp.tenths_degrees)/1000)%10; // Truncate and take the hundreds place (for F only)
        count +=1;
    }

    for (int i = 0; i < count; i++) {
        *display = (*display) | (maskarr[temparr[i]] << i*7); // Shift the correct bits left by i*7 and set them onto display.
    }

    if (temp.tenths_degrees < 0) { // Place negative sign if needed.
        *display = (*display) | (negmask << count*7);
    }

    if (temp.is_fahrenheit) {   // Set to fahrenheit or celsius.
        *display = (*display) | (Fmask << 28);
    } else {
        *display = (*display) | (Cmask << 28);
    }

    // Success!
    return 0;
}
