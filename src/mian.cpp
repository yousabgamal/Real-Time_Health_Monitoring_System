#include "main.h"

static const char* MY_BELT_ID = "belt_id";
static bool ret_val = true , handshakeDone = false;
static float accX = 0.0 , accY = 0.0 , accZ = 0.0 , Temp_Val = 0.0 , Heart_Rate = 0.0 ;
static uint8 SPo2_Ratio = 0;
static uint16 GSR_Val = 0;

const char* ssid = "network_name";
const char* password = "network_password";

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8 * payload, size_t length);

void HeartRateTask(void *pvParameters) 
{
  while(ONE)
  {
    Heart_Rate_And_SPo2_Sensor_Update();
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  Initialize_Communication(115200);
  ret_val = Acceleration_Sensor_Config();
    if(ret_val == true)
    {
        Serial.println("MPU6050 initialized successfully.");
    }
    else
    {
        while(ONE)
        {
            Serial.println("Couldn't initialize MPU6050!!");
            delay(1000);
        }
    }
  delay(150);
  ret_val = Temperature_Sensor_Config();
    if(ret_val == true)
    {
        Serial.println("MLX90614 initialized successfully.");
    }
    else
    {
        while(ONE)
        {
            Serial.println("Couldn't initialize MLX90614!!");
            delay(1000);
        }
    }
  delay(150);
  ret_val = Heart_Rate_And_SPo2_Sensor_Config();
    if(ret_val == true)
    {
        Serial.println("MAX30100 initialized successfully.");
    }
    else
    {
        while(ONE)
        {
            Serial.println("Couldn't initialize MAX30100!!");
            delay(1000);
        }
    } 
  delay(150);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Connected.");

  webSocket.beginSSL("domain_name", "port_number", "endpoint");
  webSocket.onEvent(webSocketEvent);
  xTaskCreatePinnedToCore(
  HeartRateTask,       
  "HeartRateTask",     
  4096,                
  NULL,               
  1,                   
  NULL,               
  1);                    
}

void webSocketEvent(WStype_t type, uint8 * payload, size_t length)
{ 
  switch (type)
  {
    case WStype_CONNECTED:// When the connection is established
      Serial.println(" Connected to SignalR Hub.");
      webSocket.sendTXT("{\"protocol\":\"json\",\"version\":1}\x1e");
      break;
 
    case WStype_TEXT:
      Serial.printf(" Message: %s\n", payload);
      if (strstr((char*)payload, "{}") || strstr((char*)payload, "{\"type\":1}")) 
      {
        handshakeDone = true;
        Serial.println(" Handshake done.");
      }
      break;

    case WStype_DISCONNECTED:
      Serial.println(" Disconnected!");
      handshakeDone = false;
      break;
  }
}

void loop() {

  static uint32 lastSend = 0;
  static uint32 lastHeart = 0;

  webSocket.loop();

  if (millis() - lastHeart > TWO_SECONDS_CHECK) 
  {
    lastHeart = millis();
    for(int i = 0; i < 99; i++) {
      Heart_Rate_And_SPo2_Sensor_Update();
    }
    Get_Heart_Rate_And_SPo2_Ratio(&Heart_Rate , &SPo2_Ratio);
  }
  else
  {
    /* Nothing */
  }

  if (handshakeDone && millis() - lastSend > ONE_SECOND_CHECK) 
  {
    lastSend = millis();

    Get_Acceleration_Sensor(&accX , &accY , &accZ);
    Get_Temperature_Sensor(&Temp_Val);
    Get_Physical_Effort(&GSR_Val);

    JsonDocument doc;
    doc["BeltID"] = MY_BELT_ID;        
    doc["heartRate"] = Heart_Rate;
    doc["spo2"] = SPo2_Ratio;
    doc["temperature"] = Temp_Val;
    doc["accX"] = accX;
    doc["accY"] = accY;
    doc["accZ"] = accZ;
    doc["sweat"] = GSR_Val;
    doc["timestamp"] = millis();

    String json;
    serializeJson(doc, json);

    String msg = "{\"type\":1,\"target\":\"SendSensorData\",\"arguments\":[" + json + "]}\x1e";
    webSocket.sendTXT(msg);

    Serial.println("Sent data: " + json);
  }
  else
  {
    /* Nothing */
  }
}

