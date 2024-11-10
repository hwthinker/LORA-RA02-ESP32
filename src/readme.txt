// Sender Code (sender.cpp)
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Pin definitions
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DI0 26
#define BAND 433E6

int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  // Setup LoRa transceiver module
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // Setup LoRa parameters
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();
  
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Send LoRa packet
  LoRa.beginPacket();
  LoRa.print("Counter: ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(5000); // Send every 5 seconds
}

// Receiver Code (receiver.cpp)
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Pin definitions
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DI0 26
#define BAND 433E6

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  // SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  // Setup LoRa transceiver module
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // Setup LoRa parameters (same as sender)
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();
  
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet '");

    // Read packet
    while (LoRa.available()) {
      String message = LoRa.readString();
      Serial.print(message);
    }

    // Print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}