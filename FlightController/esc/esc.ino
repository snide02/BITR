/*
        Arduino Brushless Motor Control
     by Dejan, https://howtomechatronics.com/tutorials/arduino/arduino-brushless-motor-control-tutorial-esc-bldc/
     
     Taken from here so we can test.
*/

#include <Servo.h>

Servo ESC;     // create servo object to control the ESC

int potValue;  // value from the analog pin
int sentVal;

void setup() {
  // Attach the ESC on pin 9
  ESC.attach(7,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  sentVal = potValue;

  /*if (sentVal <= 20 && sentVal >= -20){
    sentVal = 0;
  }*/

  //ESC.write(potValue);    // Send the signal to the ESC
  //ESC.write(sentVal);

  //Serial.println(ESC.read());
  //delay(2000);


  ESC.write(180);
  Serial.print("  Sent Value: ");
  Serial.println(180); 
  delay(2000);

  ESC.write(0);
  Serial.print("  Sent Value: ");
  Serial.println(0);
  delay(2000);


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
