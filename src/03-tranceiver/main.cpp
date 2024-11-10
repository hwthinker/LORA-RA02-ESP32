#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Define pin yang digunakan
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DI0 26
#define BAND 433E6

// Variable untuk counter
int counter = 0;
String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Transceiver");

  // Configure pins
  LoRa.setPins(SS, RST, DI0);

  // Inisialisasi LoRa
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Set parameters LoRa
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.enableCrc();

  Serial.println("LoRa Initializing OK!");
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // Start packet
  LoRa.write(destination);              // Add destination address
  LoRa.write(localAddress);             // Add sender address
  LoRa.write(msgCount);                 // Add message ID
  LoRa.write(outgoing.length());        // Add payload length
  LoRa.print(outgoing);                 // Add payload
  LoRa.endPacket();                     // Finish packet and send it
  msgCount++;                           // Increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // If there's no packet, return

  // Read packet header bytes:
  int recipient = LoRa.read();          // Recipient address
  byte sender = LoRa.read();            // Sender address
  byte incomingMsgId = LoRa.read();     // Incoming msg ID
  byte incomingLength = LoRa.read();    // Incoming msg length

  String incoming = "";                  // Payload of packet

  while (LoRa.available()) {            // Can't use readString() in callback
    incoming += (char)LoRa.read();      // Add bytes one by one
  }

  if (incomingLength != incoming.length()) {   // Check length for error
    Serial.println("Error: Message length does not match length");
    return;
  }

  // If message is not for this device, ignore it
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;
  }

  // Print received message
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}

void loop() {
  // Setiap 5 detik, kirim pesan
  if (millis() % 5000 == 0) {
    String message = "Counter: " + String(counter);
    sendMessage(message);
    Serial.println("Sending: " + message);
    counter++;
    delay(1);  // Prevent sending multiple times in same millisecond
  }

  // Parse any received packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    onReceive(packetSize);
  }
}