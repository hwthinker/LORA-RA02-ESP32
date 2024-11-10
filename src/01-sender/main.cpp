//lora library by Sandeep Mistry
// lora Sender 915MHz V2.1-1.6
// Modified by HwThinker
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

#define SCK     18   
#define MISO    19   
#define MOSI    23   
#define SS      5   
#define RST     14  
#define DI0     26   
#define BAND    433E6

// #define LED_BUILTIN 25

int counter = 0;
int state = 0;


void setup() {
  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial); //If just the the basic function, must connect to a computer

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
}

void loop() {

  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(2200);
  //  digitalWrite(LED_BUILTIN, (state) ? HIGH : LOW);
  //  state = !state;
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}
