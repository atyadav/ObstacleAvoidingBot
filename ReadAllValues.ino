/*
  APDS-9960 sensor reads any obstacle and accordingly bot would move.
  In case of obstacle bot moves backword and then turn right.
  This action continues
  Atul Yadav 28 Dec 2021

  Material - 
  1. Arduino Nano Sense BLE
  2. L298N Motor Driver
  3. 5 Volt regulator IC 7805 - Custom PCB 
  4. Two 9 Volts battery holders
  5. Jumper Wires- F-F (8) , M-F (1)
  6. Two BO Motors with Wheels
*/

#include <Arduino_APDS9960.h>

#define MOTOR_A1_PIN 2
#define MOTOR_A2_PIN 4
#define PWM_MOTOR_1 3

#define MOTOR_B1_PIN 7
#define MOTOR_B2_PIN 8
#define PWM_MOTOR_2 5

void stop()
{
      //Stop the Bot
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, LOW);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
      delay(500);
}

void forward()
{
      //Stop the Bot
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_A2_PIN, LOW);
    
      digitalWrite(MOTOR_B1_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);
      delay(100);
}
void setup() {
  Serial.begin(9600);
 // while (!Serial); // Wait for Serial Monitor to open

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
    while (true); // Stop forever
  }

  // put your setup code here, to run once:
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT); 
  pinMode(PWM_MOTOR_2, OUTPUT);
}

int proximity = 0;
int r = 0, g = 0, b = 0;
unsigned long lastUpdate = 0;

void loop() {

  // Check if a proximity reading is available.
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();
  }
  // Print updates every 100 ms
  //if (millis() - lastUpdate > 100) {
   // lastUpdate = millis();
    Serial.print("PR=");
    Serial.println(proximity);

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(PWM_MOTOR_1, proximity-50); //ENA pin
  analogWrite(PWM_MOTOR_2, proximity-50); //ENB pin
 
  //}

 
  if ( proximity <200 )
  {
      analogWrite(PWM_MOTOR_1, 150); //ENA pin
      analogWrite(PWM_MOTOR_2, 150); //ENB pin
    
      // GO BACK for 2 Sec
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, HIGH);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);

      delay(750);

      stop();
          
      //Turn Right 

      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, HIGH);
    
      digitalWrite(MOTOR_B1_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);

      delay(500);
    
      stop();
     
     }

      //Move Forward
      forward();
      

    if (APDS.proximityAvailable()) {
      proximity = APDS.readProximity();
    }


}
