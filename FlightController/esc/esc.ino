/*
        Arduino Brushless Motor Control
     by Dejan, https://howtomechatronics.com/tutorials/arduino/arduino-brushless-motor-control-tutorial-esc-bldc/
     
     Taken from here so we can test.
*/

#include <Servo.h>

Servo ESC5, ESC4, ESC3, ESC2;     // create servo object to control the ESC
Servo servo9;
Servo servo8;
Servo servo7;
Servo servo6;

int potValue;  // value from the analog pin
int sentVal;

void setup() {
  // Attach the ESC on pin 9
  ESC5.attach(5,1000,1800); // (pin, min pulse width, max pulse width in microseconds) 
  ESC4.attach(4,1000,1800);
  ESC3.attach(3,1000,1800);
  ESC2.attach(2,1000,1800);
  servo9.attach(9,1000,2000);
  servo8.attach(8,1000,2000);
  servo7.attach(7,1000,2000);
  servo6.attach(6,1000,2000);
  
ESC5.write(90);
ESC4.write(90);
ESC3.write(90);
ESC2.write(90);

servo9.write(180);
servo8.write(180);
servo7.write(180);
servo6.write(180);

delay(1000);

  Serial.begin(9600);
}

void loop() {
 // potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
 // potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  //sentVal = potValue;

  /*if (sentVal <= 20 && sentVal >= -20){
    sentVal = 0;
  }*/

  //ESC.write(potValue);    // Send the signal to the ESC
  //ESC.write(sentVal);

  //Serial.println(ESC.read());
  //delay(2000);

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




ESC5.write(120);
ESC4.write(120);
ESC3.write(120);
ESC2.write(120);
Serial.println(120);
delay(3000);

ESC5.write(90);
ESC4.write(90);
ESC3.write(90);
ESC2.write(90);
Serial.println(90);
delay(3000);

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

/*this block moves all 4 servos down to 180 then up to 0
servo9.write(180);
servo8.write(180);
servo7.write(180);
servo6.write(180);

delay(3000);

servo9.write(0);
servo8.write(0);
servo7.write(0);
servo6.write(0);

delay(3000);
*/

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
  Serial.print("  Sent Value: ");
  Serial.println(sentVal);
  */
}
