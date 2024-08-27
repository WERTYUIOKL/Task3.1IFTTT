#include <WiFiNINA.h>

char ssid[] = "vivo"; //WiFi SSID
char pass[] = "rakeshsingh"; //WiFi password
const char* webhookUrl = "/trigger/Sunlight/with/key/eWOeuigjkAgVhRWAI3SQ_4meFLCpv17LcbjMp9ZF-40"; // URL path for IFTTT

WiFiClient client;

// Pin connected to the LM393 digital output
const int LIGHT_SENSOR_PIN = A5;

void setup() {
  Serial.begin(9600);
  while (!Serial); 
  // Initializes the light sensor pin
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // Connects to WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Test connection to the IFTTT server
  if (client.connect("maker.ifttt.com", 80)) {
    Serial.println("Connected to IFTTT server");
    client.stop();  // Close the test connection
  } else {
    Serial.println("Connection to IFTTT server failed");
  }
}

void loop() {
  // Read the light sensor value (HIGH or LOW)
  int lightValue = digitalRead(LIGHT_SENSOR_PIN);
  Serial.print("Light Sensor State (raw): ");
  Serial.println(lightValue); 

  
  if (lightValue == HIGH) {
    Serial.println("No light detected");
    sendIFTTTNotification("Sunlight stopped!");
  } else {
    Serial.println("Light detected");
    sendIFTTTNotification("Sunlight detected on the terrarium!");
  }
  
  delay(60000);  // 60 seconds delay
}


void sendIFTTTNotification(String message) {
  message.replace(" ", "+");

  // Connects to IFTTT and sends an HTTP GET request
  if (client.connect("maker.ifttt.com", 80)) {
    Serial.println("Sending notification...");
    client.println("GET " + String(webhookUrl) + "?value1=" + message + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println(); // End of HTTP header

    // Read the server response
    while (client.connected() || client.available()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    // Disconnect from the server
    client.stop();
    Serial.println();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Failed to connect to IFTTT server");
  }
}
