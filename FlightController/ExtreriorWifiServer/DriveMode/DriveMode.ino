#include <Servo.h>
#include <WiFiS3.h>

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int throttle;
int counterThrottle;
float steer;

int motor1Speed;
int motor2Speed;
int motor3Speed;
int motor4Speed;

int lX, lY, rX, rY, eStop, enable, mode;

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

bool wasConnected = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Attach the ESC on pin 9
  motor1.attach(5,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  motor2.attach(4,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  motor3.attach(3,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  motor4.attach(2,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

  servo1.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  servo2.attach(8,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  servo3.attach(7,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  servo4.attach(6,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  
  strcpy(packet, "0T00");

  /*motor1.write(90);
  motor2.write(90);
  motor3.write(90);
  motor4.write(90);

  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  servo4.write(180);*/

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

}

void loop() {
  while(!client.connected()){
    client.connect(host, port);
    if(wasConnected){
      motor1.write(90);
      motor2.write(90);
      motor3.write(90);
      motor4.write(90);

      Serial.print("WARNING!!!!  CONNECTION LOST!!!!");
    }
  }
  wasConnected = true;
  //while(client.connected()){
  //String read = client.readString();
  lX = client.read();
  lY = client.read();
  rX = client.read();
  rY = client.read();
  eStop = client.read();
  enable = client.read();
  mode = client.read();

  //if( read != ""){
  Serial.print("Packet Recieved: ");
  //Serial.println(read);
  Serial.print("Char Recieved: ");
  Serial.print(lX);
  Serial.print(lY);
  Serial.print(rX);
  Serial.print(rY);
  Serial.print(eStop);
  Serial.print(enable);
  Serial.println(mode);
  delay(100);
  client.print(lX);
  //packet[read.length() + 1];
  //read.toCharArray(packet, read.length() + 1);

  Serial.print("Packet: ");
  Serial.println(packet);

  /*lX = packet[0];
  lY = packet[1];
  rX = packet[2];
  rY = packet[3];
  eStop = packet[4];
  enable = packet[5];
  mode = packet[6];*/

  //}
  
  if(eStop == 49){
    while(1){
      motor1.write(90);
      motor2.write(90);
      motor3.write(90);
      motor4.write(90);

      Serial.print("EMERGENCY!!!!  E_STOP HAS BEEN TRIGGERED!!!!");

    }
  }

  if(enable == 49){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  throttle = map(lY, 84, 21, 90, 180);
  //throttle = map(lX, 17, 86, 70, 110);
  counterThrottle = map(throttle, 110, 90, 70, 90);
  steer = map(rX, 17, 83, -100, 100);
  steer = steer / 100;
  steer = 0;
  counterThrottle = throttle;

  Serial.print("Throttle: ");
  Serial.println(throttle);
  Serial.print("Steer: ");
  Serial.println(steer);

  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  servo4.write(180);
  

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

  motor1.write(motor1Speed);
  motor2.write(motor2Speed);
  motor3.write(motor3Speed);
  motor4.write(motor4Speed);

  Serial.print("Motor 1 Speed: ");
  Serial.println(motor1Speed);
  Serial.print("Motor 4 Speed: ");
  Serial.println(motor4Speed);

  Serial.print("Motor 2 Speed: ");
  Serial.println(motor2Speed);
  Serial.print("Motor 3 Speed: ");
  Serial.println(motor3Speed);
  
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
