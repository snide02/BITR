#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>

#define MOTOR_FL_PIN 9 // Front left motor pin
#define MOTOR_FR_PIN 10 // Front right motor pin
#define MOTOR_BL_PIN 11 // Back left motor pin
#define MOTOR_BR_PIN 12 // Back right motor pin

// Define ROLL_PID constants
#define ROLL_KP 1.1
#define ROLL_KI 0.4
#define ROLL_KD 0.2

// Define PITCH_PID constants
#define PITCH_KP 1.1
#define PITCH_KI 0.4
#define PITCH_KD 0.2

// Define YAW_PID constants
#define YAW_KP 1.1
#define YAW_KI 0.4
#define YAW_KD 0.2


// Define ROLL_ANGLE ROLL_SETPOINT (desired ROLL_ANGLE)
#define ROLL_SETPOINT 0.0 // Adjust as needed for desired orientation

// Define Vars
double loop_time;

// Define Roll Vars
double ROLL_ANGLE;
double ROLL_ERROR, LAST_ROLL_ERROR;
double ROLL_INTEGRAL, ROLL_DERIVATIVE;
double ROLL_MAX_INPUT, ROLL_MIN_INPUT, ROLL_MAX_ERROR;
double roll_pid_output;

// Define Pitch Vars
double PITCH_ANGLE;
double PITCH_ERROR, LAST_PITCH_ERROR;
double PITCH_INTEGRAL, PITCH_DERIVATIVE;
double PITCH_MAX_INPUT, PITCH_MIN_INPUT, PITCH_MAX_ERROR;
double pitch_pid_output;

// Define Yaw Vars
double YAW_ANGLE;
double YAW_ERROR, LAST_YAW_ERROR;
double YAW_INTEGRAL, YAW_DERIVATIVE;
double YAW_MAX_INPUT, YAW_MIN_INPUT, YAW_MAX_ERROR;
double yaw_pid_output;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Servo motorFL, motorFR, motorBL, motorBR; // Servo objects for each motor

// Create PID controller
void calculatePID(double x_input, double y_input, double z_input) {
  //======================================== BELOW IS FOR ROLL ========================================
  // Calculate ROLL_ERROR
  ROLL_ERROR = ROLL_SETPOINT - x_input;

  // ROLL_PROPORTIONAL_TERM
  double ROLL_PROPORTIONAL_TERM = ROLL_KP * ROLL_ERROR;

  // ROLL_INTEGRAL_TERM
  if(abs(ROLL_INTEGRAL) <= 200)
    ROLL_INTEGRAL += ROLL_ERROR * 0.01;
  double ROLL_INTEGRAL_TERM = ROLL_KI * ROLL_INTEGRAL;

  // ROLL_DERIVATIVE_TERM
  ROLL_DERIVATIVE = (ROLL_ERROR - LAST_ROLL_ERROR) / 0.01;
  double ROLL_DERIVATIVE_TERM = ROLL_KD * ROLL_DERIVATIVE;

  // Calculate output
  roll_pid_output = ROLL_PROPORTIONAL_TERM + ROLL_INTEGRAL_TERM + ROLL_DERIVATIVE_TERM;

  // Save ROLL_ERROR for next iteration
  LAST_ROLL_ERROR = ROLL_ERROR;


  //======================================== BELOW IS FOR PITCH ========================================
  // Calculate PITCH_ERROR
  PITCH_ERROR = PITCH_SETPOINT - x_input;

  // PITCH_PROPORTIONAL_TERM
  double PITCH_PROPORTIONAL_TERM = PITCH_KP * PITCH_ERROR;

  // PITCH_INTEGRAL_TERM
  if(abs(PITCH_INTEGRAL) <= 200)
    PITCH_INTEGRAL += PITCH_ERROR * 0.01;
  double PITCH_INTEGRAL_TERM = PITCH_KI * PITCH_INTEGRAL;

  // PITCH_DERIVATIVE_TERM
  PITCH_DERIVATIVE = (PITCH_ERROR - LAST_PITCH_ERROR) / 0.01;
  double PITCH_DERIVATIVE_TERM = PITCH_KD * PITCH_DERIVATIVE;

  // Calculate output
  pitch_pid_output = PITCH_PROPORTIONAL_TERM + PITCH_INTEGRAL_TERM + PITCH_DERIVATIVE_TERM;

  // Save PITCH_ERROR for next iteration
  LAST_PITCH_ERROR = PITCH_ERROR;


    //======================================== BELOW IS FOR YAW ========================================
  // Calculate YAW_ERROR
  YAW_ERROR = YAW_SETPOINT - x_input;

  // YAW_PROPORTIONAL_TERM
  double YAW_PROPORTIONAL_TERM = YAW_KP * YAW_ERROR;

  // YAW_INTEGRAL_TERM
  if(abs(YAW_INTEGRAL) <= 200)
    YAW_INTEGRAL += YAW_ERROR * 0.01;
  double YAW_INTEGRAL_TERM = YAW_KI * YAW_INTEGRAL;

  // YAW_DERIVATIVE_TERM
  YAW_DERIVATIVE = (YAW_ERROR - LAST_YAW_ERROR) / 0.01;
  double YAW_DERIVATIVE_TERM = YAW_KD * YAW_DERIVATIVE;

  // Calculate output
  yaw_pid_output = YAW_PROPORTIONAL_TERM + YAW_INTEGRAL_TERM + YAW_DERIVATIVE_TERM;

  // Save YAW_ERROR for next iteration
  LAST_YAW_ERROR = YAW_ERROR;
}

void setup() {
  Serial.begin(115200);  
  motorFL.attach(MOTOR_FL_PIN, 1100, 2000);
  motorFR.attach(MOTOR_FR_PIN, 1100, 2000);
  motorBL.attach(MOTOR_BL_PIN, 1100, 2000);
  motorBR.attach(MOTOR_BR_PIN, 1100, 2000);
  motorFL.write(0);
  motorFR.write(0);
  motorBL.write(0);
  motorBR.write(0);
  delay(4000);

  ROLL_INTEGRAL = 0;
  PITCH_INTEGRAL = 0;
  YAW_INTEGRAL = 0;

  // Define the range of input values to the PID controller
  //Are these true max and min? TEST TO FIND OUT!!!
  ROLL_MAX_INPUT = 180.0; // Maximum observed input value
  ROLL_MIN_INPUT = -180.0; // Minimum observed input value
  PITCH_MAX_INPUT = 180.0; // Maximum observed input value
  PITCH_MIN_INPUT = -180.0; // Minimum observed input value
  YAW_MAX_INPUT = 180.0; // Maximum observed input value
  YAW_MIN_INPUT = -180.0; // Minimum observed input value

  // Define the loop time (in milliseconds)
  loop_time = 10.0; // Time between consecutive PID calculations

  // Calculate the maximum possible ROLL_ERROR
  ROLL_MAX_ERROR = (ROLL_MAX_INPUT - ROLL_MIN_INPUT) / (2 * (ROLL_KP + ROLL_KI / (loop_time / 1000.0) + ROLL_KD * (loop_time / 1000.0)));
  PITCH_MAX_ERROR = (PITCH_MAX_INPUT - PITCH_MIN_INPUT) / (2 * (PITCH_KP + PITCH_KI / (loop_time / 1000.0) + PITCH_KD * (loop_time / 1000.0)));
  YAW_MAX_ERROR = (YAW_MAX_INPUT - YAW_MIN_INPUT) / (2 * (YAW_KP + YAW_KI / (loop_time / 1000.0) + YAW_KD * (loop_time / 1000.0)));

  // Initialize IMU
  Wire.begin();
  bno.begin();
  bno.setExtCrystalUse(true);
}

void loop() {
  // Read IMU data
  sensors_event_t event;
  bno.getEvent(&event);
  //These x y z angles will need to be properly set once the IMU is attached to the robot!!!
  ROLL_ANGLE = event.orientation.y; 
  PITCH_ANGLE = event.orientation.z;
  YAW_ANGLE = event.orientation.x; 

  // Calculate PID
  //Are the angles passed in the proper angles for x, y, z? TEST WITH IMU!!!
  calculatePID(ROLL_ANGLE, PITCH_ANGLE, YAW_ANGLE); //roll_pid_output, pitch_pid_output, yaw_pid_output are now stored

  // Map PID output to motor speeds
  //pidOutput is out of commission.
  //  --> Motor speeds will need to be calculated based off of roll_pid_output, pitch_pid_output, yaw_pid_output
  int motorSpeedFL = throttle - pitch_pid_output - roll_pid_output + yaw_pid_output; // Front left  -> M3
  int motorSpeedFR = throttle + pitch_pid_output - roll_pid_output - yaw_pid_output; // Front right -> M2
  int motorSpeedBL = throttle + pitch_pid_output + roll_pid_output + yaw_pid_output; // Back left   -> M4
  int motorSpeedBR = throttle - pitch_pid_output + roll_pid_output - yaw_pid_output; // Back right  -> M1

  // Set motor speeds
  motorFL.write(motorSpeedFL);
  motorFR.write(motorSpeedFR);
  motorBL.write(motorSpeedBL);
  motorBR.write(motorSpeedBR);

  // Delay for stability
  delay(loop_time);
}


//for left right or forwards backwards, map esc of 1000 to 2000 to -10 to 10

/*
Roll: Leaning left to right
Pitch: Looking up and down
Yaw: Looking left to right

Make sure propellers are always spinning at the beginning

Cap PWMs to between 1100 and 2000 probably

Will need two PIDs for a two-axis system. Ex rollPID and pitchPID.
  - Ex line for writing PWM to ESC below
  - pwm_left_front = 115 + inputThrottle + rollPID - pitchPID
    - 115 is initial value so the motor spins initially and isn't starting at 0
    - inputThrottle should be from the user's input

TO DO
 - test and change max and min inputs for roll pitch and yaw




*/
