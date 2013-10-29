/*#######################################################
#
# Hit the Heat Arduino node module
#
# Maciej Lasyk
# http://maciek.lasyk.info
# Twitter: @docent_net
#
# https://github.com/docent-net/HitTheHeat 
#
#########################################################
# Copyright 2013 Maciej Lasyk
#
#        Licensed under the Apache License, Version 2.0 (the "License");
#        you may not use this file except in compliance with the License.
#        You may obtain a copy of the License at
#
#                http://www.apache.org/licenses/LICENSE-2.0
#
#        Unless required by applicable law or agreed to in writing, software
#        distributed under the License is distributed on an "AS IS" BASIS,
#        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#        See the License for the specific language governing permissions and
#        limitations under the License.
#######################################################*/

#include <SPI.h>
#include <Ethernet.h>

/*#######################################################
#
# configuration
#
# loop_delay: [ms] - how often should we refresh & send
#                     temperature to webserver?
# debug_on [false|true] - set to true to see dbg info 
#                         on console
# mac - MAC address that will be binded to Arduino
# ip - IP address that will be binded to Arduino
# server - webserver's IP address
#
#######################################################*/
long loop_delay = 5000;
bool debug_on = true;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xCC, 0x02 };  
byte ip[] = { 192, 168, 0, 2 };   
byte server[] = { 192, 168, 0, 1 };

/*#######################################################
#
# logic section below
#
#######################################################*/

float temp;

EthernetClient client;
long previousMillis = 0;
String http_req="";
static char temp_char[5];

void setup() {
  Serial.begin(9600);
  
  if(debug_on) Serial.println("initializing...");
  
  Ethernet.begin(mac, ip);
};

void loop () {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > loop_delay)
  {
    previousMillis = currentMillis;   
    
    if(debug_on) Serial.println("connecting to server...");
  
    if (client.connect(server, 80)) 
    {
      temp = readTemp();
      dtostrf(temp, 5, 2, temp_char);
    
      http_req = "GET /temp.py?t=";
      http_req += temp_char;
      http_req += " HTTP/1.0";
    
      if(debug_on) Serial.println(http_req);
      if(debug_on) Serial.println("connected");

      client.println(http_req);
      client.println();
      
      if(debug_on) Serial.println("sleeping until next loop...");
      client.stop();   
    } 
  }
};

float readTemp()
{
  if(debug_on) Serial.println("reading temp...");
  temp = analogRead(0)*5/1024.0;
  temp = temp - 0.5;
  temp = temp / 0.01;
  if(debug_on) Serial.print("T: ");
  if(debug_on) Serial.println(temp);
  return temp;
}
