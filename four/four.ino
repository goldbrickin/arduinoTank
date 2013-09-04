#include <Ping.h>
#include <Wire.h>
#include <Servo.h>
#include <LSM303.h>
#include <AFMotor.h>

#define BAUDRATE 9600
#define PINGPIN 11
#define SERVOPIN 9
#define LEFTTURN 1
#define RIGHTTURN 0

AF_DCMotor motorL(3);
AF_DCMotor motorR(4);

LSM303 compass;

Servo servo;
Ping ping = Ping(PINGPIN,0,0);

int rightInterruptPin = 0;
int leftInterruptPin = 1;

volatile int state = LOW;

char val = ' ';
char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character

int centerHeading, currentHeading;
int interruptCounterLeft, interruptCounterRight;
boolean calibrateVisibility = false;
int interruptInch = 50;

void setup() {
  // open the serial communication on 9600 baudrate
  Serial.begin(BAUDRATE);
  Wire.begin();
  servo.attach(SERVOPIN);
  compass.init();
  compass.enableDefault();
  attachInterrupt(rightInterruptPin, countRight, CHANGE);
  attachInterrupt(leftInterruptPin, countLeft, CHANGE);
  
  // Calibration values. Use the Calibrate example program to get the values.
  compass.m_min.x = -327; compass.m_min.y = -806; compass.m_min.z = -49;
  compass.m_max.x = 577; compass.m_max.y = 227; compass.m_max.z = 891;
  
  motorL.setSpeed(255);
  motorR.setSpeed(255);
  
  motorL.run(RELEASE);
  motorR.run(RELEASE);
  
  interruptCounterRight = 0;
  interruptCounterLeft = 0;
}
 
void loop() {
  if (Serial.available()) {
    val = Serial.read();
    if (val == 'd') {
      getDistance('i');
    } else if (val == 'h') {
      getHeading();
    } else if (val == 'r') {
      look('r');
    } else if (val == 'l') {
      look('l');
    } else if (val == 'c') {
      look('c');
    } else if (val == 'f') {
      go('f', getIntSerialParameter());
    } else if (val == 'b') {
      go('b', getIntSerialParameter());
    } else if (val == ',') {
      turn(LEFTTURN, getIntSerialParameter());
    } else if (val == '.') {
      turn(RIGHTTURN, getIntSerialParameter());
    } else if (val == '1') {
      moveTrack('l');
    } else if (val == '2') {
      moveTrack('r');
    } 
  }
}

void countRight()
{
  interruptCounterRight++;
}

void countLeft()
{
  interruptCounterLeft++;
}

void getHeading() {
  compass.read();
  int heading = compass.heading((LSM303::vector){0,-1,0});
  currentHeading = heading;
  //Serial.println(heading);
  delay(10);
}

void look(char dir)
{
  if (dir == 'r')
  {
    servo.write(170);
  }
  else if (dir == 'l')
  {
    servo.write(10);
  }
  else
  {
    servo.write(90);
  }
}

void turn(int turnDirection, float turnAngle)
{
  getHeading();
  
  float targetAngle, angleToleranceLow, angleToleranceHigh;

  switch(turnDirection) {
    case RIGHTTURN:
      targetAngle = adjustedHeading(currentHeading, turnAngle);
      angleToleranceLow = adjustedHeading(targetAngle, -2);
      angleToleranceHigh = adjustedHeading(targetAngle, 15);
      break;
    case LEFTTURN:
      targetAngle = adjustedHeading(currentHeading, -turnAngle);
      angleToleranceLow = adjustedHeading(targetAngle, -15);
      angleToleranceHigh = adjustedHeading(targetAngle, 2);
      break;
  }
  
  /*
  Serial.print("Low Range: ");
  Serial.println(angleToleranceLow);
  Serial.print("High Range: ");
  Serial.println(angleToleranceHigh);
  */
  
  while (!isBetweenAngles(currentHeading, angleToleranceLow, angleToleranceHigh))
  {
    //Serial.print("Compass: ");
    //Serial.println(currentCompass);
    if (turnDirection == RIGHTTURN)
    {
      motorL.run(FORWARD);
      motorR.run(BACKWARD);
    }
    else
    {
      motorL.run(BACKWARD);
      motorR.run(FORWARD);
    }
    delay(100);
    motorL.run(RELEASE);
    motorR.run(RELEASE); 
    getHeading();  
  }
}

void resetInterrupt(char which)
{
  if (which == 'r')
  {
    interruptCounterRight = 0;
  }
  else if (which == 'l')
  {
    interruptCounterLeft = 0;
  }
  else
  {
    interruptCounterRight = 0;
    interruptCounterLeft = 0;
  }
}

void go(char way, int howFar)
{
  resetInterrupt('b');
  int startVisibility = (int)getDistance('i');
  
  while (interruptCounterRight < (howFar * interruptInch))
  {
    if (way == 'b') 
    {  
      motorL.run(BACKWARD);
      motorR.run(BACKWARD);
    }
    else
    {
      motorL.run(FORWARD);
      motorR.run(FORWARD);
    }
    
    delay(10);
  
    motorL.run(RELEASE);
    motorR.run(RELEASE); 
  }
  
  int finishVisibility = (int)getDistance('i');
  
  if (calibrateVisibility == true)
  {
    if (way == 'b')
    {
      interruptInch = interruptCounterRight / (finishVisibility - startVisibility);
    }
    else
    {
      interruptInch = interruptCounterRight / (startVisibility - finishVisibility);
    }
    
    calibrateVisibility = false;
  }
    
  Serial.print("Interrupts:  ");
  Serial.println(interruptCounterRight);
    
  Serial.print("Distance calculated:  ");
  Serial.println(howFar);
  
  Serial.print("Adjusted interrupt per inch:  ");
  Serial.println(interruptInch);
  
  resetInterrupt('b');
}

void moveTrack(char leftOrRight)
{
  if (leftOrRight == 'r')
  {
    motorR.run(FORWARD);
    delay(1000);
    motorR.run(RELEASE);
  }
  else
  {
    motorL.run(FORWARD);
    delay(1000);
    motorL.run(RELEASE);
  }
}

int adjustedHeading(int currentHeading, int adjustment)
{
  int returnHeading = currentHeading + adjustment;
  
  if (returnHeading > 360)
  {
    returnHeading = adjustment - (360 - currentHeading);
  }
  else if (returnHeading < 0)
  {
    returnHeading = 360 + (currentHeading + adjustment);
  }  
  return returnHeading;
}

float getDistance(char unit)
{
  float distance;
  ping.fire();
  
  switch(unit) {
    case 'i':
      Serial.print("Visibility (in): ");
      distance = ping.inches();
      break;
    case 'c':
      Serial.print("Visibility (cm): ");
      distance = ping.centimeters();
      break;
    case 'm':
      Serial.print("Visibility (ms): ");
      distance = ping.microseconds();
      break;
  }
  Serial.println(distance);
  return distance;
}

boolean isBetweenAngles(float testAngle, float lowAngle, float highAngle)
{
  if (lowAngle < highAngle)
  {
    if (testAngle > lowAngle && testAngle < highAngle) return true;
  }  
  else
  {
    if ((testAngle > lowAngle && testAngle < 360) || (testAngle < highAngle)) return true;
  }
  return false;
}

int getIntSerialParameter()
{
  delay(10);
  
  while (Serial.available() > 0)
  {
    if(index < 19) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      if (inChar == 'c') 
      {
        calibrateVisibility = true;
        inChar = Serial.read();
      }
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
      inData[index] = '\0'; // Null terminate the string
    }
  }
  index = 0;
  
  return atoi(inData);
}
