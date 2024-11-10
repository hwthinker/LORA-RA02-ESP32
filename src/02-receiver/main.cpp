#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define SCK     18    
#define MISO    19   
#define MOSI    23  
#define SS      5   
#define RST     14  
#define DI0     26   
#define BAND    433E6

// Pin LED
// #define LED_PIN 25

void setup()
{
  // Konfigurasi pin LED
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, LOW); // Mulai dengan LED mati

  Serial.begin(9600);
  while (!Serial)
    ;

  // Konfigurasi pin LoRa
  LoRa.setPins(SS, RST, DI0);

  // Inisialisasi LoRa
  if (!LoRa.begin(BAND))
  {
    Serial.println("Gagal menginisialisasi LoRa");
    while (1)
      ;
  }
  Serial.println("LoRa inisialisasi berhasil!");
}

void loop()
{
  // Cek apakah ada paket yang diterima
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    String receivedMessage = "";

    while (LoRa.available())
    {
      receivedMessage += (char)LoRa.read();
    }

    Serial.print("Pesan diterima: ");
    Serial.println(receivedMessage);
    // digitalWrite(LED_PIN,!digitalRead(LED_PIN));
  }
}
