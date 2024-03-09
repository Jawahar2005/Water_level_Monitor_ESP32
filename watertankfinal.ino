#define BLYNK_TEMPLATE_ID "TMPL3ED_Ge25m"
#define BLYNK_TEMPLATE_NAME "water level controller"
#define BLYNK_AUTH_TOKEN "NFaQ4IdI7BQuue-75-b4Nw3X_M6Ph80_"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>                         
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Arul -2.5";
char pass[] = "Arul@123";

const int trigPin = 12; // trigger connected to D6(GPIO12)
const int echoPin = 14; // echo connected to D5(GPIO14)

#define SOUND_VELOCITY 0.034  //define sound velocity in cm/uS

BlynkTimer timer; 
long duration;   //Time taken by the sound.
int distanceCm;   //Distance measured by the ultrasonic sensor.
int height = 16; //declaring the height of the tank in cm.
int emptyDist = 4; //Empty space in the tank.(not to be measured.)

float level = 0;  //The water level in the tank. 
float distWaterLevel =0; //Ultrasonic distance to correct water level

void myTimerEvent()
{
  Blynk.virtualWrite(V0, level);// Sending value to the blynk 
  
}


void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(5,OUTPUT);     //Receiving command from blynk(D1)-GPIO5

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); //Connecting to the blynk.
  timer.setInterval(1000L, myTimerEvent); // Setup a function to be called every second
}


BLYNK_WRITE(V1)           //switch in blynk                      
{
  if(param.asInt() == 1) // assigning incoming value from pin V3 to a variable
  {
    digitalWrite(5, HIGH);
  }
  else if(param.asInt() == 0)
  {
    digitalWrite(5,LOW);
  }
  Serial.print("the serial value is:");
  Serial.println(param.asInt());
  
}


void loop() {
  digitalWrite(trigPin, LOW);// Clears the trigPin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distanceCm = duration * SOUND_VELOCITY/2;// Calculate the distance
  distWaterLevel = distanceCm - emptyDist; //Maximum level of full tank (water to be filled)
  
  level = (height-emptyDist)-distWaterLevel;  //Level of the tank.

  if (level<0)                      //If is it negative(Rare condition)
  level=level*-1;

  level = (level/(height-emptyDist))*100;   //In percentage

  if(level<15)
  {
    Blynk.logEvent("water_level_warning");
    //digitalWrite(5,HIGH);
    //Blynk.virtualWrite(V1,HIGH);
  }
  if(level>95)
  {
    Blynk.logEvent("water_level_max");
    //digitalWrite(5,LOW);
    //Blynk.virtualWrite(V1,LOW);
  }  

  Serial.print("Distance (cm): "); // Prints the distance on the Serial Monitor
  Serial.println(distanceCm);
  Serial.print("Percentage;");
  Serial.println(level);
  delay(1000);
  Blynk.run();
  timer.run(); -
}



