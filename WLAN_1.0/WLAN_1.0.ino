/*
  * Copyright 2023 Jörg Bischof
  *
  * Licensed under the EUPL, version 1.2 or - when
    which have been approved by the European Commission -
    Subsequent versions of the EUPL ("License").
  * You may use this work solely in accordance with this license.
  * A copy of the license can be found here:
  *
  * https://joinup.ec.europa.eu/software/page/eupl
  *
  * Unless required by applicable legislation or
    Agreed in writing, the subject of the license
    distributed software "as is",
    WITHOUT ANY WARRANTY OR CONDITIONS - EXPRESS OR
    tacit - spread.
  * The language-specific permissions and restrictions under the
    license can be found in the license text.
*/

#include <ESP8266WiFi.h>

const char *ssid = "SSID des WLAN-Routers";
const char *pw = "das Zugangspasswort";

WiFiServer server(80);
WiFiClient client;

String header = "";
byte ant[] = { 0, 2, 5, 12, 4 };
String text[] = { "GND", "ANT1", "ANT2", "ANT3", "ANT4" };

String styl = R"(
    <style>
      html {
        font-familiy: 'Helvetica Neue', Helvetica, Arial, sans-serif;
        font-size: 10px;
        display: inline-block;
        margin: 0, auto;
        text-align: center;
      }
      button {
        background: lightgray;
        border: none;
        color: white;
        margin: 2px;
        height: 6rem;
        width: 80%;
        max-width: 50rem;
        text-decoration: none;
        text-align: center;
        font-size: 2rem;
        font-weight: medium;
        cursor: pointer;
      }
      .on {
        background: red;
      }
    </style>
)";

void antOn(byte a);

void setup() {
  Serial.begin(115200);
  for (byte i = 1; i <= 4; i++) {
    pinMode(ant[i], OUTPUT);
    digitalWrite(ant[i], LOW);
  }

  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected: ");
  Serial.println("IP-Adresse: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          Serial.println(header);
          if (currentLine.length() == 0) {
            client.print("http/1.1 200 OK\r\n");
            client.print("Content-type: text/html\r\n\r\n");
            client.println("<!DOCTYPE html><html>");
            client.println("<head>");
            client.println("<title>WLAN-Schalter</title>");
            client.println("<meta name='viewport' content='width=device-width, inital-scale=1.0'>");
            client.println(styl);
            client.println("</head><body>");
            if (header.indexOf("GET /ant0") < 0 && header.indexOf("GET /ant1") < 0 && header.indexOf("GET /ant2") < 0 && header.indexOf("GET /ant3") < 0 && header.indexOf("GET /ant4") < 0) {
              client.println("<p><a href='/ant0'><button type='button'>Start</button></a></p>");
            } else {
              if (header.indexOf("GET /ant0") >= 0) {
                antOn(0);
              } else if (header.indexOf("GET /ant1") >= 0) {
                antOn(1);
              } else if (header.indexOf("GET /ant2") >= 0) {
                antOn(2);
              } else if (header.indexOf("GET /ant3") >= 0) {
                antOn(3);
              } else if (header.indexOf("GET /ant4") >= 0) {
                antOn(4);
              }
            }
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println();
  }
}
