
#include <WiFiS3.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

const char* host = "192.168.1.101";   //AAAABITR Router
const uint16_t port = 49002;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged\n";       // a string to send back

//WiFiUDP Udp;
WiFiClient client;

int lY;

void setup() {
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

  //String read = client.readString();
  int lX = client.read();
  int lY = client.read();
  int rX = client.read();
  int rY = client.read();
  int eStop = client.read();
  int enable = client.read();
  int mode = client.read();
  //client.readBytes()
  //client.readString()
  //char ch = static_cast<char>(read);

  //if( read != ""){
    Serial.print("Char Recieved: ");
    Serial.print(lX);
    Serial.print(lY);
    Serial.print(rX);
    Serial.print(rY);
    Serial.print(eStop);
    Serial.print(enable);
    Serial.println(mode);
    //Serial.println(lX);
    delay(100);
    client.print(lX);
    //char packet[read.length()];
    //read.toCharArray(packet, read.length());
  //}

  Serial.print("lY: ");
  Serial.println(lY);
  
  /*lY = map(read[1], 84, 21, 90, 180);
  Serial.print("Speed: ");
  Serial.println(lY);*/

  /*if(read != -1){
    Serial.print("Char Recieved: ");
    Serial.println(read);
    //delay(1000);
    client.print(read);
  }*/

  

  /*if(ch == 'L'){
    //ip = WiFi.localIP();
    
    client.print(123);
    //clinet.print(ip.toString());
  }*/
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
