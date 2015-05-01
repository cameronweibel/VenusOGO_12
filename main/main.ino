//This is the main controller program for the Venus rovers

#include <Servo.h>                           // Include servo library
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
Servo servoTurret;                           //Declare ultrasonic sensor eyes 
Servo servoGrip;                             //Declare the gripper

void setup()                                 // Built-in initialization block
{ 
  Serial.begin(9600);
  
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoTurret.attach(11);                    // Attach turret signal to pin 11 
  //servoGrip.attach(10);                      // Attach grip signal to pin 10
  
  forward(3000);                             //testing
  left(2000);
  //backward(2000);
  //right(2000);   
  maneuver(0, 0, -1);                        // Disable servos
}  


void loop() {
  // put your main code here, to run repeatedly:
}

//STEERING
                                            
void left(int time)                          
{
 maneuver(-200,200,time); 
}

void right(int time)                         
{
 maneuver(200,-200,time); 
}

void forward(int time)                        
{
 maneuver(-200,-200,time); 
}

void backward(int time)                      
{
 maneuver(200,200,time); 
}

//Servo driver for the steering

void maneuver(int speedLeft, int speedRight, int msTime)       //control of the movement
{
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoLeft.writeMicroseconds(1500 + speedLeft);   // Set Left servo speed
  servoRight.writeMicroseconds(1500 - speedRight); // Set right servo speed
  if(msTime==-1)                                   // if msTime = -1
  {                                  
    servoLeft.detach();                            // Stop servo signals
    servoRight.detach();   
  }
  delay(msTime);                                   // Delay for msTime
}
