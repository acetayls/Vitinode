#include <Arduino.h>
#include <TTN_esp32.h>
#include "TTN_CayenneLPP.h"
#include "configuration.h"

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
 
#define BATPIN 36
float batt_level = 0;
float batt_pourcent=0;
float tension_min=3.2; //V //tensions valables uniquement pour les batteries lipo
//float tension_max=4.2; //V 

#if VITI_TYPE == 1
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #define ONE_WIRE_BUS 17
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature sensors(&oneWire);
    DeviceAddress deviceAddress;
    float temp_sonde = 0;

    //Display
    #include <U8g2lib.h>
    #include <Wire.h>
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /*rst*/ 16, /*scl*/ 22, /*sda*/ 21);///* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);
 
    const uint8_t payloadBufferLength = 5;  //nombre de payloadbuffer (=zone de mémoire tampon)
 
#elif VITI_TYPE == 2
    #include <DHT.h>
    #define DHTPIN 16
    #define DHTTYPE DHT22
    DHT dht(DHTPIN, DHTTYPE);
    float humidity = 0;
    float temperature = 0;
    const uint8_t payloadBufferLength = 7;

#elif  VITI_TYPE == 3
    #include <SPI.h>
    #include <Wire.h>
    const int AirValue = 2620;   //mV // 2620 si sonde capa // 4095 si sonde resistive
    const int WaterValue = 880;  //mV // 880 si sonde capa // 850 si sonde resistive
    const int SensorPin = 38; //pin 38
    float soilMoistureValue=0;
    float soilmoisturepercent=0;
    const uint8_t payloadBufferLength = 5;

#elif  VITI_TYPE == 4
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #define ONE_WIRE_BUS 17
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature sensors(&oneWire);
    DeviceAddress deviceAddress;
    float temp_sonde = 0;

    #include <DHT.h>
    #define DHTPIN 16
    #define DHTTYPE DHT22
    DHT dht(DHTPIN, DHTTYPE);
    float humidity = 0;
    float temperature = 0;

    #include <SPI.h>
    #include <Wire.h>
    const int AirValue = 2620;   //mV // 2620 si sonde capa // 4095 si sonde resistive
    const int WaterValue = 880;  //mV // 880 si sonde capa // 850 si sonde resistive
    const int SensorPin = 38;
    float soilMoistureValue=0;
    float soilmoisturepercent=0;

    const uint8_t payloadBufferLength = 11;

#elif VITI_TYPE == 5
    #include <Ultrasonic.h>
    Ultrasonic ultrasonic(16, 17);  //ports de branchement TRIG en 16; ECHO en 17
    float distance = 0 ;
    float ratio = 0 ;
    float niv_pourcent=0;
    const uint8_t payloadBufferLength = 7;

#elif VITI_TYPE == 6
    int pulse = 0; // Variable for saving pulses count.
    float kwh=0;
    float pluie=0;
    float ratio_kwh = 0.00125; // kwh = pulse*ratio_kwh; car 800imp/kwh soit 1/800 kwh par imp
    const int input = 16;
    unsigned long last_send = 0; //milliseconds
    const uint8_t payloadBufferLength = 5;

#elif VITI_TYPE == 7
    int pulse = 0; // Variable for saving pulses count.
    float pluie=0;
    float kwh=0;
    float ratio_pluie = 0.3; // pluie = pulse*ratio_pluie; 0.3 car 0.3mm de pluie par tip de bascule
    const int input = 16; //pin de lecture
    unsigned long last_send = 0; //milliseconds
    const uint8_t payloadBufferLength = 5;

#endif
 
/* Payload buffer is bytes send to ttn and manage by ttn payload formater.
*
* For VITIBOX : [1, int(sonde), int(sonde/100), int(batt),int(batt/100)]
* For VITINODE : [2, int(hum), int(hum/100), int(temp), int(temp/100),int(batt),int(batt/100) ]
* For VITISOL : [3, int(moisture), int(moisture/100), int(batt), int(batt/100) ]
* For VITINODEXTEND : [4, int(sonde), int(sonde/100), int(hum), int(hum/100), int(temp), int(temp/100), int(moisture), int(moisture/100), int(batt),int(batt/100) ] 
* For AGRISONAR : [5, int(distance), int(distance/100), int(batt), int(batt/100) ]
* For VINIPOWER : [6, int(kwh), int(kwh/100), int(batt), int(batt/100) ]
* For VITIPLUIE : [7, int(pluie), int(pluie/100), int(batt), int(batt/100) ]
*/
uint8_t payloadBuffer[payloadBufferLength];
 
void message(const uint8_t* payload, size_t size, int rssi)
{
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(size) + " bytes RSSI=" + String(rssi) + "db");
    for (int i = 0; i < size; i++){
        Serial.print(" " + String(payload[i]));}
        // Serial.write(payload[i]);
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
 
// stores the data on the RTC memory so that it will not be deleted during the deep sleep
RTC_DATA_ATTR int bootCount = 0; 
RTC_DATA_ATTR int pckCounter = 0;   // sending packet number...

void waitForTransactions()
{
    Serial.println("Waiting for pending transactions... ");
    Serial.println("Waiting took " + String(ttn.waitForPendingTransactions()) + "ms");
}
 
void getSensors()
{
    batt_level = analogRead(BATPIN)* batt_coeff;
    // Serial.print("battery V : ");Serial.println(batt_level);
    batt_pourcent = ((batt_level - tension_min)*100);
    if (batt_pourcent>100){
        batt_pourcent=100;
    } else if (batt_pourcent<0){
        batt_pourcent=0;
    } else{}
    // Serial.print("battery % : ");Serial.println(batt_pourcent);

    #if VITI_TYPE == 1
        sensors.requestTemperatures();
        temp_sonde = sensors.getTempCByIndex(0);

    #elif VITI_TYPE == 2
        humidity = dht.readHumidity();  //HR
        temperature = dht.readTemperature();     // Read temperature as Celsius (the default)

    #elif VITI_TYPE == 3
        soilMoistureValue = analogRead(SensorPin);  //si sonde capa
        Serial.println(soilMoistureValue);
        soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100); //si capa //fait une échelle entre 0 et 100 avec les valeurs de références

    #elif VITI_TYPE == 4
        sensors.requestTemperatures();
        temp_sonde = sensors.getTempCByIndex(0);

        humidity = dht.readHumidity();  
        temperature = dht.readTemperature();     // Read temperature as Celsius (the default)
        
        soilMoistureValue = analogRead(SensorPin);  //si sonde capa
        Serial.println(soilMoistureValue);
        soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100); //si capa

    #elif VITI_TYPE == 5
        distance = ultrasonic.read();
        ratio = 100 - ((distance*100)/300); //modif 300 par la hauteur totale en cm
        Serial.print("hauteur en % : "); Serial.println(ratio);
        
        if ((ratio>0)&&(ratio<=25)){ //choisir ses paliers
            niv_pourcent=10;
        } else if ((ratio>25)&&(ratio<=40)){
            niv_pourcent=35;
        } else if ((ratio>40)&&(ratio<=60)){
            niv_pourcent=50;
        } else if ((ratio>60)&&(ratio<=85)){
            niv_pourcent=75;
        } else if ((ratio>85)&&(ratio<=100)){
            niv_pourcent=90;
        } else{}

    #elif VITI_TYPE == 6
        if(!digitalRead(input)){  //vrai quand =0
            pulse++; //incrémente la variable
            kwh = pulse*ratio_kwh;
            Serial.print("pulse :");Serial.println(pulse);
            delay(500);}
        delay(1); // Delay for stability.

    #elif VITI_TYPE == 7
        if(!digitalRead(input)){
            pulse++;
            pluie = pulse*ratio_pluie;
            Serial.print("pulse :");Serial.println(pulse);
            delay(500)}
        delay(1); // Delay for stability.

    #endif
}
 
void sendData()
{
/***************************************************************************
*  HERE GO THE DATA TO SEND !
 ****************************************************************************/
 
  #if VITI_TYPE == 1
    payloadBuffer[0] = byte(1);
//    uint32_t s = temp_sonde * 100;
    payloadBuffer[1] = int(temp_sonde);
    payloadBuffer[2] = int(int(temp_sonde * 100) % 100);

    payloadBuffer[3] = int(batt_pourcent);
    payloadBuffer[4] = int(int(batt_pourcent * 100) % 100);

    Serial.print("sonde :  ");Serial.println(temp_sonde);
    // display
    u8g2.clearBuffer();
    // sonde
    String to_display = String(temp_sonde)+" C";
    u8g2.setFont(u8g2_font_ncenB24_tr);
    u8g2.drawStr(5,48,to_display.c_str());
    // batt
    String batt_display = String(batt_level)+" V";
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(60,64,batt_display.c_str());
 
    u8g2.sendBuffer();
 
  #elif VITI_TYPE == 2 
    payloadBuffer[0] =  byte(2);

    // uint32_t h = humidity * 100;
    payloadBuffer[1] = int(humidity);
    payloadBuffer[2] = int(int(humidity * 100) % 100);
 
    // uint32_t t = temperature * 100;
    payloadBuffer[3] = int(temperature);
    payloadBuffer[4] = int(int(temperature * 100) % 100);
 
    payloadBuffer[5] = int(batt_pourcent);
    payloadBuffer[6] = int(int(batt_pourcent * 100) % 100);
   
    Serial.print("humidity : ");Serial.println(humidity);
    Serial.print("temp :  ");Serial.println(temperature);

  #elif VITI_TYPE == 3
    payloadBuffer[0] =  byte(3);

    payloadBuffer[1] = int(soilMoistureValue);
    payloadBuffer[2] = int(int(soilMoistureValue * 100) % 100);
 
    payloadBuffer[3] = int(batt_pourcent);
    payloadBuffer[4] = int(int(batt_pourcent * 100) % 100);
    
    if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
        {
        Serial.print(soilmoisturepercent);Serial.println(" pourcent humidité sol");
        }
    
    #elif VITI_TYPE == 4
    payloadBuffer[0] =  byte(3);

    payloadBuffer[1] = int(temp_sonde);
    payloadBuffer[2] = int(int(temp_sonde * 100) % 100);

    uint32_t h = humidity * 100;
    payloadBuffer[3] = int(humidity);
    payloadBuffer[4] = int(int(humidity * 100) % 100);
 
    uint32_t t = temperature * 100;
    payloadBuffer[5] = int(temperature);
    payloadBuffer[6] = int(int(temperature * 100) % 100);

    payloadBuffer[7] = int(soilMoistureValue);
    payloadBuffer[8] = int(int(soilMoistureValue * 100) % 100);
 
    payloadBuffer[9] = int(batt_pourcent);
    payloadBuffer[10] = int(int(batt_pourcent * 100) % 100);
    
    Serial.print("temp sonde :  ");Serial.println(temp_sonde);
    Serial.print("air humidity %: ");Serial.println(humidity);
    Serial.print("temp air °C:  ");Serial.println(temperature);
    if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
        {
        Serial.print(soilmoisturepercent);Serial.println(" pourcent humidité sol");
        }

  #elif VITI_TYPE == 5
    payloadBuffer[0] =  byte(4);

    uint32_t d = distance * 100;
    payloadBuffer[1] = int(distance);
    payloadBuffer[2] = int(int(distance * 100) % 100);

    payloadBuffer[3] = int(niv_pourcent);
    payloadBuffer[4] = int(int(niv_pourcent * 100) % 100);
 
    payloadBuffer[5] = int(batt_pourcent);
    payloadBuffer[6] = int(int(batt_pourcent * 100) % 100);  

    Serial.print("Distance in CM: "); Serial.println(distance); 
    Serial.print("Remplissage en % : "); Serial.println(niv_pourcent); 
    
  #elif VITI_TYPE == 6
    payloadBuffer[0] =  byte(5);

    payloadBuffer[1] = int(kwh);
    payloadBuffer[2] = int(int(kwh * 100) % 100);
 
    payloadBuffer[3] = int(batt_pourcent);
    payloadBuffer[4] = int(int(batt_pourcent * 100) % 100);
   
    Serial.print(pulse);Serial.println(" pulse = ");
    Serial.print(kwh);Serial.println(" kWh \n");

    #elif VITI_TYPE == 7
    payloadBuffer[0] =  byte(6);

    payloadBuffer[1] = int(pluie);
    payloadBuffer[2] = int(int(pluie * 100) % 100);
 
    payloadBuffer[3] = int(batt_pourcent);
    payloadBuffer[4] = int(int(batt_pourcent * 100) % 100);
   
    Serial.print(pulse);Serial.println(" pulse = ");
    Serial.print(pluie);Serial.println(" mm \n");

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
       
        Wire.begin(21, 22);
        u8g2.begin();
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr(5,16,"Starting ...");
        u8g2.sendBuffer();
//        u8g2.drawStr(5,32,"Not Connected yet");
    #elif VITI_TYPE == 2
        dht.begin();
    #elif VITI_TYPE == 4
        sensors.begin();
        dht.begin();
    #elif VITI_TYPE == 6
        pinMode(input, INPUT_PULLDOWN);
    #elif VITI_TYPE == 6
        pinMode(input, INPUT_PULLDOWN);
    #endif

    // Print the wakeup reason for ESP32
    //print_wakeup_reason();
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
        
    #if VITI_TYPE != (6) && VITI_TYPE != (7) // Les autres que 6 et 7 utilisent le deep sleep, pour 6&7, voir dans loop()
        // Make sure any pending transactions are handled first
        waitForTransactions();
        //attendre
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
    #endif
}

void loop()
{
    #if VITI_TYPE == 6  || VITI_TYPE == 7
        // on regarde en permanence si on à une impulsion.
        getSensors();

        // tout les time_between_send Milliseconds, on envoi à TTN (et on remet à zero de compteur d'impulsion)
        if ((millis() - last_send) > time_between_send) {
            // Send our data
            sendData();
            // Make sure our transactions is handled before going to sleep
            waitForTransactions();
            // Remise à Zéro du compteur d'impulsion
            pulse = 0;
            kwh=0;
            pluie=0;

            last_send = millis();
        }
    #endif
    // for other, go to deep sleep --> Never called
}
