#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Definisi pin VSPI untuk LoRa
#define LORA_SCK 18    // VSPI SCK
#define LORA_MISO 19   // VSPI MISO
#define LORA_MOSI 23   // VSPI MOSI
#define LORA_CS 5      // VSPI CS
#define LORA_RST 14    // Bisa diganti
#define LORA_DIO0 26   // Bisa diganti

// Counter untuk paket
int counter = 0;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Test with VSPI");
  
  // Konfigurasi VSPI
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  
  // Setup LoRa
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  
  // Konfigurasi tambahan LoRa (opsional)
  LoRa.setSpreadingFactor(7);     // range 6-12
  LoRa.setSignalBandwidth(125E3); // 125kHz
  LoRa.setCodingRate4(5);         // 4/5
  LoRa.setTxPower(20);            // 20dBm
  
  Serial.println("LoRa initialization successful!");
}

void loop() {
  // Kirim paket
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Mulai pengiriman
  LoRa.beginPacket();
  LoRa.print("Hello LoRa #");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  // Cek paket masuk
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet: '");
    
    // Baca paket
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  delay(5000);
}