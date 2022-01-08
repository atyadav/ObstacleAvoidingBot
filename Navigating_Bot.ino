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
  7. Two IR Sensors (looking into right and left side)
*/

#include <Arduino_APDS9960.h> // Proximity, Colour 
#include <Arduino_LSM9DS1.h> // Gyroscope 

#define MOTOR_A1_PIN 2
#define MOTOR_A2_PIN 4
#define PWM_MOTOR_1 3

#define MOTOR_B1_PIN 7
#define MOTOR_B2_PIN 8
#define PWM_MOTOR_2 5

#define IR_LFT_PIN 9
#define IR_RHT_PIN 11


void setup() {
  Serial.begin(9600);
 // while (!Serial); // Wait for Serial Monitor to open

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
    while (true); // Stop forever
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // put your setup code here, to run once:
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT); 
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(IR_LFT_PIN, INPUT); 
  pinMode(IR_RHT_PIN, INPUT);
}

int proximity = 0;
int r = 0, g = 0, b = 0;
int Motor1_ENB = 200 , Motor2_ENB=200 ;
unsigned long lastUpdate = 0;

void loop() {
  Serial.print("PR=");
  Serial.println(proximity);
  
  Motor1_ENB=200;
  Motor2_ENB=200 ;
  
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(PWM_MOTOR_1, Motor1_ENB); //ENA pin
  analogWrite(PWM_MOTOR_2, Motor2_ENB); //ENB pin
 
  //}

 //Move Forward
 forward();


//Check if there is obstacle on left side, if Yes then turn right
  int leftObstacle = digitalRead(IR_LFT_PIN);
  int rightObstacle = digitalRead(IR_RHT_PIN);
  
  // print out the state of the button:
 
 if (!leftObstacle)
 {
    Serial.print("Left Obstacle - ");
    Serial.println(leftObstacle);
    stop();
    Serial.println("Taking right turn");
    right(200);
 }

//Check if there is obstacle on right side, if Yes then turn left
  if (!rightObstacle)
 {
    Serial.print("Right Obstacle - ");
    Serial.println(rightObstacle);
    stop();
    Serial.println("Taking Left turn");
    left(200);
 }

  // Check if a proximity reading is available.
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();
  }
  
  if ( proximity <200 )
  {
      analogWrite(PWM_MOTOR_1, proximity); //ENA pin
      analogWrite(PWM_MOTOR_2, proximity); //ENB pin
    
      // GO BACK for 2 Sec
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, HIGH);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);

      delay(500);

      stop();
          
      right(500);
    
      stop();
     
     }

      //Move Forward
      forward();
      

    if (APDS.proximityAvailable()) {
      proximity = APDS.readProximity();
    }

}

void stop()
{
      //Stop the Bot
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, LOW);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);
      delay(500);
}

  //void forward()
  //{
  //    //Check for gyroscope values to continue stright
  //     float temp = YValue();
  //     Serial.println(temp);
  //     if (temp < -5.0 )
  //     {
  //      Serial.println("Taking left Gyroscope");
  //      //left(100);
  //      Motor1_ENB = 150;
  //      
  //     }
  //     if(temp > 5.0)
  //     {
  //      Serial.println("Taking right Gyroscope");
  //      //right(100);
  //       Motor2_ENB = 150;
  //     } 
  //      
  //      //Stop the Bot
  //      digitalWrite(MOTOR_A1_PIN, HIGH);
  //      digitalWrite(MOTOR_A2_PIN, LOW);
  //    
  //      digitalWrite(MOTOR_B1_PIN, HIGH);
  //      digitalWrite(MOTOR_B2_PIN, LOW);
  //      delay(100);
  //      
  //      //Reset values 
  //      Motor1_ENB = 200;
  //      Motor2_ENB = 200;
  //}

void forward()
{
    //Check for gyroscope values to continue stright
     float fConst = - 0.9 ;
     float temp = YValue();
     Serial.println(temp);
     if (temp < -(1.0+fConst) )
     {
      Serial.println("Taking left Gyroscope");
      //left(100);
      //Motor2_ENB = 150;
      Motor2_ENB = Motor2_ENB + (temp*3);
      //Motor1_ENB = Motor1_ENB - (temp*3);
       Motor1_ENB = Motor1_ENB + (temp*3);
     }
     if(temp > (1.0+fConst))
     {
      Serial.println("Taking right Gyroscope");
      //right(100);
      // Motor1_ENB = 150;
       Motor1_ENB = Motor1_ENB + (temp*3);
       //Motor1_ENB = Motor1_ENB - (temp*3);
       Motor2_ENB = Motor2_ENB + (temp*3);
     } 
    
      //Controlling speed (0 = off and 255 = max speed):
      analogWrite(PWM_MOTOR_1, Motor1_ENB); //ENA pin
      analogWrite(PWM_MOTOR_2, Motor2_ENB); //ENB 
      
      //Stop the Bot
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_A2_PIN, LOW);
    
      digitalWrite(MOTOR_B1_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);
      delay(100);

}

void right(int tWait)
{
      //Turn Right 

      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_A2_PIN, HIGH);
    
      digitalWrite(MOTOR_B1_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);

      delay(tWait);
}

void left(int tWait)
{
      //Turn Left 

      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_A2_PIN, LOW);
    
      digitalWrite(MOTOR_B1_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);

      delay(tWait);
}

float YValue()
{
  float x,y,z;
  y=0;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    
    Serial.println("Gyroscope Y Value : ");
    Serial.println(y);
  
  }
    return y;
}
