#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
 
void setup()                                 // Built-in initialization block
{
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  
  forward(2000);
  left(2000);
  backward(2000);
  right(2000);
  
  maneuver(0, 0, -1);                        // Disable servos
}  


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
 
void maneuver(int speedLeft, int speedRight, int msTime)
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
 
void loop()                                  // Main loop auto-repeats
{                                            // Empty, nothing needs repeating
  maneuver(200, 200, 2000);                  // Forward 2 seconds
}




