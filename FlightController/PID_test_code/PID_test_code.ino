#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>


#define MOTOR_PIN 9 // Example pin for motor control

// Define PID constants
#define KP 1.1  //try starting at 0.1
#define KI 0.4  //make sure KI is very small because integrating 100 times per second. 0.005
#define KD 0.2

// Define setpoint (desired angle)
#define SETPOINT 15.0

// Define variables
double angle, error, lastError, integral, derivative, output;
double max_input, min_input, loop_time, max_error, min_pidOutput, max_pidOutput;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Servo ESC;     // create servo object to control the ESC

// Create PID controller
double calculatePID(double input) {
  // Calculate error
  error = SETPOINT - input;
  
  // Proportional term
  double proportional = KP * error;

  // Integral term
  if(abs(integral) <= 200)
    integral += error * 0.01;
  double integralTerm = KI * integral;

  // Derivative term
  derivative = (error - lastError) / 0.01;
  double derivativeTerm = KD * derivative;

  // Calculate output
  output = proportional + integralTerm + derivativeTerm;

  // Save error for next iteration
  lastError = error;

  return output;
}

void setup() {
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  //setPWM();
  Serial.begin(115200);
  ESC.write(0);
  delay(4000);

  integral = 0;

  // Define the range of input values to the PID controller
  max_input = 85.0;  // Maximum observed input value
  min_input = -38.5; // Minimum observed input value

  // Define the loop time (in milliseconds)
  loop_time = 10.0; // Time between consecutive PID calculations (10 milliseconds in your case)

  // Calculate the maximum possible error
  max_error = (max_input - min_input) / (2 * (KP + KI / (loop_time / 1000.0) + KD * (loop_time / 1000.0)));

  // Calculate the minimum and maximum possible values of pidOutput
  min_pidOutput = SETPOINT - max_error;
  max_pidOutput = SETPOINT + max_error;

  // Initialize motor pin
  pinMode(MOTOR_PIN, OUTPUT);

  // Initialize IMU
  Wire.begin();
  bno.begin();
  bno.setExtCrystalUse(true);

  //ESC.write(37);
  //delay(10000);
}

void loop() {
  // Read IMU data
  sensors_event_t event;
  bno.getEvent(&event);
  angle = event.orientation.y; // y-axis which is long length of sensor from top view

  // Calculate PID
  double pidOutput = calculatePID(angle);
  //Serial.print("pidOutput: ");
  //Serial.println(pidOutput);
  Serial.print(" angle: ");
  Serial.print(angle);

  // Adjust motor speed or position based on PID output
  // Example: Motor speed control
//  int motorSpeed = map(pidOutput, -180, 180, -255, 255); // Adjust mapping according to your motor's specifications
  //int motorSpeed = map(pidOutput, 0, 1023, -180, 180); // Adjust mapping according to your motor's specifications
  int motorSpeed = map(pidOutput, min_pidOutput, max_pidOutput, 0.0, 180); // -84 is the highest observed value when arm is straight up
  //potentially map to 35, not 0.0.

  Serial.print(" integral: ");
  Serial.print(integral);
  Serial.print(" pidOutput: ");
  Serial.print(pidOutput);
  Serial.print(" motorSpeed: ");
  Serial.println(motorSpeed);
  //analogWrite(MOTOR_PIN, abs(motorSpeed));
  //if(motorSpeed <= 80)
  //  ESC.write(motorSpeed);
  //else
  //  ESC.write(80);

  ESC.write(pidOutput);

  // Delay for stability
  delay(loop_time);

  //resting speed was motorSpeed = 68
  //Moving past y angle 0, we want motorSpeed = 68
  //need to synchronize powering ESC and turning on of Arduino
}


//EOF
