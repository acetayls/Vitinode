#include <Arduino.h>
#include <TTN_esp32.h>

#include "TTN_CayenneLPP.h"
/***************************************************************************
 *  Go to your TTN console register a device then the copy fields
 *  and replace the CHANGE_ME strings below
 * 
 *  REMOVE : .pio/libdepds/ttgo-lora-v32v1/TTTN_esp32/src/BLE*****
 * 
 * 
 * 
 ****************************************************************************/
const char* devEui = "70B3D57ED0053832"; // Change to TTN Device EUI
const char* appEui = "0000000000000000"; // Change to TTN Application EUI
const char* appKey = "FD6E0F1EC93C8FA12B23C1E5A1DE9C80"; // Chaneg to TTN Application Key

// ADJUST TIME BETWEEN SEND
const unsigned long secs_between_send = 60*60;

#define VITI_TYPE 1 //si vitbox : 1 --> sonde DS18B20 sinon 2 --> sonde DHT22


TTN_esp32 ttn ;
TTN_CayenneLPP lpp;
const uint8_t fport = 10;

/******
 * 
 *  SENSOR DECLARATION
 * 
 ******/
#include <Adafruit_Sensor.h>
#include <Adafruit_I2CDevice.h>


#if VITI_TYPE == 1
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #define ONE_WIRE_BUS 16
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature sensors(&oneWire);
    DeviceAddress deviceAddress;
    float temp_sonde = 0;

    const uint8_t payloadBufferLength = 3;


#elif VITI_TYPE == 2
    #include <DHT.h>
    #define DHTPIN 4
    #define DHTTYPE DHT22
    DHT dht(DHTPIN, DHTTYPE);
    float humidity = 0;
    float temperature = 0;

    const uint8_t payloadBufferLength = 5;
#endif


/* Payload buffer is bytes send to ttn and manage by ttn payload formater.
*
* For vitibox : [1, int(sonde), sonde/100]
*
* For vititeo : [2, int(hum), int(hum/100), int(temp), int(temp/100) ]
*/
uint8_t payloadBuffer[payloadBufferLength];



void message(const uint8_t* payload, size_t size, int rssi)
{
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(size) + " bytes RSSI=" + String(rssi) + "db");
    for (int i = 0; i < size; i++)
    {
        Serial.print(" " + String(payload[i]));
        // Serial.write(payload[i]);
    }

    Serial.println();
}

void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case 1:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 3:
        Serial.println("Wakeup caused by timer");
        break;
    case 4:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 5:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.println("Wakeup was not caused by deep sleep");
        break;
    }
}

void waitForTransactions()
{
    Serial.println("Waiting for pending transactions... ");
    Serial.println("Waiting took " + String(ttn.waitForPendingTransactions()) + "ms");
}

void getSensors()
{
    #if VITI_TYPE == 1
        sensors.requestTemperatures();
        temp_sonde = sensors.getTempCByIndex(0);
    #elif VITI_TYPE == 2
        humidity = dht.readHumidity();
        // Read temperature as Celsius (the default)
        temperature = dht.readTemperature();
    #endif
}


void sendData()
{
/***************************************************************************
 *  HERE GO THE DATA TO SEND !
 * 
 * 
 ****************************************************************************/
    /*
    const float temperature = temperatureRead();
    lpp.reset();
    lpp.addTemperature(1, temperature);
    if (ttn.sendBytes(lpp.getBuffer(), lpp.getSize()))
    {
        Serial.printf("Temp: %f TTN_CayenneLPP: %d %x %02X%02X\n", temperature, lpp.getBuffer()[0], lpp.getBuffer()[1],
            lpp.getBuffer()[2], lpp.getBuffer()[3]);
    }
    */
  
  #if VITI_TYPE == 1
    payloadBuffer[0] = byte(1);
    uint32_t s = temp_sonde * 100;
    payloadBuffer[1] = int(temp_sonde);
    payloadBuffer[2] = int(int(temp_sonde * 100) % 100);
    Serial.print("sonde :  ");Serial.println(temp_sonde);
  #elif VITI_TYPE == 2
    payloadBuffer[0] =  byte(2);

    uint32_t h = humidity * 100;
    payloadBuffer[1] = int(humidity);
    payloadBuffer[2] = int(int(humidity * 100) % 100);

    uint32_t t = temperature * 100;
    payloadBuffer[3] = int(temperature);
    payloadBuffer[4] = int(int(temperature * 100) % 100);

    
    Serial.print("humidity : ");Serial.println(humidity);
    Serial.print("temp :  ");Serial.println(temperature);
  #endif

  Serial.println("SEND TO TTN : ");
  for (int i = 0; i < sizeof(payloadBuffer); i++) {
    Serial.println(payloadBuffer[i]);
}
  Serial.println(payloadBufferLength);
  ttn.sendBytes(payloadBuffer, payloadBufferLength, fport);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting");

    #if VITI_TYPE == 1
        sensors.begin();
    #elif VITI_TYPE == 2
        dht.begin();
    #endif
    
    

    // Print the wakeup reason for ESP32
    print_wakeup_reason();

    ttn.begin();
    // Declare callback function for handling downlink messages from server
    ttn.onMessage(message);
    // Join the network
    ttn.join(devEui, appEui, appKey);
    Serial.print("Joining TTN ");
    while (!ttn.isJoined())
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\njoined!");

    // Make sure any pending transactions are handled first
    waitForTransactions();
    //
    getSensors();
    // Send our data
    sendData();
    // Make sure our transactions is handled before going to sleep
    waitForTransactions();

    // Sleep time in micro seconds so multiply by 1000000
    esp_sleep_enable_timer_wakeup(secs_between_send * 1000000);

    // Go to sleep now
    Serial.println("Going to sleep!");
    esp_deep_sleep_start();
    // Everything beyond this point will never be called
}

void loop()
{
    // Never called
}