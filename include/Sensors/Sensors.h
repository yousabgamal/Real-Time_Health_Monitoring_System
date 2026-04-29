/**
 * @file Sensors.h
 * @author Yousab Gamal(yous.gmm@gmail.com)
 * @brief A file that includes all the functions declaration for all sensors
 * @version 0.1
 * @date 2025-10-23
 * @copyright Copyright (c) 2025
 */

 #ifndef SENSORS_H
 #define SENSORS_H

/******************Section: Includes**********************/
#include "type_def.h"
#include "MAX30100_PulseOximeter.h"
#include "Adafruit_MLX90614.h"
#include <Adafruit_MPU6050.h>

/******************Section: Macros Declarations***********/
#define ONE_SECOND_CHECK  1000
#define FIVE_SECOND_CHECK 5000
#define TEN_SECOND_CHECK  10000

#define ACCEL_CAL_SAMPLE_COUNT      500  
#define ACCEL_LOW_PASS_ALPHA        0.15f
#define ACCEL_DEADZONE_THRESHOLD    0.8f

#define TEMP_HR_MAX             198.0f
#define AVERAGE_HR_AFTER_EFFORT 118.8f

/******************Section: Macros Functions Declarations*/

/******************Section: Data Types Declarations*******/
typedef struct
{
    PulseOximeter HR_And_SPo2_Sensor_Lib_Obj = PulseOximeter();
    float HR_MAX = 198.0f;
    float HR_Normal = 0.0f;
    float Instant_HR = 0.0f; 
    float Instant_SPo2 = 0.0f; 
}Heart_Rate_And_SPo2_Sensor_Data_Structure;

typedef struct
{
    Adafruit_MLX90614 Temperature_Sensor_Lib_Obj = Adafruit_MLX90614();
    float Temp_Normal = 0.0f;
}Temperature_Sensor_Data_Structure;

typedef struct
{
    Adafruit_MPU6050 Acceleration_Sensor_Lib_Obj;
    float Baias_X = 0.0f;
    float Baias_Y = 0.0f;  
    float Baias_Z = 0.0f;
    float Filtered_X = 0.0f;
    float Filtered_Y = 0.0f;   
    float Filtered_Z = 0.0f;
}Acceleration_Sensor_Data_Structure;

typedef struct
{
    float GSR_Normal = 0.0f;
    float GSR_Smooth = 0.0f;
    uint16 MAX_GSR = 0;
    uint16 MIN_GSR = 0;
}Physical_Effort_Data_Structure;

/******************Section: Functions Declarations********/
void Initialize_Communication(uint32 bps);

void Heart_Rate_And_SPo2_Sensor_Update(void);
bool Heart_Rate_And_SPo2_Sensor_Config(void);
void Get_Heart_Rate_And_SPo2_Ratio(float * Heart_Rate , uint8 * SPo2_Ratio);

bool Temperature_Sensor_Config(void);
void Get_Temperature_Sensor(float * Athletic_Temp);

bool Acceleration_Sensor_Config(void);
void Get_Acceleration_Sensor(float * filteredAx , float * filteredAy , float * filteredAz);

void Get_Physical_Effort(uint16 * Physical_Effort);

 #endif