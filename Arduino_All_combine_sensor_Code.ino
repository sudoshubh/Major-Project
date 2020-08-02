/*
 * ALL IN ONE CODE
 */

// HEADER FILES
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// GPS
SoftwareSerial GPS_SoftSerial(4, 3);//(TX,RX)
TinyGPSPlus gps;
volatile float minutes, seconds;
volatile int degree, secs, mins;
double lat_val, lng_val, alt_m_val;

// FLEXI FORCE
#define FLEXI_PIN A5  
int flexi_input = 0;

// VIBRATION SENSOR
#define VIBRATION_PIN A3
float vibration_input = 0;
float old_vibration_input = 0;
float vibration_diff = 0;

// COLLISION SENSOR
#define COLLISION_PIN A0
int collision_input = 0;


// GSM
SoftwareSerial mySerial(9, 10);//(TX,RX)
// #define MYNUMBER 
 
void setup() {
  Serial.begin(9600);

  // FLEXI FORCE
  pinMode(FLEXI_PIN, INPUT);

  // VIBRATION SENSOR
  pinMode(VIBRATION_PIN, INPUT);

  // COLLISION SENSOR
  pinMode(COLLISION_PIN, INPUT);

  // GSM
  mySerial.begin(9600);

  // GPS
  GPS_SoftSerial.begin(9600);
}

// FLEXI FORCE
float flexiForce() {
  flexi_input = analogRead(FLEXI_PIN);
//  Serial.print("Flexi Force Value: ");
//  Serial.println(flexi_input);
  Serial.print("F");
  Serial.print(":input:");
  Serial.print(flexi_input);
  Serial.print("::");
  return flexi_input;
}

// VIVRATION SENSOR
int vibrationSensor( int threshold ) {
  old_vibration_input = vibration_input;
  vibration_input = analogRead(VIBRATION_PIN);
  vibration_diff = abs(old_vibration_input - vibration_input);
  
 /* Serial.println("Vibration Sensor: ");
 Serial.print("OLD: ");
 Serial.print(old_vibration_input);
 Serial.print("    NEW: ");
 Serial.print(vibration_input);
 Serial.print("    DIFF: ");
 Serial.println(vibration_diff); */

  Serial.print("V");
  Serial.print(":old:");
  Serial.print(old_vibration_input);
  Serial.print(":new:");
  Serial.print(vibration_input);
  Serial.print(":diff:");
  Serial.print(vibration_diff);
  Serial.print("::");

  if ( vibration_diff >= threshold ) {
    return 1;
  }
  return 0;
  
}

// COLLISION SENSOR
int collisionSensor() {
  collision_input = digitalRead(COLLISION_PIN);
  Serial.print("C");
  Serial.print(":value:");
  Serial.print(collision_input);
  Serial.print("::");
  //Serial.print("Collision Sensor: ");
//  Serial.println(collision_input);
  return collision_input;
}


// GSM MODULE////
void sendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919917591857\"\r"); // Replace x with mobile number
  delay(1000);
  
   mySerial.println("Alert :\n Vehical no : UP21 XX XXXX\n");// The SMS text you want to send
   mySerial.println("ACCIDENT OCCURED AT -\nLOCATION :"+ String(lat_val)+" LONGITUDE :"+ String(lng_val));// The SMS text you want to send
  delay(100);
//  Serial.print("MESSAGE SENT");
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

// GPS

void gpsSensor(){
  smartDelay(1000);
  unsigned long start;
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat(); 
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();
  alt_m_val = gps.altitude.meters(); 
  alt_valid = gps.altitude.isValid(); 

/* Serial.print("GPS");
 Serial.print(":valid:");
 Serial.print(loc_valid);
 Serial.print(":LAT:");
 Serial.print(lat_val);
 Serial.print(":LONG:");
 Serial.print(lng_val);
 Serial.print("::"); */
  
  if (!loc_valid)
  {
//    Serial.print("Latitude : *****");
//    Serial.print("Longitude : *****");
    //delay(4000);
  }
  else
  {
    
  //  Serial.println("GPS READING: ");
    DegMinSec(lat_val);
  //  Serial.print("Latitude in Decimal Degrees : ");
  //  Serial.println(lat_val, 6);

    DegMinSec(lng_val); 
   // Serial.print("Longitude in Decimal Degrees : ");
   // Serial.println(lng_val, 6);
    //delay(4000);
    Serial.print("GPS");
    Serial.print(":valid:");
    Serial.print(loc_valid);
    Serial.print(":LAT:");
    Serial.print(lat_val,6);
    Serial.print(":LONG:");
    Serial.print(lng_val,6);
    Serial.print("::");
  }  
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available()) 
    gps.encode(GPS_SoftSerial.read());
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}



void loop() {
  vibrationSensor(3);
  flexiForce();
  collisionSensor();
  gpsSensor();
  Serial.println();
/*flexiForce();
  
 if ( vibrationSensor(3) ) 
 {    if ( flexiForce() > 100 ) {
      if ( collisionSensor() == 0 ) {
        gpsSensor();
        //Serial.println("############################### ACCIDENT #################################");
        sendMessage();
        delay(500);
      }
    }
  }*/
  delay(2000);
}
