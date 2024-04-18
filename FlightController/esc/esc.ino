/*
        Arduino Brushless Motor Control
     by Dejan, https://howtomechatronics.com/tutorials/arduino/arduino-brushless-motor-control-tutorial-esc-bldc/
     
     Taken from here so we can test.
*/

#include <Servo.h>
#include <Wire.h> 
#include <Adafruit_PWMServoDriver.h>

Servo ESC5, ESC4, ESC3, ESC2;     // create servo object to control the ESC
Servo servo9;
Servo servo8;
Servo servo7;
Servo servo6;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int potValue, ipotValue;  // value from the analog pin
int sentVal;

void setup() {

  pwm.begin();
  pwm.setPWMFreq(50);

  // (pin, min pulse width, max pulse width in microseconds)
  ESC5.attach(5,1000,2000); //motor1
  ESC4.attach(4,1000,2000); //motor2
  ESC3.attach(3,1000,2000); //motor3
  ESC2.attach(2,1000,2000); //motor4
  servo9.attach(9,1000,2000);
  servo8.attach(8,1000,2000);
  servo7.attach(7,1000,2000);
  servo6.attach(6,1000,2000);
  /*  
  ESC5.write(90);
  ESC4.write(90);
  ESC3.write(90);
  ESC2.write(90);

  servo9.write(180);
  servo8.write(180);
  servo7.write(180);
  servo6.write(180);
  */
  delay(3000);

  Serial.begin(9600);
}

void loop() {
  int zero = 323;
  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, zero, 436);   // scale it to use it with the servo library (value between 0 and 180)
  ipotValue = map(potValue, zero, 436, zero, 217);
  sentVal = potValue;
  Serial.print("Pot Value:");
  Serial.println(potValue);
  Serial.print("Ipot Value:");
  Serial.println(ipotValue);

  /*if (sentVal <= 20 && sentVal >= -20){
    sentVal = 0;
  }*/

  pwm.setPWM(3, 0, potValue);
  pwm.setPWM(1, 0, ipotValue);
  //ESC2.write(potValue);    // Send the signal to the ESC
  //ESC.write(sentVal);

  //Serial.println(ESC2.read());

/*
  servo9.write(180);
  servo8.write(180);
  servo7.write(180);
  servo6.write(180);

  delay(500);

/*
  ESC7.write(180);
  Serial.print("  Sent Value: ");
  Serial.println(180); 
  delay(2000);

  ESC7.write(0);
  Serial.print("  Sent Value: ");
  Serial.println(0);
  delay(2000);
*/



/*
ESC5.write(110);
ESC4.write(110);
ESC3.write(110);
ESC2.write(110);
Serial.println("110");
delay(3000);

ESC5.write(90);
ESC4.write(90);
ESC3.write(90);
ESC2.write(90);
Serial.println(90);
delay(3000);
*/

/*
ESC5.write(60);
ESC4.write(60);
ESC3.write(60);
ESC2.write(60);
Serial.println(60);
delay(3000);

ESC5.write(90);
ESC4.write(90);
ESC3.write(90);
ESC2.write(90);
Serial.println(90);
delay(3000);
*/
/*
//this block moves all 4 servos down to 180 then up to 0
servo9.write(180);
servo8.write(180);
servo7.write(180);
servo6.write(180);
Serial.println("AAAA   Servo 180");

delay(3000);

servo9.write(0);
servo8.write(0);
servo7.write(0);
servo6.write(0);
Serial.println("BBBB   Servo 0");

delay(3000);


  /*
  if(ESC.read() == 90){
    ESC.write(180);
    delay(2000);
    Serial.print("  Sent Value: ");
    Serial.println(180);
  }
  else if(ESC.read() == -20){
    ESC.write(90);
    delay(2000);
    Serial.print("  Sent Value: ");
    Serial.println(90);
  }
  else if(ESC.read() == 180){
    ESC.write(-20);
    delay(2000);
    Serial.print("  Sent Value: ");
    Serial.println(-20);
  }*/

  /*
  Serial.print("potValue: ");
  Serial.print(potValue);

  ESC.write(sentVal);

  Serial.print("  Sent Value: ");
  Serial.println(sentVal);
  */
}
