#include <Servo.h>

//IO pins
int pinL = 5;
int pinR = 4;
int pinServo = 11;                   //PWM pin

int leftValue = 0;
int rightValue = 0;
int error = 0;                       //deviation bewtween the 2 sensors
int errorAVG = 0;                    //error average - rolling 2 point
int deadband = 8;

//servo stuff
Servo hServo;
int Position = 45;
int minPos = 5;
int maxPos = 165;
float output = (maxPos - minPos) / 2;  //intial output position

void setup()
{
  Serial.begin(9600);
  hServo.attach(pinServo);
  
  //set servo to center for alignment purpose
  Serial.println("Moving Servo to Minimum Position");
  hServo.write(minPos);
  delay(5000);
  Serial.println("Moving Servo to Maximum Position");
  hServo.write(maxPos);
  delay(5000);
  Serial.println("Moving Servo to Mid-Point");
  hServo.write(output);
  delay(5000);
  Serial.println("Going Live...........");
}

void loop()
{
  //input reading
  leftValue = analogRead(pinL);
  rightValue = analogRead(pinR);
  
  Serial.print("L = "); Serial.print(leftValue); Serial.print(" | ");
  Serial.print("R = "); Serial.print(rightValue); Serial.print(" | ");
  Serial.print("E = "); Serial.print(error); Serial.print(" | ");
  Serial.print("Eavg = "); Serial.print(errorAVG);
  Serial.println();
  
  //calculate
  error = leftValue - rightValue;
  errorAVG = (errorAVG + error) / 2;
  float newOutput = output + getTravel();
  
  if (newOutput > maxPos)
  {
    Serial.println("At Upper Limnit");
    newOutput = maxPos;
  }
  else
  {
    if (newOutput < minPos)
  {
    Serial.println("At Lower Limtit");
    newOutput = minPos;
  }
}
Serial.println("Writing output");
hServo.write(newOutput);
output = newOutput;
}

int getTravel()
{
  //-1 = Left; +1 + Right
  
  if (errorAVG < (deadband * -1))
  {
    return 1;
  }
  else
  {
  if (errorAVG > deadband)
  {
    return -1;
  }
  else
  {
    //do not move within deadband
    return 0;
  }
}
}
