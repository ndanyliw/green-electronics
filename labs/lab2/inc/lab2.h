/**
 * @file  lab2.h
 * @brief Lab 2 starter code.
 * 
 * @author Ned Danyliw
 * @date  09.2015
 */

#include "ge_libs.h"

//Calibration Voltage in volts (Adjust to your values)
#define CAL_VOLTS 10.0
#define CAL_CURR  3.0

//Initialize the energy meter
void meter_init();

//Display energy meter data
void meter_display();

//Update calibration for 0V and 0A
void calibrate_offset();

//Update calibration for the standard voltage
void calibrate_voltage();

//Update calibration for the standard current
void calibrate_current();

//ADC callback function
void my_adc_callback(uint16_t *data);