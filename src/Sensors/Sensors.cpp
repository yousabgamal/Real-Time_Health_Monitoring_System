/**
 * @file Sensors.cpp
 * @author Yousab Gamal(yous.gmm@gmail.com.com)
 * @brief A file that include the functions definition to all the sensors, also
 *        include the sensors pins configuration.
 * @version 0.1
 * @date 2025-10-23
 * @copyright Copyright (c) 2025
 * 
 */

#include "Sensors/Sensors.h"
Heart_Rate_And_SPo2_Sensor_Data_Structure Heart_Rate_And_SPo2_Sensor_Data_Obj;
Temperature_Sensor_Data_Structure Temperature_Sensor_Data_Obj;
Acceleration_Sensor_Data_Structure Acceleration_Sensor_Data_Obj;
Physical_Effort_Data_Structure physical_Effort_Data_Obj;

static void calibrateSensor(void);
static float deadzoneZero(float Value);

/**
 * @brief A function to define the speed of transition and initialize the I2C communication
 * @param bps: Bits per seconds
*/
void Initialize_Communication(uint32 bps)
{
    Serial.begin(bps);
    Wire.begin(SDA_MAX30100_AND_MPU650_SENSOR_PIN , SCL_MAX30100_AND_MPU650_SENSOR_PIN);
    Wire.setClock(100000); 
    while(!(Wire.begin(SDA_MAX30100_AND_MPU650_SENSOR_PIN , SCL_MAX30100_AND_MPU650_SENSOR_PIN)))
    {
        //Bloking the code until the I2C communication is initialized successfully
    }
    Wire1.begin(SDA_MLX90614_SENSOR_PIN , SCL_MLX90614_SENSOR_PIN);
    while(!(Wire1.begin(SDA_MLX90614_SENSOR_PIN , SCL_MLX90614_SENSOR_PIN)))
    {
        //Bloking the code until the I2C communication is initialized successfully
    }
    Wire1.setClock(50000);
}

/**
 * @brief A function to update the heart rate and SPo2 sensor readings
 */
void Heart_Rate_And_SPo2_Sensor_Update(void)
{
    Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.update();
}

/**
 * @brief A function for heart rate and SPo2 sensor pins configrations and communication protcol(I2C)
 * @return true 
 * @return false 
 */
bool Heart_Rate_And_SPo2_Sensor_Config(void)
{
    static bool ret_val = true;
    if(!(Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.begin())) 
    {
        ret_val = false;
    } 
    else
    {
        Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.setIRLedCurrent(MAX30100_LED_CURR_27_1MA); 
    }
    return ret_val;
};

/**
 * @brief A function to get the SPo2 ratio and the heart rate from the athletic
 *        and store the two results in two different pointers
 * @param Heart_Rate: Pointer holds the heart rate data retrieved from the athletic 
 * @param SPo2_Ratio: Pointer holds the SPo2 data retrieve from the athletic 
 * @return Nothing
 */
void Get_Heart_Rate_And_SPo2_Ratio(float * Heart_Rate , uint8 * SPo2_Ratio)
{
    static float lastHeartRate = 0.0f;
    static uint32_t startTime = 0;
    static bool ignoreData = true;
    Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR = Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.getHeartRate();
    if(lastHeartRate != 0)
    {
        if(Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR == 0)
        {
            *(Heart_Rate) = 0;
            *(SPo2_Ratio) = 0;
            lastHeartRate = 0.0f;
            return;
        }
        else{/*Nothing*/}
        if(fabs(lastHeartRate - Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR) > 10.0f)
        {
            *(Heart_Rate) = (Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR * 0.2) + (lastHeartRate * 0.8);
            return;
        }
        else{/*Nothing*/}
    }
    
    if(startTime == 0)
    {
        startTime = millis();
    }
    if(ignoreData)
    {
        if(millis() - startTime >= FIVE_SECOND_CHECK) 
        {
            ignoreData = false; 
        }
        else
        {
            *Heart_Rate = 0; 
            *SPo2_Ratio = 0;
            return;          
        }
    }
    else{/*Nothing*/}
    if(Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR < 40)
    {
        *(Heart_Rate) = 0;
    }
    else
    {
        *(Heart_Rate) = Heart_Rate_And_SPo2_Sensor_Data_Obj.Instant_HR;
        if(*(Heart_Rate) > AVERAGE_HR_AFTER_EFFORT)
        {
            if(Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_MAX == TEMP_HR_MAX)
            {
                Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_MAX = *(Heart_Rate);
            }
            else if(*(Heart_Rate) > Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_MAX)
            {
                Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_MAX = *(Heart_Rate);
            }
        }
        Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_Normal = *(Heart_Rate) / Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_MAX;
    }
    if(*(Heart_Rate) == 0)
    {
        *(SPo2_Ratio) = Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.getSpO2();
    }
    else
    {
        *(SPo2_Ratio) = Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_And_SPo2_Sensor_Lib_Obj.getSpO2();
        if(*(SPo2_Ratio) > 100)
        {
            *(SPo2_Ratio) = 100;
        }
        else if(*(SPo2_Ratio) < 0)
        {
            *(SPo2_Ratio) = 0;
        }
        else
        {
            /*Nothing*/
        }
    }
    lastHeartRate = *(Heart_Rate);
}

/**
 * @brief  A function for temperature sensor pins configrations and communication protcol(I2C)
 * @return true or false
 */
bool Temperature_Sensor_Config()
{
    bool ret_val = true;
    if(!Temperature_Sensor_Data_Obj.Temperature_Sensor_Lib_Obj.begin(0x5A, &Wire1))
    {
        ret_val = false;
    }
    else
    {
        /*Nothing*/
    }
    return ret_val;
}

/**
 * @brief A function to get the temperature of the athletic in celsius degree from 
 *        the sensor and store it in the pointer
 * @param Athletic_Temp: Pointer holds the temperature data retrieved from the athletic 
 * @return Nothing
 */
void Get_Temperature_Sensor(float * Athletic_Temp)
{
    static float difference_check = 0.0f;
    float ambient_temp = Temperature_Sensor_Data_Obj.Temperature_Sensor_Lib_Obj.readAmbientTempC();
    float object_temp  = Temperature_Sensor_Data_Obj.Temperature_Sensor_Lib_Obj.readObjectTempC();
    if((object_temp - ambient_temp) > 2.0)  
    {
        *(Athletic_Temp) = object_temp;
        difference_check = (*(Athletic_Temp) - 36);
        if(difference_check <  0)
        {
            difference_check *= -1;
        }
        else
        {
            /*Nothing*/
        }
        Temperature_Sensor_Data_Obj.Temp_Normal = difference_check / 36; /*36 is the normal body temperature and 3.6 is the difference between the normal body temperature and the maximum body temperature*/
    }
    else
    {
        *(Athletic_Temp) = 0.0;
    }
}

/**
 * @brief A function for acceleration sensor pins configrations and communication protcol(I2C) 
 * @return true 
 * @return false 
 */
bool Acceleration_Sensor_Config(void)
{
    bool ret_val = true;
    if(!Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.begin(0x68, &Wire))
    {  
        ret_val = false;
    }
    else
    {
        /*Nothing*/
    }
    Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.setAccelerometerRange(MPU6050_RANGE_8_G);
    Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.setGyroRange(MPU6050_RANGE_500_DEG);
    Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.setFilterBandwidth(MPU6050_BAND_44_HZ);
    delay(100);
    calibrateSensor();
    return ret_val;
}

/**
 * @brief A function to get the acceleration(x, y and z) from the sensor while 
 *        the athletic runing in the field
 * @param Object_Speed: Pointer holds the acceleration(x, y and z) data retrieved from the the sensor 
 */
void Get_Acceleration_Sensor(float * Final_x , float * Final_y , float * Final_z)
{
    sensors_event_t a, g, temp;
    Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.getEvent(&a, &g, &temp);
    float ax = a.acceleration.x - Acceleration_Sensor_Data_Obj.Baias_X;
    float ay = a.acceleration.y - Acceleration_Sensor_Data_Obj.Baias_Y;
    float az = a.acceleration.z - Acceleration_Sensor_Data_Obj.Baias_Z;

    Acceleration_Sensor_Data_Obj.Filtered_X = ACCEL_LOW_PASS_ALPHA * ax + (1.0f - ACCEL_LOW_PASS_ALPHA) * Acceleration_Sensor_Data_Obj.Filtered_X;
    Acceleration_Sensor_Data_Obj.Filtered_Y = ACCEL_LOW_PASS_ALPHA * ay + (1.0f - ACCEL_LOW_PASS_ALPHA) * Acceleration_Sensor_Data_Obj.Filtered_Y;
    Acceleration_Sensor_Data_Obj.Filtered_Z = ACCEL_LOW_PASS_ALPHA * az + (1.0f - ACCEL_LOW_PASS_ALPHA) * Acceleration_Sensor_Data_Obj.Filtered_Z;

    float outX = deadzoneZero(Acceleration_Sensor_Data_Obj.Filtered_X);
    float outY = deadzoneZero(Acceleration_Sensor_Data_Obj.Filtered_Y);
    float outZ = deadzoneZero(Acceleration_Sensor_Data_Obj.Filtered_Z);

    if(outZ < 0)
    {
        outZ = 0;
    }
    else
    {/*Nothing*/}

    *(Final_x) = outX;
    *(Final_y) = outY;
    *(Final_z) = outZ;
}

/**
 * @brief A function to get the effort of the athletic in the field
 *        (Notice that: it measures in voltage)
 * @param Physical_Effort: Pointer holds the physical effort of the athletic 
 */
void Get_Physical_Effort(uint16 * Physical_Effort)
{
    static uint8 First_Time_Check = 0;
    static uint16 GSR_Value = 0;
    static uint32 start_Time = 0;
    static float Effort = 0.0f;
    GSR_Value = analogRead(GSR_INPUT_SENSOR_PIN);
    if(fabs(GSR_Value - physical_Effort_Data_Obj.GSR_Smooth) > 100 && physical_Effort_Data_Obj.GSR_Smooth != 0)
    {
        physical_Effort_Data_Obj.GSR_Smooth = (0.1f * (float)GSR_Value) + (0.9f * physical_Effort_Data_Obj.GSR_Smooth);
    }
    else
    {
        /*Nothing*/
    }
    if(First_Time_Check == 0 && GSR_Value != 0)
    {
        physical_Effort_Data_Obj.MIN_GSR = GSR_Value;
        physical_Effort_Data_Obj.MAX_GSR = GSR_Value;
        start_Time = millis();    
        while(millis() - start_Time < TEN_SECOND_CHECK)   
        {
            GSR_Value = analogRead(GSR_INPUT_SENSOR_PIN);
            if(GSR_Value > physical_Effort_Data_Obj.MAX_GSR)
            {
                physical_Effort_Data_Obj.MAX_GSR = GSR_Value;
            }
            if(GSR_Value < physical_Effort_Data_Obj.MIN_GSR)
            {
                physical_Effort_Data_Obj.MIN_GSR = GSR_Value;
            }
        }
        First_Time_Check = 1;
    }
    else
    {
        /*Nothing*/
    }
    if(GSR_Value > physical_Effort_Data_Obj.MAX_GSR)
    {
        physical_Effort_Data_Obj.MAX_GSR = GSR_Value;
    }
    else 
    {
        /*Nothing*/
    }
    if(physical_Effort_Data_Obj.MAX_GSR > physical_Effort_Data_Obj.MIN_GSR)
    {
        physical_Effort_Data_Obj.GSR_Normal = (float)(physical_Effort_Data_Obj.GSR_Smooth - physical_Effort_Data_Obj.MIN_GSR) / (float)(physical_Effort_Data_Obj.MAX_GSR - physical_Effort_Data_Obj.MIN_GSR);
        if(physical_Effort_Data_Obj.GSR_Normal > 1)
        {
            physical_Effort_Data_Obj.GSR_Normal = 1;
        }
        else if(physical_Effort_Data_Obj.GSR_Normal < 0)
        {
            physical_Effort_Data_Obj.GSR_Normal = 0;
        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {
        physical_Effort_Data_Obj.GSR_Normal = 0;
    }
    Effort = (0.5f * Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_Normal) + (0.3f * physical_Effort_Data_Obj.GSR_Normal) + (0.2f * Temperature_Sensor_Data_Obj.Temp_Normal);
    *(Physical_Effort) = (uint16)(Effort * 100);
    Heart_Rate_And_SPo2_Sensor_Data_Obj.HR_Normal = 0.0f;
    physical_Effort_Data_Obj.GSR_Normal = 0.0f;
    Temperature_Sensor_Data_Obj.Temp_Normal = 0.0f;
    physical_Effort_Data_Obj.GSR_Smooth = GSR_Value;
}

/**
 * @brief A helper fun to get the bais to be subtracted in the following code
 */
static void calibrateSensor(void)
{
    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    for(int i = 0; i < ACCEL_CAL_SAMPLE_COUNT; ++i)
    {
        sensors_event_t a, g, temp;
        Acceleration_Sensor_Data_Obj.Acceleration_Sensor_Lib_Obj.getEvent(&a, &g, &temp);
        sumX += a.acceleration.x;
        sumY += a.acceleration.y;
        sumZ += a.acceleration.z;
        // delay(5); 
    }
    Acceleration_Sensor_Data_Obj.Baias_X = (float)(sumX / ACCEL_CAL_SAMPLE_COUNT);
    Acceleration_Sensor_Data_Obj.Baias_Y = (float)(sumY / ACCEL_CAL_SAMPLE_COUNT);
    Acceleration_Sensor_Data_Obj.Baias_Z = (float)(sumZ / ACCEL_CAL_SAMPLE_COUNT);
    Acceleration_Sensor_Data_Obj.Filtered_X = Acceleration_Sensor_Data_Obj.Filtered_Y = Acceleration_Sensor_Data_Obj.Filtered_Z = (float)0.0;
}

/**
 * @brief A helper fun to clean the small noise(Filtration) from the sensor
 * @param Value Any value under it will be ignored
 * @return float 
 */
static float deadzoneZero(float Value)
{
    if(fabs(Value) < ACCEL_DEADZONE_THRESHOLD)
    {
        Value = 0.0;
    }
    else
    {
        /*Nothing*/
    }
    return Value;
}