/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2019 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to create a node that repeats messages
 * from nodes far from gateway back to gateway.
 * It is important that nodes that has enabled repeater mode calls
 * process() frequently. Repeaters should never sleep.
 */
 //--------------------------------------------------------------------
//                 +/          
//                 `hh-        
//        ::        /mm:       
//         hy`      -mmd       
//         omo      +mmm.  -+` 
//         hmy     .dmmm`   od-
//        smmo    .hmmmy    /mh
//      `smmd`   .dmmmd.    ymm
//     `ymmd-   -dmmmm/    omms
//     ymmd.   :mmmmm/    ommd.
//    +mmd.   -mmmmm/    ymmd- 
//    hmm:   `dmmmm/    smmd-  
//    dmh    +mmmm+    :mmd-   
//    omh    hmmms     smm+    
//     sm.   dmmm.     smm`    
//      /+   ymmd      :mm     
//           -mmm       +m:    
//            +mm:       -o    
//             :dy             
//              `+:     
//--------------------------------------------------------------------
//   __|              _/           _ )  |                       
//   _| |  |   ` \    -_)   -_)    _ \  |   -_)  |  |   -_)     
//  _| \_,_| _|_|_| \___| \___|   ___/ _| \___| \_,_| \___|  
//--------------------------------------------------------------------    
// 2022/03/28 - FB V1.0.0
//--------------------------------------------------------------------

// Enable debug prints to serial monitor
#define MY_DEBUG

#define VERSION   "v1.0.0"

// Enable and select radio type attached
#define MY_RADIO_RF24

#define MY_RF24_PA_LEVEL     RF24_PA_MAX

// Enabled repeater feature for this node
#define MY_REPEATER_FEATURE

#define MY_DEFAULT_ERR_LED_PIN 6  // Error led pin
#define MY_DEFAULT_RX_LED_PIN  7  // Receive led pin, on board LED
#define MY_DEFAULT_TX_LED_PIN  7  //

#define MY_RF24_CE_PIN 1
#define MY_RF24_CS_PIN 3


#include <MySensors.h>
#include <DHT.h> 

#define DHTPIN 0        // Broche sur laquelle est branché le DHT / what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), DHT11

#define CHILD_ID_TEMP 0  
#define CHILD_ID_HUM 1


uint32_t SEND_FREQUENCY_TEMP =   60000;
uint32_t TIMEOUT_SERIAL =   3000;
uint32_t lastSend_temp = 0;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

DHT dht(DHTPIN, DHTTYPE);



//--------------------------------------------------------------------
// BEFORE
//--------------------------------------------------------------------
void before()
{
uint32_t beginTime = millis();
uint32_t currentTime;

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    currentTime  = millis();
    if (currentTime - beginTime > TIMEOUT_SERIAL) {
      break;
    }
  }
  
  Serial.println(F("   __|              _/           _ )  |"));
  Serial.println(F("   _| |  |   ` \\    -_)   -_)    _ \\  |   -_)  |  |   -_)"));
  Serial.println(F("  _| \\_,_| _|_|_| \\___| \\___|   ___/ _| \\___| \\_,_| \\___|"));
  Serial.print(F("                                             "));
  Serial.println(VERSION);

}

//--------------------------------------------------------------------
// SETUP
//--------------------------------------------------------------------
void setup()
{ 
  Serial.println(F("SETUP"));
  dht.begin();
}

//--------------------------------------------------------------------
// PRESENTATION
//--------------------------------------------------------------------
void presentation()
{
  Serial.println(F("PRESENTATION"));
  //Send the sensor node sketch version information to the gateway
  sendSketchInfo("Repeater Node", VERSION);

  present(CHILD_ID_TEMP, S_TEMP);
  present(CHILD_ID_HUM, S_HUM);
}

//--------------------------------------------------------------------
// LOOP
//--------------------------------------------------------------------
void loop()
{
uint32_t currentTime = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature)) {
      Serial.println("Impossible de lire la temperature sur le DHT");
  } 
  else {
    if (currentTime - lastSend_temp > SEND_FREQUENCY_TEMP) {
      Serial.print("T: ");
      Serial.print(temperature);
      Serial.print(" | H: ");
      Serial.println(humidity);
      send(msgTemp.set(temperature, 1));
      send(msgHum.set(humidity, 1));
      lastSend_temp = currentTime;
    } 
  }  
}
