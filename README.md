Seric Accelerometer Set-Up Guide – version 1.0

The Seric Accelerometer is comprised of and ADXL-345 MEMS accelerometer and a Wemos 8266 microcontroller.

The accelerometer is preconfigured to operate with a WiFi address in the 192.168.0.x range and uses a WiFi SSID of VodafoneMobileWiFi-A360F3 and password of 6hwJL54279. 

The accelerometer publishes data to an MQtt message broker that is currently running on an IP address of 192.168.0.207

The MQtt topics have been set to 

The SSID and password is changed using the Arduino IDE. The two seric-1001 to 1004

All of the above parameters are easily configured using the Arduino IDE – the parameters are shown in the code snippet below:-

// *********************SET WiFi Credentials  
const char* ssid = "*********";
const char* password = "******";
// ************** SET TOPIC NAME FOR ACCELEROMETER **************
const char* topic="seric-1005"; 
const char* topic_temp="temp-1001"; 
// ************** SET MQtt SERVER IP ADDRESS ********************
const char* mqtt_server = "192.168.0.207";    // MQtt Broker address
// ********************** END CONFIGURATION 

When programming the device it is important to load the correct Arduino libraries. See list below:-
  


Particular attention should be given to the arduinofft library – this MUST be version 1.6.2 and not the latest default version.

Also note that the board type must be set to LOLIN(WEMOS) D1 R2 & mini.
Node-Red subscribes to the message broker on IP address 192.168.0.107. Like other parameters, this IP address can be changed to suite another broker but please ensure that you remember that the IP address range in the accelerometer is currently set to the 192.168.0.x subnet. If you do change the broker IP address please either leave it on the same subnet, or move the accelerometers to a subnet matching that of the broker.

The Node-Red and the Arduino code will be made available to you. 
 

Node-Red can be configured to raise a work order in Maximo by linking the two function nodes as shown in the flow above.

Using a Raspberry Pi 2B to process the accelerometer data

A Raspberry Pi is easily configured to process and monitor the data received from the accelerometer.

Data from the accelerometer is sent to the broker, which is installed on the Raspberry Pi. A Mosquitto broker can be installed on the Raspberry Pi using the following commands:-

1.	sudo apt update sudo apt upgrade
2.	sudo apt install mosquitto mosquitto-clients
3.	After install check that Mosquitto is running with sudo systemctl status mosquitto. 
Node-Red is installed on the Raspberry by default but, by default, it must be started manually. To automatically start Node-Red we use the PM2 application which is installed on the Pi using:-
sudo npm install -g pm2

To automatically start Node-Red on boot enter this command to set pm2:-
pm2 start /usr/bin/node-red -- -v  then enter pm2 save followed by pm2 startup
Finally run sudo env PATH=$PATH:/usr/bin /usr/lib/node_modules/pm2/bin/pm2 startup systemd -u pi --hp /home/pi.


Connecting the device to a power supply.

The devices include a 4-pin Aerospace connector. Pin 1 is connected to the +ve 5 volt and pin 3 is connected to the -ve 5 Volt supply. A standard 5V USB adapter or any other 5V DC power supply may be used to power the device. The device is fitted with a filter to allow connections of up to 20 metres between the power supply and the accelerometer.

Operation and Usage Guidelines

These devices provide an indication of relative vibration amplitude and frequency. The devices should not be used in mission critical applications and should be used to provide an indication of ‘normal’ and ‘anomalous’ vibrations. Using the devices on both a wide band vibration or FFT frequency / amplitude over time will allow uses to identify changes such as increased vibration from what is considered ‘normal’ and identify new vibration signatures at different frequencies.

<img width="451" height="688" alt="image" src="https://github.com/user-attachments/assets/bc9c343b-e2c8-45e5-8e2c-4a5ccdd01859" />

