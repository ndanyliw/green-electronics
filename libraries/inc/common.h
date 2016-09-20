/**
 * @file common.h
 * @brief This header imports the standard imports that should be
 * included in all files.
 * 
 * @author Ned Danyliw
 * @date 9.2016
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "core_cm4.h"
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
// #include "stm32f3_discovery_lsm303dlhc.h"
// #include "stm32f3_discovery_l3gd20.h"
#include "stm32f30x_conf.h"
#include "math.h"


extern void delay_ms(__IO uint32_t ms);

#endif /* COMMON_H_ */
