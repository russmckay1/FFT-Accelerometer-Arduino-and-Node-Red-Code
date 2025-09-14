  
// *    Wemos D1 Mini - ADXL345 -
// NOTE : MUST USE arduinoFFT library by Ennque enrique@shapeoko.com 1.6.2
 
// Replace the next variables with your SSID/Password combination

// *********************SET WiFi Credentials  
//const char* ssid = "VodafoneMobileWiFi-A360F3";
//const char* password = "6hwJL54279";
// *********************SET WiFi Credentials  
const char* ssid = "BTHub3-Q2XC";
const char* password = "eb54cdde74";
// ************** SET TOPIC NAME FOR ACCELEROMETER **************
const char* topic="COL-BC-010"; 
const char* topic_temp="temp-1001"; 
// ************** SET MQtt SERVER IP ADDRESS ********************
const char* mqtt_server = "192.168.1.174";    // MQtt Broker address
// ********************** END CONFIGURATION 

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include "arduinoFFT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

arduinoFFT FFT = arduinoFFT();
#define ONE_WIRE_BUS 2
String temp;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);





unsigned int sampling_period_us;
unsigned long microseconds;
#define SAMPLES 256            //Must be a power of 2
#define SAMPLING_FREQUENCY 3200 //Hz, must be less than 10000 due to ADC
//#define SAMPLING_FREQUENCY 6400 //Hz, must be less than 10000 due to ADC

double vReal[SAMPLES];
double vImag[SAMPLES];
double tx[SAMPLES];
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
void setup_wifi() {
// *************************  RMCK IAConnects***************** 
//  IPAddress ip(10,42,0,6);  
//  IPAddress dns(10,42,0,1);
//  IPAddress gateway(10,42,0,1);
//  IPAddress subnet(255, 255, 255, 0);
//  WiFi.config(ip, dns, gateway, subnet);

  // *********************  RMCK PI GATEWAY ***********************
 // IPAddress ip(192,168,1,109);  
 // IPAddress dns(192,168,1,254);
 // IPAddress gateway(192,168,1,254);
//  IPAddress subnet(255, 255, 255, 0);
 // WiFi.config(ip, dns, gateway, subnet);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print("...");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
// ********************************************
 
  // We start by connecting to a WiFi network
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
  WiFiClient espClient;
  PubSubClient client(espClient);
  long lastMsg = 0;
  char msg[50];
  int value = 0;

void reconnect() {
 
// Loop until we're reconnected
   while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
//  Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
// Attempt to connect
    if (client.connect(clientId.c_str())) {
       Serial.println("connected");
// Once connected, publish an announcement...
       client.publish("rmck", "Success!! NEW MQtt Connection Available");
     }
   }
 }
  
//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster

}

void displaySensorDetails(void)

{
 sensor_t sensor;
 accel.getSensor(&sensor);
}
 


void displayRange(void)
{
Serial.print ("Range: +/- ");
 
 
// Serial.println(" g");
}

void setup() {
  //*********************************** SLEEP ********************

 setup_wifi();
 sensors.begin(); 

Serial.begin(115200);
Serial.setTimeout(2000);
while(!Serial){}
Serial.println("System NOT in deep sleep mode!");


 
void setRange(range_t ADXL345_RANGE_16G);
sampling_period_us = round(10000 * (1.0 / SAMPLING_FREQUENCY));

  
  //if you get here you have connected to the WiFi
  Serial.println("connected to gateway...)");
  //client.setServer(mqtt_server, 1994);
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);

  if(!accel.begin())
 {
 /* There was a problem detecting the ADXL345 ... check your connections */
 Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
 while(1);
 }
 /* Set the range to whatever is appropriate for your project */
   
     accel.setRange(ADXL345_RANGE_16_G);
     // displaySetRange(ADXL345_RANGE_8_G);
     // displaySetRange(ADXL345_RANGE_4_G);
     // displaySetRange(ADXL345_RANGE_2_G);
     /* Display some basic information on this sensor */
 
 displaySensorDetails();
 /* Display additional settings (outside the scope of sensor_t) */
// displayDataRate();
// displayRange();
}

void loop() {
//
if (WiFi.status() == WL_CONNECTED)
        {
          while(WiFi.status() == WL_CONNECTED)
          { 
          sensors_event_t event;
          accel.getEvent(&event);  
            {
             if (!client.connected()) 
              {
                 reconnect();
               } 


/*SAMPLING*/

  sensors_event_t event;
  accel.getEvent(&event);
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros();    //Overflows after around 70 minutes!
    sensors_event_t event;
    accel.getEvent(&event);
   

    vReal[i] = event.acceleration.x;
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us)) {
    }
  }


  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

/*PRINT RESULTS*/
Serial.print("Peak vibration = ");
  Serial.print(peak);     //Print out what frequency is the most dominant.


 

client.loop();

   if (client.publish(topic,"s"))
        {
        }
        else
        {
           Serial.println("Publish failed");
        } 


  for (int i = 4; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/

   // Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
   // Serial.print(" ");
   // Serial.println(vReal[i], 1),i;    //View only this line in serial plotter to visualize the bins
    
    tx[i]=vReal[i];

    String payload = String(i) + " " +  String(tx[i]);
   client.loop();
   if (client.publish(topic, (char*) payload.c_str()))
        {
        }
        else
        {
           Serial.println("Publish failed");
        } 
  }
  //Serial.println("ENDOFSTRING");
  client.loop();
   if (client.publish(topic,"ENDOFSTRING"))
        {
        }
        else
        {
           Serial.println("Publish failed");
        };
//Serial.print(" ");
//sensors.requestTemperatures();
//Serial.print("Temp = ");
//temp=(sensors.getTempCByIndex(0));
//Serial.print (temp);
//if (client.publish(topic_temp,String(temp).c_str(), true))
//        {
//        }
//        else
//        {
//           Serial.println("Publish failed");
//        };


Serial.println("");
  delay(500); //Repeat the process every second OR:
//  while(1);       //Run code once
 // ** SLEEP HERE ***
// Serial.print("SLEEP NOW - 15s");
// ESP.deepSleep(15e6);
}
 client.loop();
 
  

delay(100);
             }
           }              
         }
