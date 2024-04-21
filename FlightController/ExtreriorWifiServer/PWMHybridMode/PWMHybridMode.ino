#include <Servo.h>
#include <WiFiS3.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int motor1 = 0;
int motor2 = 1;
int motor3 = 2;
int motor4 = 3;
int servo1 = 4;
int servo2 = 5;
int servo3 = 6;
int servo4 = 7;

int throttle;
int counterThrottle;
float steer;

int motor1Speed;
int motor2Speed;
int motor3Speed;
int motor4Speed;

int lX, lY, rX, rY, eStop, direction, mode;

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

//const char* host = "192.168.1.87";  //my Home ip
//const char* host = "192.168.1.242";    //Paul's house ip
const char* host = "192.168.1.101";   //AAAABITR Router
//const char* host = "192.168.9.208";   //BITR24 Router
const uint16_t port = 49002;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged\n";       // a string to send back

char packet[9];

//WiFiUDP Udp;
WiFiClient client;
bool wasConnected;
int numNullPackets;

void setup() {
  wasConnected = false;
  numNullPackets = 0;

  pwm.begin();
  pwm.setPWMFreq(50);
  
  strcpy(packet, "0T00");

  delay(3000);

  Serial.begin(9600);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);

  }

  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  //Udp.begin(localPort);

  pwm.setPWM(motor1, 0, 370);
  pwm.setPWM(motor2, 0, 370);
  pwm.setPWM(motor3, 0, 370);
  pwm.setPWM(motor4, 0, 370);

}

void loop() {
  while(!client.connected()){
    client.connect(host, port);
    if(wasConnected){
      pwm.setPWM(motor1, 0, 327);
      pwm.setPWM(motor2, 0, 327);
      pwm.setPWM(motor3, 0, 327);
      pwm.setPWM(motor4, 0, 327);

      Serial.println("WARNING!!!!  CONNECTION LOST!!!!");
    }
  }

  wasConnected = true;

  //while(client.connected()){
  //String read = client.readString();

  if(client.read() == 90){
    lX = client.read();
    lY = client.read();
    rX = client.read();
    rY = client.read();
    eStop = client.read();
    direction = client.read();
    mode = client.read();
  }

  //if( read != ""){
  Serial.print("Char Recieved: ");
  //Serial.println(read);
  Serial.print(lX);
  Serial.print(lY);
  Serial.print(rX);
  Serial.print(rY);
  Serial.print(eStop);
  Serial.print(direction);
  Serial.println(mode);
  delay(50);
  client.print(lX);
  //packet[read.length() + 1];
  //read.toCharArray(packet, read.length() + 1);

  Serial.print("lY: ");
  Serial.println(lY);
  //}
  
  if(eStop == 49){
    while(1){
      pwm.setPWM(motor1, 0, 327);
      pwm.setPWM(motor2, 0, 327);
      pwm.setPWM(motor3, 0, 327);
      pwm.setPWM(motor4, 0, 327);

      Serial.println("EMERGENCY!!!!  E_STOP HAS BEEN TRIGGERED!!!!");
    }
  }

  if(mode == 49){ //Check for drive mode
    pwm.setPWM(servo1, 0, 283);
    pwm.setPWM(servo2, 0, 283);
    pwm.setPWM(servo3, 0, 283);
    pwm.setPWM(servo4, 0, 283);
  }
  if(mode == 48){ //Check for drive mode
    pwm.setPWM(servo1, 0, 370);
    pwm.setPWM(servo2, 0, 370);
    pwm.setPWM(servo3, 0, 370);
    pwm.setPWM(servo4, 0, 370);
  }
  if(mode == 50){ //Check for Hybrid Mode
    if(direction = 48){ //Check if Forward
      pwm.setPWM(servo1, 0, 370);
      pwm.setPWM(servo2, 0, 283);
      pwm.setPWM(servo3, 0, 283);
      pwm.setPWM(servo4, 0, 370);
    }
    if(direction = 49){ //Check if Backwards
      pwm.setPWM(servo1, 0, 283);
      pwm.setPWM(servo2, 0, 370);
      pwm.setPWM(servo3, 0, 283);
      pwm.setPWM(servo4, 0, 370);
    }
  }

  if(lY != -1){
    //throttle = map(lY, 84, 21, 327, 436);
    throttle = map(lY, 84, 21, 327, 350);

    counterThrottle = map(throttle, 327, 350, 327, 304);
    steer = map(rX, 17, 83, -100, 100);
    steer = steer / 100;
    steer = 0;
    //counterThrottle = throttle;
    numNullPackets = 0;
  }else {
    Serial.println("!!!!  lY = -1, THROTTLE NOT CHANGED");
    numNullPackets = numNullPackets + 1;
    if(numNullPackets == 5){
      pwm.setPWM(motor1, 0, 327);
      pwm.setPWM(motor2, 0, 327);
      pwm.setPWM(motor3, 0, 327);
      pwm.setPWM(motor4, 0, 327);

      Serial.print("WARNING!!!!  RECIEVED TOO MANY NULL!!!!");
    }
  }

  Serial.print("Throttle: ");
  Serial.println(throttle);
  Serial.print("Steer: ");
  Serial.println(steer);
  

  if(steer < 0){ // Steer Left
    
    motor1Speed = throttle + throttle * steer ;
    motor4Speed = throttle + throttle * steer;

    motor2Speed = counterThrottle;
    motor3Speed = counterThrottle;
  }
  else if (steer > 0){ // Steer Right
    
    motor1Speed = throttle;
    motor4Speed = throttle;

    motor2Speed = counterThrottle - counterThrottle * steer;
    motor3Speed = counterThrottle - counterThrottle * steer;
  }
  else{
    
    motor1Speed = throttle;
    motor4Speed = throttle;

    motor2Speed = throttle;
    motor3Speed = throttle;
  }
    
  pwm.setPWM(motor1, 0, motor1Speed); 
  pwm.setPWM(motor2, 0, motor2Speed);
  pwm.setPWM(motor3, 0, motor3Speed);
  pwm.setPWM(motor4, 0, motor4Speed);

  Serial.print("Motor 1 Speed: ");
  Serial.println(motor1Speed);
  Serial.print("Motor 4 Speed: ");
  Serial.println(motor4Speed);

  Serial.print("Motor 2 Speed: ");
  Serial.println(motor2Speed);
  Serial.print("Motor 3 Speed: ");
  Serial.println(motor3Speed);
  //}
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}