/**
 * @file type_def.h
 * @author Yousab Gamal(yous.gmm@gmail.com)
 * @brief A file that includes all data types declarations
 * @version 0.1
 * @date 2025-10-14
 * @copyright Copyright (c) 2025
 */
#ifndef type_def
#define	type_def

/******************Section: Includes**********************/

/******************Section: Macros Functions Declarations*/

/******************Section: Data Types Declarations*******/
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

typedef signed char     sint8;
typedef signed short    sint16;
typedef signed int      sint32;

/******************Section: Macros Declarations***********/
#define ZERO    0x00
#define ONE     0x01

#define TRUE    0x01
#define FALSE   0x00

#define SDA_MAX30100_AND_MPU650_SENSOR_PIN   0X08
#define SCL_MAX30100_AND_MPU650_SENSOR_PIN   0X09
#define SDA_MLX90614_SENSOR_PIN   0X06
#define SCL_MLX90614_SENSOR_PIN   0X07
#define GSR_INPUT_SENSOR_PIN     0X04

/******************Section: Functions Declarations********/

#endif	

