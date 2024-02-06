/*
  WiFi Web Server

 A simple web server that shows the value of the analog input pins.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * Analog inputs attached to pins A0 through A5 (optional)

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe


  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-server
 */

#include "WiFiS3.h"

#include <Servo.h>

Servo ESC;     // create servo object to control the ESC

int potValue;  // value from the analog pin

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

String header;

WiFiServer server(80);

void setup() {
  // Attach the ESC on pin 9
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  //potValue = map(potValue, 0, 1023, -90, 90);   // scale it to use it with the servo library (value between 0 and 180)
  //ESC.write(potValue);    // Send the signal to the ESC

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply

        header += c;


        if (c == '\n' && currentLineIsBlank) {
          Serial.println(header.indexOf("p#slider\value"));//see what was captured

          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          //client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }

          //client.println("<br><input type=\"range\" min=\"-90\" max=\"90\" value=\"0\" class=\"slider\" id=\"myRange\"><p id=\"slider value\">space holder</p><script>var slider = document.getElementById(\"myRange\");var output = document.getElementById(\"slider value\");output.innerHTML = slider.value;slider.oninput = function() {output.innerHTML = this.value;}</script>");
          
          printSlider(client);

          if(header.indexOf("9") >= 0){
              Serial.println(header.indexOf("9"));
          }

          //potValue = slider.value;

          //ESC.write(potValue);    // Send the signal to the ESC
          client.print("<p style=\"font-size:7vw;\">potValue: ");
          client.print(potValue);
          client.print(" <br></p>");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
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
