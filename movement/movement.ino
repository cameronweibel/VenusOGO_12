#include <Servo.h>                           // Include servo library
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
Servo servoTurret;                           //Declare ultrasonic sensor eyes 
Servo servoGrip;                             //Declare the gripper

// Sequence of turret positions.
int sequence[] = {-20, 0, 20, 40, 60, 80, 100, 120, 140, 160};
// Ultrasonic Sensor
const int pingPin = 9;
// Pre-calculate degrees per adjustment of turret servo. 
 
void setup()                                 // Built-in initialization block
{ 
  Serial.begin(9600);
  
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoTurret.attach(11);                    // Attach turret signal to pin 11 
  servoGrip.attach(10);                      // Attach grip signal to pin 10
  
  //forward(3000);                             //testing
   left(2000);
  //backward(2000);
  //right(2000);   
  //turret(sequence[9]);                       //ultrasonic sensor turn
  //turret(sequence[0]); 
  //grip(1500);                                    //grip the rock sample
  //grip(680);                                       //1500 open and 680 close
  
  maneuver(0, 0, -1);                        // Disable servos
}  

                                             
void left(int time)                          // turn left
{
 maneuver(-200,200,time); 
}

void right(int time)                         //turn right
{
 maneuver(200,-200,time); 
}

void forward(int time)                        //forward
{
 maneuver(-200,-200,time); 
}

void backward(int time)                       //backward
{
 maneuver(200,200,time); 
}
 
/*
 * Position the horn of a Parallax Standard Servo
 * Parameter: degreeVal in a range from 90 to -90 degrees. 
 */ 
void turret(int degreeVal)                    //ultrasonic sensor eyes
{
  servoTurret.writeMicroseconds(700+(degreeVal*10)); 
}

void grip (int state)                                  //grip rock sample
{
  servoGrip.writeMicroseconds(state); 
}

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
 
void loop()                                  // Main loop auto-repeats
{                     
                                             // Empty, nothing needs  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(100);

  if(cm < 30)
  {
    backward(2000);
  }
}

//Ultrasonic Sensor 
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}







