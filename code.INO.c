#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// WiFi Configuration
const char* ssid = "xxxxx";
const char* password = "xxxxxx";


int relayPin = 5;               // Pin Relay
ESP8266WebServer server(80);  
String message = "";
int cpt=0;



void handleRoot() {
  Serial.println("HTTP GET /");
  message = "<html><body>";
  message += "<h1>Relay OLIMEX #1</h1>";
    message += "<ul>";
      message += "<li><a href='/on' >Set Relay = ON</a></li>";
      message += "<li><a href='/off'>Set Relay = OFF</a></li>";
      message += "<li><a href='/gate'>OpenGate</a></li>";
      message += "<li><a href='/status'>Actual state of relay</a></li>";
    message += "</ul>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handleRelayOn() {
  Serial.println("Relay go -> ON");
  digitalWrite(relayPin, HIGH);
   message = "<html><body>";
   message += "<h1>Relay is ON !</h1>";
    message += "<ul>";
      message += "<li><a href='./off'>Set Relay = OFF</a></li>";
      message += "<li><a href='./'>Root</a></li>";
    message += "</ul>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handleGate(){
  digitalWrite(relayPin, HIGH);
  delay(500);
  digitalWrite(relayPin, LOW);
  message = "ok";
  server.send(200, "text/html", message);
}

void handleRelayOff() {
  Serial.println("Relay go -> OFF");
  digitalWrite(relayPin, LOW);
   message = "<html><body>";
   message += "<h1>Relay is OFF !</h1>";
    message += "<ul>";
      message += "<li><a href='./on' >Set Relay = ON</a></li>";
      message += "<li><a href='./'>Root</a></li>";
    message += "</ul>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}


void handleStatus() {
  Serial.println("Ask STATUS");
   message = "<html><body>";
   if ( digitalRead(relayPin) == 1 ) {
      message += "<h1>Status = ON</h1>";  
      message += "<ul>";
        message += "<li><a href='/off'>Commutate State = OFF</a></li>";
      }
   else {
      message += "<h1>Status = OFF</h1>";  
      message += "<ul>";
        message += "<li><a href='/on' >Commutate State = ON</a></li>";
      }
        message += "<li><a href='./'>Actual</a></li>";
        message += "</ul>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handleRelayMOn() {
  Serial.println("Relay Go ON");
  digitalWrite(relayPin, HIGH);
  message = "ON";
  server.send(200, "text/html", message);
}

void handleRelayMOff() {
  digitalWrite(relayPin, LOW);
  message = "OFF";
  server.send(200, "text/html", message);
}

void handleMStatus() {
   if ( digitalRead(relayPin) == 1 ) {
      message = "ON";  
      }
   else {
      message = "OFF";  
      }
  server.send(200, "text/html", message);
}

void handleNotFound() {
   message = "<html><body>";
   message += "<h1>Page not Found !</h1>";
   message += "</body></html>";
   server.send(200, "text/html", message);
  
}

//--------------------------
void setup() {
  //Initialization
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting ...");
  pinMode(relayPin, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connesso a : ");
  Serial.println(ssid);
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleRelayOn);
  server.on("/gate",handleGate);
  server.on("/off", handleRelayOff);
  server.on("/status", handleStatus);
  server.on("/mon", handleRelayMOn);
  server.on("/moff", handleRelayMOff);
  server.on("/mstatus", handleMStatus);  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Server HTTP is running");
}

//--------------------------
void loop() {
  delay(10);
  cpt++;
  server.handleClient();
  if ( (cpt % 50) == 0 ) {
      Serial.print(".");
      if ( (cpt % 1000) == 0 ) {
          cpt = 0;
          Serial.println(".");
          }
      }
}