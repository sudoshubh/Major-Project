//Code that runs on Node MCU
//Code that Connects Node MCU to Internet and Send Sensors data to Web Server
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


/* Set these to your desired credentials. */
const char *ssid = "XXXXX";  //ENTER YOUR WIFI SETTINGS
const char *password = "XXXXXX";

//Web/Server address to read/write from 
const char *host = "http://XXXXXXXX/accept.php"; //mention your domain name & directory of accept.php
  
void setup() {
 delay(1000);
 Serial.begin(9600);
 WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

String buff = String("");

void resetBuffer() {
  buff = String("");
}

void sendDataToServer(String data) {
    Serial.print(data);
    Serial.println(".....END.....");
//    resetBuffer();
   if (1) {
    HTTPClient http;    //Declare object of class   HTTPClient

  String ADCData, station, getData, Link;
  //int adcvalue=analogRead(A0);  //Read Analog value of LDR
  //ADCData = String(adcvalue);   //String to interger conversion
  station = "B";

  //GET Data
  getData = "?sensor=" + data.substring(0, data.length() - 1); //phone kat gaya  //Note "?" added at front,
  Link = "http://learnedge.co.in/iot/accept.php" + getData; 
  resetBuffer();
  
  http.begin(Link);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return 
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}
  
}
void loop() {
  int i = 0;
  while(Serial.available()) {
    char str = Serial.read();
    if( str == 10 ) {
      sendDataToServer(buff);
    } else {
      buff = buff + str;
    }

 delay(2000);

}
