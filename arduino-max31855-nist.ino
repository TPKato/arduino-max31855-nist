#include <SPI.h>
#include "Adafruit_MAX31855.h"

#include "nist-its90.h"

#define MEASURE_TIME

#define DO   12
#define CS   11
#define CLK  10
Adafruit_MAX31855 thermocouple(CLK, CS, DO);

void setup()
{
  Serial.begin(9600);
  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop()
{
  double internalTemp = thermocouple.readInternal();
  double rawTemp = thermocouple.readCelsius();

  if (isnan(rawTemp)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
#ifdef MEASURE_TIME
    unsigned long time_start, time_end;

    time_start = micros();
#endif
    double correctedTemp = emf2temp_K((rawTemp - internalTemp) * 0.041276 + temp2emf_K(internalTemp));
    if (isnan(correctedTemp)) {
      Serial.print("Temperature is out of range. This should never happen.");
    }
#ifdef MEASURE_TIME
    time_end = micros();

    Serial.print(time_end - time_start);
    Serial.println(" us");

    Serial.print("Traw = ");
    Serial.println(rawTemp, 5);

    Serial.print("Tint = ");
    Serial.println(internalTemp, 5);
#endif

    Serial.print("Corrected Temp = ");
    Serial.println(correctedTemp, 5);
    Serial.println("");
  }

  delay(1000);
}
