//This is the main controller program for the Venus rovers
//Group 12
//Cameron Weibel

//Included libraries
#include <Servo.h>                           

//Servo declarations used on the rover
Servo servoLeft;
Servo servoRight;
Servo servoTurret;
Servo servoGrip;

//Pin assignments for hardware components
int turretPin = 11;
int leftPin = 13;
int rightPin = 12;
int gripPin = 10;

// Built-in initialization block
void setup()                                
{ 
  //Sets the baud rate for transmission
  Serial.begin(9600);
}  

//Loop must be present, otherwise program wll not run
void loop(){   
  //Test run
  delay(1000);
  lookRight();
  delay(1000);
  lookLeft();
  delay(1000);
  lookStraight();
  delay(1000);
  
  forward(2000);
  backward(2000);
  right(2000);
  left(2000);  

  gripClose();
  gripOpen();
}

//STEERING
//The methods below that control steering, drive for a specified duration time                                    
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
void maneuver(int speedLeft, int speedRight, int msTime)
{
  //Attach both servos to their pins
  servoLeft.attach(leftPin);
  servoRight.attach(rightPin);
  
  // speedLeft, speedRight ranges: Backward(-200)  Linear(-100)  Stop(0)  Linear(100)   Forward(200)
  // Set right and left servo speed
  servoLeft.writeMicroseconds(1500 + speedLeft);   
  servoRight.writeMicroseconds(1500 - speedRight);
  
  // Delay for msTime
  delay(msTime);   

  //Detach components to avoid mis-writes
  servoLeft.detach();  
  servoRight.detach(); 
}

//GRIPPING
//Opens and closes gripper a certain degree and delays for msTime
void gripper(int degree, int msTime)
{
  //Attach gripper servo to its pin
  servoGrip.attach(gripPin);
  
  //Rotate the servo a certain degree, which corresponds to open(90) and closed(20)
  servoGrip.write(degree);
  
  // Delay for msTime
  delay(msTime); 

  //Detach gripper servo to avoid mis-writes
  servoGrip.detach(); 
  
}

//Opens/closes the gripper
void gripOpen(){
  gripper(90,2000); 
}

void gripClose(){
  gripper(10,2000); 
}

//LOOKING LEFT AND RIGHT
void lookRight()
{
  //Attaches turret to its pin
  servoTurret.attach(turretPin);
  
  
  // Set servo degree
  servoTurret.write(10);   
  
  // Delay for msTime
  delay(1000);  

  //Detach turret servo to avoid mis-writes
  servoTurret.detach();  
}

void lookLeft()
{
  //Attaches turret to its pin
  servoTurret.attach(turretPin);
  
  // Set servo degree
  servoTurret.write(180);   // Set servo speed
  
  // Delay for msTime
  delay(1000);
  
  //Detach turret servo to avoid mis-writes
  servoTurret.detach();  
}

void lookStraight()
{
  //Attaches turret to its pin
  servoTurret.attach(turretPin);
  
  // Set servo degree
  servoTurret.write(90);
  
  // Delay for msTime
  delay(1000);
  
  //Detach turret servo to avoid mis-writes
  servoTurret.detach();  
}

