#include <WiFi.h>
#include "Thing.CoAP.h"

Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

const char* ssid = "SSID";
const char* password = "PSWD";

IPAddress serverIp(192, 168, 1, 10);
const uint16_t serverPort = 8080;

unsigned long lastSendTime = 0;            
const unsigned long sendInterval = 60000;  

void sendRandomText() {
  // Generate random payload size between 1024 and 2048 bytes
  size_t payloadSize = 1024 + random(1024);

  // Create a random payload
  std::string message(payloadSize, ' ');
  for (size_t i = 0; i < payloadSize; ++i) {
    message[i] = 'A' + (rand() % 26);  
  }

  // Append current boot-up time (millis()) to the message
  // Get current time when sending
  unsigned long currentSendTime = millis();
  
  // Add client send time to the message
  message += "\nClientSendTime: " + std::to_string(currentSendTime);

  // Convert message to vector for CoAP
  std::vector<uint8_t> payload(message.begin(), message.end());

  // Send the payload
  coapClient.Put("upload", payload, [&](Thing::CoAP::Response response) {
    auto responsePayload = response.GetPayload();
    if (!responsePayload.empty()) {
      std::string received(responsePayload.begin(), responsePayload.end());
      Serial.println("Server Response:");
      Serial.println(received.c_str());
    } else {
      Serial.println("No response payload received.");
    }
  });

  Serial.printf("Sent payload of size: %d bytes at time %lu ms\n", payloadSize, currentSendTime);
}

void setup() {
  Serial.begin(115200);

  // WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // CoAP client setup
  coapClient.SetPacketProvider(udpProvider);
  coapClient.Start(serverIp, serverPort);

  // Seed random number generator
  randomSeed(micros());
}

void loop() {
  coapClient.Process();

  // Send a message every interval
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= sendInterval) {
    sendRandomText();
    lastSendTime = currentMillis;
  }
}
