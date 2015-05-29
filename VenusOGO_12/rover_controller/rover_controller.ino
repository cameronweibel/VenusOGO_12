//This is the main controller program for the Venus rovers

#include <Servo.h>                           // Include servo library
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
Servo servoTurret;                           //Declare ultrasonic sensor eyes 
Servo servoGrip;                             //Declare the gripper

void setup()                                 // Built-in initialization block
{ 
  Serial.begin(9600);
  
  servoTurret.attach(11);                    // Attach turret signal to pin 11 
  servoGrip.attach(10);                      // Attach grip signal to pin 10
  
  delay(1000);
  lookRight();
  delay(1000);
  lookLeft();
  delay(1000);
  lookStraight();
  delay(1000);
  
  servoTurret.detach();
  
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  
  forward(2000);                             //testing
  backward(2000);
  right(2000);
  left(2000);  
  
  detacher();
  
  gripClose();
  gripOpen();
  
  servoGrip.detach();                      // Attach grip signal to pin 10
  
  
}  

void loop(){
  
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

//GRIPPING

void gripper(int degree, int msTime)       //control of the movement
{
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoGrip.write(degree);   // Set servo speed
  if(msTime==-1)                                   // if msTime = -1
  {                                  
    servoGrip.detach();                            // Stop servo signals
  }
  delay(msTime);                                   // Delay for msTime
}

void gripOpen(){
  gripper(90,2000); 
}

void gripClose(){
  gripper(10,2000); 
}

//DETACH

void detacher(){
  servoLeft.detach();                      // Attach left signal to pin 13
  servoRight.detach();                     // Attach right signal to pin 12
  //servoTurret.detach(11);                    // Attach turret signal to pin 11 
}

//LOOK LEFT AND RIGHT
void lookRight()       //control of the movement
{
  servoTurret.write(10);   // Set servo speed
  delay(1000);                                   // Delay for msTime
}

void lookLeft()       //control of the movement
{
  servoTurret.write(180);   // Set servo speed
  delay(1000);                                   // Delay for msTime
}

void lookStraight()       //control of the movement
{
  servoTurret.write(90);   // Set servo speed
  delay(1000);                                   // Delay for msTime
}

