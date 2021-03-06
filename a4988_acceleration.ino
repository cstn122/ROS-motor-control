/*with accel control
 
  ROS + Stepping motor control
  https://www.youtube.com/watch?v=TTF3L--TUGc

  Execution(at terminal):
  1 $ roscore

  2 $ rosrun rosserial_python serial_node.py /dev/ttyACM0
  
  3 $ rostopic echo /motor1/start

  4 $ rostopic pub motor1/start std_msgs/Float64 "data: 2010010.0"
                                                     (002|010|010:speed=2 accel=10 10rev)
*/

#include "ros.h"
#include <std_msgs/Float64.h>
#include <AccelStepper.h>
#include <string.h>

// Define stepper motor connections and motor interface type(1 means driver).
#define dirPin 5
#define stepPin 2
#define motorInterfaceType 1

ros::NodeHandle nh;

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void stepMotor1 (const std_msgs::Float64 &rpm) {
  float speed = ((abs(rpm.data)-fmod(abs(rpm.data),1000000.0))/1000000.0)*200 ;
  stepper.setMaxSpeed(speed);         // steps/s
  stepper.setSpeed(speed);         // steps/s

  float acceleration = ((abs(fmod(abs(rpm.data),1000000.0))-fmod(fmod(abs(rpm.data),1000000.0),1000.0))/1000.0)*200 ;
  stepper.setAcceleration(acceleration);     // steps/s^2
  
  rounds = fmod(rpm.data,1000.0)*200;
  stepper.moveTo(rounds);
  stepper.runToPosition();
  //stepper.run();

  speed = 0;
  acceleration = 0;
}


ros::Subscriber<std_msgs::Float64> motor1("motor1/start", &stepMotor1);

void setup() {
  nh.initNode();
  nh.subscribe(motor1);
}


void loop() {
  nh.spinOnce();
  delay(1);
}
