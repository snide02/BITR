/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will create a new access point (with no password).
  It will then launch a new server and print out the IP address
  to the Serial Monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 13.

  If the IP address of your board is yourAddress:
    http://yourAddress/H turns the LED on
    http://yourAddress/L turns it off

  created 25 Nov 2012
  by Tom Igoe
  adapted to WiFi AP by Adafruit

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#access-point
 */

#include "WiFiS3.h"
#include <Servo.h>
#include "arduino_secrets.h" 

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

Servo ESC;
int potValue;

void setup() {
  //Initialize serial and wait for port to open:
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  WiFi.config(IPAddress(192,48,56,2));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {
  
  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, -90, 90);   // scale it to use it with the servo library (value between 0 and 180)
  ESC.write(potValue);    // Send the signal to the ESC

  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 2");  // refresh the page automatically every 5 sec
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");

            client.print("<p style=\"font-size:7vw;\">potValue: ");
            client.print(potValue);
            client.print(" <br></p>");

            printSlider(client);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void printSlider(WiFiClient client){
  client.print("<br><input type=\"range\" min=\"");
  client.print(-90);
  client.print("\" max=\"");
  client.print(90);
  client.print("\" value=\"");
  client.print(0);
  client.print("\" class=\"slider\" id=\"myRange\">");
  client.print("<p id=\"slider value\">space holder</p>");
  //range slider script
  client.print("<script>var slider = document.getElementById(\"myRange\");");
  client.print("var output = document.getElementById(\"slider value\");");
  client.print("output.innerHTML = slider.value;");
  client.print("slider.oninput = function() {output.innerHTML = this.value;console.log(slider.value);}</script>");
  //Serial.print(client.read());
}
