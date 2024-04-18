/*************************************************** 
  This is an example for our keyestudio 16-channel PWM & Servo driver
  PWM test - this will drive 16 PWMs in a 'wave'

  The Servo Shield uses I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  keyestudio invests time and resources providing this open source code, 
  please support keyestudio and open-source hardware by purchasing 
  products from keyestudio !
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int potValue;  // value from the analog pin

void setup() {
  Serial.begin(9600);
  //delay(3000);
  Serial.println("16 channel PWM test!");

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!

  pwm.begin();
  pwm.setPWMFreq(50); //maximum is 1600
    
  // save I2C bitrate
  //uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  //TWBR = 12; // upgrade to 400KHz!
    
}

void loop() {
  // Drive each PWM in a 'wave'
  // "safe" limits of 200 and 400 are used here to protect servos with undetermined end stop points
  /*for (uint16_t i=200; i<400; i += 4) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm.setPWM(pwmnum, 0, i);
     }
    delay(200);
  }*/

  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, 217, 435);   // scale it to use it with the servo library (value between 0 and 180)

  pwm.setPWM(0, 0, potValue);

  Serial.println(potValue);

}
