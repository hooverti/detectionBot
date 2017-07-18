#include <AFMotor.h>
#include <Servo.h>

Servo servo;
AF_DCMotor right_motor(1, MOTOR12_8KHZ);    //declares the left motor and gives it a set frequency
AF_DCMotor left_motor(2, MOTOR12_8KHZ);     //declares the right motor and gives it a set frequency
const int trigPin = A0;                     //delcares the trig pin for the ping sensor plugged into A0
const int echoPin = A1;                     //declares echo pin as plugged into A1

void setup(){
  Serial.begin(9600);

  servo.attach(9);
  right_motor.setSpeed(200);           //sets the speed for the motors out of 255
  left_motor.setSpeed(201);
}

//main function
void loop(){

  servo.write(90);
  right_motor.setSpeed(200);
  left_motor.setSpeed(201);
  
  long cm;
  cm = visionSetup();

//if statement for deciding if there is something in the way of the robot
  if(cm > 30.00){
    motorForward();
  }
  else{
    right_motor.run(BACKWARD);
    left_motor.run(BACKWARD);
    delay(50);
    right_motor.setSpeed(0);
    left_motor.setSpeed(0);
    delay(450);
    checkPath();
  }
}

long visionSetup()
{
  long duration, cm;
  pinMode(trigPin, OUTPUT);            //sets up the trigPin for the "eyes" as output
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);                

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);            //sets up echoPin for the eyes as input
  duration = pulseIn(echoPin, HIGH);

//holds the distance calculated from the time to distance function
  cm = microsecondsToCentimeters(duration);

  return cm;
}

//function for making robot move forward
void motorForward()
{
  right_motor.run(FORWARD);
  left_motor.run(FORWARD);
}

//function for making robot turn right
void motorTurnRight()
{
  right_motor.setSpeed(200);
  left_motor.setSpeed(190);
  right_motor.run(BACKWARD);
  left_motor.run(FORWARD);
  delay (500);
}

//function for making robot turn left
void motorTurnLeft()
{
  right_motor.setSpeed(190);
  left_motor.setSpeed(200);
  right_motor.run(FORWARD);
  left_motor.run(BACKWARD);
  delay(500);
}

//function for converting time to distance for the eyes
long microsecondsToCentimeters(long microseconds)
{
  return ((microseconds / 29) / 2);
}

void checkPath()
{
  long pathLeft;
  long pathRight;  

  servo.write(16);
  delay(1000);
  pathRight = visionSetup();
  delay(300);
  
  servo.write(164);
  delay(1000);
  pathLeft = visionSetup();
  delay(300);

  servo.write(90);
  delay(300);

  if(pathLeft > pathRight)
  {
    motorTurnLeft();  
  }
  else{
    motorTurnRight();
  }
}

