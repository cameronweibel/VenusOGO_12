// This is the main controller program for the Venus rovers
// Group 12
// Cameron Weibel

// Included libraries
#include <Servo.h> 
#include <Wire.h>
#include <SPI.h>
#include <SFE_LSM9DS0.h>


// Servo declarations used on the rover
Servo servoLeft;
Servo servoRight;
Servo servoTurret;
Servo servoGrip;

// Sensing and movement values
int turn_val = 500;
int dist_sense = 30;
int dist_side = 50;

// Pin assignments for hardware components
int turretPin = 11;
int leftPin = 13;
int rightPin = 12;
int gripPin = 10;

// IR
const int numReadings = 10;

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

const int leftIR = A0;
const int rightIR = A1;

// IMU calibration values
long IMUmagCalX;
long IMUmagCalY;

// Onboard LED
int LEDPin = 13; 

// Initialize LSM9DS0 IMU
LSM9DS0 dof(MODE_I2C, 0x6B, 0x1D);

// Built-in initialization block
void setup()                                
{ 
  //Sets the baud rate for transmission
  Serial.begin(9600);
  
   // Initialize LSM9DS0 IMU
  uint16_t status = dof.begin();
  
  // Start magnetometer calibration
  magCalibration(&IMUmagCalX, &IMUmagCalY);
  Serial.println("Begin measurements");
  
  delay(1000);
}  

//Loop must be present, otherwise program will not run
void loop(){   
  //Gerben drive
  gripClose();
  lookStraight();
  
  //100 is a placeholder, doesn't do anything
  left(100);
  right(100);

  
}

void mainMethod(){
   //Scan first and turn into "perceived" rock
  while (ping(0,9) > dist_sense)
  {
    Serial.println(IR(leftIR));
    if(IR(leftIR)==false && IR(rightIR)==false)
    {
         if (sidePing(2,3)<dist_side && sidePing(2,3) != -1)
  {
    stop();
    lookLeft();
    if (mountain(ping(0,9), sidePing(2,3))) // this is a mountain
    {
      lookStraight();
      forward();
      delay(1000);
    }else                                 // this is a rock
    {
      stop();          
      left(turn_val);
      lookStraight();
      gripOpen();
      forward();
    } 
  }else                             // nothing is detected by the side ultrasonic sensor
  {
    forward();
  }
    }
    else                         // there is black tape
    {
      stop();
      backward(turn_val);
      right(turn_val);
    }
  }
  
  stop();
  lookRight();
  if (ping(0,9) < dist_sense)
  {
    lookLeft(); 
    if (ping(0,9) < dist_sense)
    {
      backward(turn_val);
      right(turn_val); 
    }
    else{
      left(turn_val);
    }
  }
  else{
    right(turn_val);  
      }
  //Detach gripper servo to avoid mis-writes
  //servoGrip.detach(); 
  
}

//STEERING
//The methods below that control steering, drive for a specified duration time                                    
void left(int time)                          
{
 //maneuver(-200,200,time); 
 turn(270);
}

void right(int time)                         
{
 //maneuver(200,-200,time); 
 turn(90);
}

void forward()                        
{
  
  //Attach both servos to their pins
  servoLeft.attach(leftPin);
  servoRight.attach(rightPin);
  
  // speedLeft, speedRight ranges: Backward(-200)  Linear(-100)  Stop(0)  Linear(100)   Forward(200)
  // Set right and left servo speed
  servoLeft.writeMicroseconds(1300);   
  servoRight.writeMicroseconds(1700);
}

void stop(){
  //Stops the driving
  servoLeft.writeMicroseconds(1500);   
  servoRight.writeMicroseconds(1500);
  
  //Detach components to avoid mis-writes
  servoLeft.detach();  
  servoRight.detach(); 
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
  
}

//Opens/closes the gripper
void gripOpen(){
  gripper(90,500); 
  
  //Detach gripper servo to avoid mis-writes
  servoGrip.detach(); 
}

void gripClose(){
  gripper(130,10); 
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

// Side ping
unsigned long sidePing(int echoPin, int trigPin)
{
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
  long duration, distance; // Duration used to calculate distance
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
  /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 distance =-1;
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 //Serial.println(distance);
 digitalWrite(LEDPin, LOW); 
 }
 
 return distance;
 //Delay 50ms before next reading.
 delay(50);
}


//Turret ping
unsigned long ping(int echo, int ultraSoundSignal)
{
  pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
  digitalWrite(ultraSoundSignal, LOW); // Send low pulse
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
  delayMicroseconds(5); // Wait for 5 microseconds
  digitalWrite(ultraSoundSignal, LOW); // Holdoff
  pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
  digitalWrite(ultraSoundSignal, HIGH); // Turn on pullup resistor
  // please note that pulseIn has a 1sec timeout, which may
  // not be desirable. Depending on your sensor specs, you
  // can likely bound the time like this -- marcmerlin
  // echo = pulseIn(ultraSoundSignal, HIGH, 38000)
  echo = pulseIn(ultraSoundSignal, HIGH); //Listen for echo
  unsigned long ultrasoundValue = (echo / 58.138); //convert to CM then to inches
  return ultrasoundValue;
}

//IR left and right
boolean IR(const int pin) {
  
  int total = 0;
  for(int i=0;i<20;i++){
   total += analogRead(pin); 
  }  
  int average = total/19;
  int sensorValue = analogRead(pin);
  if (average <= 1000) {
    return false;
  }
  else {
    return true;
  }
  delay(10);
}

// Detection of moutain
boolean mountain (int topDist, int bottomDist)
{
  boolean ifMountain;
  if (topDist - bottomDist < 10)
  {
    ifMountain = true;
  }
  else
  {
    ifMountain = false;
  }
  return ifMountain;
}

void turn(float degrees) {
  
  // Minimum and maximum turning speed
  int a = 5;
  int b = 100;
  
  // Maximum time used for turning in milliseconds
  int maxTime = 5000;
  
  // Accuracy in degrees
  float accuracy = 2.0;
  
  // Prepare servos
  servoLeft.attach(leftPin);
  servoRight.attach(rightPin);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  
  // Determine goal heading relative to the north
  float goalHeading = heading() + degrees;
  
  // Normalize goal heading into the interval [0,360>
  while(goalHeading >= 360)
    goalHeading -= 360;
  while(goalHeading < 0)
    goalHeading += 360;
  
  float difference = degrees;
  int turningSpeed;
  
  long timer = millis();
  
  // Loop until the goal heading has been reached with an accuracy of 1.0 degree
  while(abs(difference) > accuracy && millis() < timer + maxTime) {
    
    // Normalize difference to interval <-180, 180]
    while(difference > 180.0)
      difference -= 360.0;
    while(difference <= -180.0)
      difference += 360.0;
    
    turningSpeed = (difference/abs(difference))*(((b*abs(difference))/181)+a);
    
    servoLeft.writeMicroseconds(1500 + turningSpeed);
    servoRight.writeMicroseconds(1500 + turningSpeed);
    
    //delay(100);
    
    // Determine new difference
    difference = heading() - goalHeading;
  }
  
  // Stop and detach the servos
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  servoLeft.detach();  
  servoRight.detach();
  
}

float heading() {
  
  // Perform an average measurement over 10 measurements
  long x = 0;
  long y = 0;
  
  for(int i = 0; i < 10; i++) {
    dof.readMag();
    x += dof.mx/10;
    y += dof.my/10;
  }
  
  // Return the atan2 of the x and y values converted from radians to degrees
  return atan2(x - IMUmagCalX,y - IMUmagCalY)*(180/PI);
}

void magCalibration(long *IMUmagCalX, long *IMUmagCalY) {
  
  // Define minimum and maximum values for x and y
  long xmin = 1000000000;
  long xmax = -1000000000;
  long ymin = 1000000000;
  long ymax = -1000000000;
    
  // Keep a timer
  long timer = millis();
  long x, y;
  
  // Prepare servos
  servoLeft.attach(leftPin);
  servoRight.attach(rightPin);
  
  // Turn around
  servoLeft.writeMicroseconds(1500 + 20);
  servoRight.writeMicroseconds(1500 + 20);
  
  // Loop for 30000 milliseconds (30 seconds)
  while(millis() - timer < 30000) {
    
    // Perform an average reading over 10 different readings
    x = 0;
    y = 0;
    
    for(int i = 0; i < 10; i++) {
      dof.readMag();
      x += dof.mx/10;
      y += dof.my/10;
    }
    
    // If some of the values are outside the presumed boundaries, push the boundary
    if(x < xmin)
      xmin = dof.mx;
    if(x > xmax)
      xmax = dof.mx;
    if(y < ymin)
      ymin = dof.my;
    if(y > ymax)
      ymax = dof.my;
  }
  
  // Stop and detach the servos
  servoLeft.writeMicroseconds(1500);   
  servoRight.writeMicroseconds(1500);
  servoLeft.detach();  
  servoRight.detach();
  
  // Define the calibration values and put them into the adresses
  *IMUmagCalX = (xmin+xmax)*0.5;
  *IMUmagCalY = (ymin+ymax)*0.5;
}
