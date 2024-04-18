/* http://www.youtube.com/c/electronoobs
 * 
 * This link is not for this code, just more reference material
*/

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <SPI.h>
//#include <RF24.h> This is his manual input which will be something else for us.
#include <printf.h>

#define CE_PIN   9
#define CSN_PIN 10

// Create Radio and define pins
RF24 radio(CE_PIN, CSN_PIN);

// Create Radio channels
const uint64_t pAddress = 0xB00B1E5000LL;

//Radio recived array
float recivedDataArray[4];

// Create timer
float elapsedTime, time, timePrev;
long loop_timer;
float previousMessageMillis;

// Create motors
Servo motor_LF;
Servo motor_RF;
Servo motor_LB;
Servo motor_RB;

// Create a IMU
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);

/* Temporary variable declaration */
float input_throttle = 1000;

/* Roll PID */
float roll_PID, pid_throttle_L_F, pid_throttle_L_B, pid_throttle_R_F, pid_throttle_R_B, roll_error, roll_previous_error;
float roll_pid_p = 0;
float roll_pid_i = 0;
float roll_pid_d = 0;
/* Roll PID Constants */
double roll_kp = 2;
double roll_ki = 0.02;
double roll_kd = 12;
float desired_roll_dps = 0;

/* Pitch PID */
float pitch_PID, pitch_error, pitch_previous_error;
float pitch_pid_p = 0;
float pitch_pid_i = 0;
float pitch_pid_d = 0;
/* Pitch PID Constants */
double pitch_kp = roll_kp;
double pitch_ki = roll_ki;
double pitch_kd = roll_kd;
float desired_pitch_dps = 0;

/* Yaw PID */
float yaw_PID, yaw_error, yaw_previous_error;
float yaw_pid_p = 0;
float yaw_pid_i = 0;
float yaw_pid_d = 0;
/* Pitch PID Constants */
double yaw_kp = roll_kp;
double yaw_ki = roll_ki;
double yaw_kd = roll_kd;
float desired_yaw_dps = 0;

/* Limit the PID so that it doesnt go crazy */
int pid_max = 200;

/* Low pass filter variables for the gyroscope and accelerometer */
float IMURollOld = 0, IMURollNew = 0, IMUPitchOld = 0, IMUPitchNew = 0;
float AccXOld = 0, AccYOld = 0, AccXNew = 0, AccYNew = 0;

/* Accelerometer Stabilization variables */
bool accelStabilization = true;

/* Accel PID */
float accX_PID, accY_PID, accY_error, accY_previous_error, accX_error, accX_previous_error;
float accX_pid_p = 0;
float accX_pid_i = 0;
float accX_pid_d = 0;
float accY_pid_p = 0;
float accY_pid_i = 0;
float accY_pid_d = 0;
/* Accel PID Constants */
double acc_kp = 1;
double acc_ki = 0.05;
double acc_kd = 5;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial);
  printf_begin();

  if (!bno.begin())
  {
    Serial.print("Error, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  int8_t temp = bno.getTemp();
  bno.setExtCrystalUse(true);

  delay(1000);

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, pAddress);

  //radio.printDetails();
  radio.startListening();

  delay(5000);

  motor_LF.attach(5);
  motor_RF.attach(6);
  motor_LB.attach(7);
  motor_RB.attach(8);

  motor_LF.writeMicroseconds(1000);
  motor_RF.writeMicroseconds(1000);
  motor_LB.writeMicroseconds(1000);
  motor_RB.writeMicroseconds(1000);
}
void loop() {
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  if (radio.available()) {
    radio.read( &recivedDataArray, sizeof(recivedDataArray) );
    //Serial.println("Recieved array:");
    previousMessageMillis = millis();
    for (byte i = 0; i < 4; i++) {
      //Serial.println(recivedDataArray[i]);
    }
    input_throttle = recivedDataArray[0];
    //desired_yaw_dps += recivedDataArray[1];
    desired_pitch_dps = recivedDataArray[2] * 2;
    desired_roll_dps = recivedDataArray[3] * 2;
  }

  if (input_throttle > 1500) {
    input_throttle = 1500;
  } else if (input_throttle < 1050) {
    input_throttle = 1000;
  }

  acro(gyro.x(), gyro.y(), gyro.z());

  /* Start Timer */
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000;

  if (millis() - previousMessageMillis >= 400) {
    //PID_reset();
  } else {
    motor_LF.writeMicroseconds(pid_throttle_L_F);
    motor_RF.writeMicroseconds(pid_throttle_R_F);
    motor_LB.writeMicroseconds(pid_throttle_L_B);
    motor_RB.writeMicroseconds(pid_throttle_R_B);
  }
  delay(20);

  //printPID();
  //printTrottle();
  //printDPS();
  printGyro(gyro.x(), gyro.y());
  //printAccel(accel.x(), accel.y());
}

void printDPS() {
  Serial.print("Roll DPS: ");
  Serial.println(desired_roll_dps);
  Serial.print("Pitch DPS: ");
  Serial.println(desired_pitch_dps);
}

void printTrottle() {
  Serial.print(pid_throttle_L_F);
  Serial.print("  ");
  Serial.println(pid_throttle_R_F);
  Serial.print(pid_throttle_L_B);
  Serial.print("  ");
  Serial.println(pid_throttle_R_B);
  Serial.println();
}

void printGyro(float x, float y) {
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(IMURollNew);
  Serial.print(",");
  Serial.println(IMUPitchNew);
}

void printPID() {
  Serial.print("Roll PID: ");
  Serial.println(roll_PID);
  Serial.print("Pitch PID: ");
  Serial.println(pitch_PID);
}

void printAccel(float x, float y) {
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(AccXNew);
  Serial.print(",");
  Serial.println(AccYNew);
}

void acro(float gyroX, float gyroY, float gyroZ) {

  // Gyroscope low pass filter
  IMURollNew = ((gyroX) * 0.1) + (IMURollOld * 0.9);
  IMUPitchNew = ((gyroY) * 0.1) + (IMUPitchOld * 0.9);

  if (input_throttle > 1100) {
    roll_error = IMURollOld - desired_roll_dps;
    pitch_error = IMUPitchOld - desired_pitch_dps;
    yaw_error = gyroZ - desired_yaw_dps;

    /* Prospect */
    roll_pid_p = roll_kp * roll_error;
    pitch_pid_p = pitch_kp * pitch_error;
    yaw_pid_p = yaw_kp * yaw_error;

    // Integral
    roll_pid_i += roll_ki * roll_error;
    pitch_pid_i += pitch_ki * pitch_error;
    yaw_pid_i += yaw_ki * yaw_error;

    //ROLL
    if (roll_pid_i > pid_max) roll_pid_i = pid_max;
    else if (roll_pid_i < pid_max * -1) roll_pid_i = pid_max * -1;

    roll_PID = roll_kp * roll_error + roll_pid_i + roll_kd * (roll_error - roll_previous_error);
    if (roll_PID > pid_max) roll_PID = pid_max;
    else if (roll_PID < pid_max * -1) roll_PID = pid_max * -1;

    //PITCH
    if (pitch_pid_i > pid_max) pitch_pid_i = pid_max;
    else if (pitch_pid_i < pid_max * -1) pitch_pid_i = pid_max * -1;

    pitch_PID = pitch_kp * pitch_error + pitch_pid_i + pitch_kd * (pitch_error - pitch_previous_error);
    if (pitch_PID > pid_max) pitch_PID = pid_max;
    else if (pitch_PID < pid_max * -1) pitch_PID = pid_max * -1;

    //YAW
    if (yaw_pid_i > pid_max) yaw_pid_i = pid_max;
    else if (yaw_pid_i < pid_max * -1) yaw_pid_i = pid_max * -1;

    yaw_PID = yaw_kp * yaw_error + yaw_pid_i + yaw_kd * (yaw_error - yaw_previous_error);
    if (yaw_PID > pid_max) yaw_PID = pid_max;
    else if (yaw_PID < pid_max * -1) yaw_PID = pid_max * -1;

    /* Set the throttle PID for each motor */
    pid_throttle_L_F = input_throttle - roll_PID + pitch_PID - yaw_PID;
    pid_throttle_R_F = input_throttle + roll_PID + pitch_PID + yaw_PID;
    pid_throttle_L_B = input_throttle - roll_PID - pitch_PID + yaw_PID;
    pid_throttle_R_B = input_throttle + roll_PID - pitch_PID - yaw_PID;

    /* Save Previous Error */
    roll_previous_error = roll_error;
    pitch_previous_error = pitch_error;
    yaw_previous_error = yaw_error;

  } else {
    PID_reset();
  }
  IMURollOld = IMURollNew;
  IMUPitchOld = IMUPitchNew;
}

void PID_reset() {
  //Serial.println("RESET");

  pid_throttle_L_F = 1000;
  pid_throttle_L_B = 1000;
  pid_throttle_R_F = 1000;
  pid_throttle_R_B = 1000;

  roll_pid_p = 0;
  roll_pid_i = 0;
  roll_pid_d = 0;

  pitch_pid_p = 0;
  pitch_pid_i = 0;
  pitch_pid_d = 0;

  pitch_PID = 0;
  roll_PID = 0;
  yaw_PID = 0;

  roll_error = 0;
  pitch_error = 0;
  yaw_error = 0;

  roll_previous_error = 0;
  pitch_previous_error = 0;
  yaw_previous_error = 0;
}
