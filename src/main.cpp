#include <Arduino.h>

void setup() {
    pinMode(2, OUTPUT); // GPIO 2 biasanya terhubung ke LED di beberapa board ESP32
}

void loop() {
    digitalWrite(2, HIGH); // Nyalakan LED
    delay(1000);           // Tunggu 1 detik
    digitalWrite(2, LOW);  // Matikan LED
    delay(1000);           // Tunggu 1 detik
}
