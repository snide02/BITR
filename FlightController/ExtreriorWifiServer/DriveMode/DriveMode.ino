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
float steer;

int motor1Speed;
int motor2Speed;
int motor3Speed;
int motor4Speed;

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

//const char* host = "192.168.1.87";  //my Home ip
//const char* host = "192.168.1.242";    //Paul's house ip
const char* host = "192.168.1.101";   //AAAABITR Router
const uint16_t port = 49002;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged\n";       // a string to send back

char packet[9];

//WiFiUDP Udp;
WiFiClient client;

void setup() {
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
  }

  String read = client.readString();
  //client.read()
  //client.readBytes()
  //client.readString()
  //char ch = static_cast<char>(read);

  if( read != ""){
    Serial.print("Char Recieved: ");
    Serial.println(read);
    //delay(1000);
    client.print(read);
    packet[read.length() + 1];
    read.toCharArray(packet, read.length() + 1);

    Serial.print("Packet: ");
    Serial.println(packet);

  }

  int lX = packet[0];
  int lY = packet[1];
  int rX = packet[2];
  int rY = packet[3];
  int eStop = packet[4];
  int enable = packet[5];
  int mode = packet[6];

  throttle = map(lY, 84, 21, 0, 180);
  steer = map(rX, 17, 83, -100, 100);
  steer = steer / 100;

  Serial.print("Throttle: ");
  Serial.println(throttle);
  Serial.print("Steer: ");
  Serial.println(steer);

  if(steer < 0){ // Steer Left
    motor1Speed = throttle + throttle * steer ;
    motor4Speed = throttle + throttle * steer;

    motor2Speed = throttle;
    motor3Speed = throttle;
  }
  else if (steer > 0){ // Steer Right
    motor1Speed = throttle;
    motor4Speed = throttle;

    motor2Speed = throttle - throttle * steer;
    motor3Speed = throttle - throttle * steer;
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
