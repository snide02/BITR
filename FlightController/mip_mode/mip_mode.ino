#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>


#define MOTOR_PIN 9 // Example pin for motor control

// Define PID constants
#define KP 6.0  //try starting at 0.1
#define KI 1.8  //make sure KI is very small because integrating 100 times per second. 0.005
#define KD 0.7

// Define setpoint (desired angle)
#define SETPOINT 0.0

// Define variables
double angle, error, lastError, integral, derivative, output;
double max_input, min_input, loop_time, max_error, min_pidOutput, max_pidOutput;

int test_var = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
//Servo ESC;     // create servo object to control the ESC
Adafruit_PWMServoDriver pwmBR = Adafruit_PWMServoDriver(); 
Adafruit_PWMServoDriver pwmBL = Adafruit_PWMServoDriver(); 

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
  //ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  pwmBR.begin();
  pwmBR.setPWMFreq(50); //maximum is 1600
  pwmBL.begin();
  pwmBL.setPWMFreq(50);

  Serial.begin(115200);
  //ESC.write(0);
  pwmBR.setPWM(1, 0, 0);
  pwmBL.setPWM(2, 0, 0);  
  delay(4000);

  integral = 0;

  // Define the range of input values to the PID controller
  max_input = 88.0;  // Maximum observed input value
  min_input = -39; // Minimum observed input value

  // Define the loop time (in milliseconds)
  loop_time = 10.0; // Time between consecutive PID calculations (10 milliseconds in my case)

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
  int motorSpeed = map(pidOutput, min_pidOutput, max_pidOutput, 217, 436); // -84 is the highest observed value when arm is straight up
  //potentially map to 35, not 0.0.

  //217 and 436???

  Serial.print(" integral: ");
  Serial.print(integral);
  Serial.print(" pidOutput: ");
  Serial.print(pidOutput);
  Serial.print(" test_var: ");
  Serial.print(test_var);
  Serial.print(" motorSpeed: ");
  Serial.println(motorSpeed);
  //analogWrite(MOTOR_PIN, abs(motorSpeed));
  //if(motorSpeed <= 80)
  //  ESC.write(motorSpeed);
  //else
  //  ESC.write(80);

  //ESC.write(pidOutput);

  //if(test_var < 260) test_var = test_var + 1;
  pwmBR.setPWM(1, 0, pidOutput);
  pwmBL.setPWM(2, 0, pidOutput);
  //lowest value to send is 217, middle is 327, max is 436
  //will want to write motorspeed

  // Delay for stability
  delay(loop_time);

  //resting speed was motorSpeed = 68
  //Moving past y angle 0, we want motorSpeed = 68
  //need to synchronize powering ESC and turning on of Arduino
}


//EOF
