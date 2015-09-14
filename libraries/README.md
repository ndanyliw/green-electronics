# Library Description

These libraries provide helper functions for the STM32 F3 Discovery board's most commonly used functionality. It has been designed to work with the Green Electronics breakout board. Additionally the libraries assume you will use a specific configuration for the board. If you wish to use more advanced features or non standard configurations you may have to write code to extend or replace the libraries.

## Using the libraries

To use the libraries you simply need to include the appropriate files. The Makefile in the project template has been written to automatically link against the libraries.

Make sure to `include "common.h"` if you will be using the libraries as it has some includes that everything depends on.

## Available Libraries

### ge_lcd

This library provides the interface for communicating with the class LCD. It is written to use the LCD header on the breakout board.

### ge_adc

Provides a basic interface for the ADCs on the STM32. There are many modes in which the ADCs can operate. This library assumes you will only convert in single-ended mode with sequential conversions.

### ge_pwm

PWM library using TIMER0 for generating PWM on its associated channels.

## Resource Usage

Outlined below are the resources used by the Green Electronics libraries. Using/changing configurations for these resources can break library functionality. So be careful!

### Pins

| Pin    | Used in   | Function   |
|--------|-----------|------------|
| PD2    | ge_lcd    | LCD data pin|
| PD0    | ge_lcd    | LCD data pin|
| PD7    | ge_lcd    | LCD data pin|
| PD6    | ge_lcd    | LCD data pin|
| PD5    | ge_lcd    | LCD data pin|
| PD4    | ge_lcd    | LCD data pin|