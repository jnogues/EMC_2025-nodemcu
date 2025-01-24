#include <Temperature_LM75_Derived.h>
Generic_LM75_12Bit temperature;//12 bits 0.0625ºC 220ms

#define TASKER_MAX_TASKS 10
#include "Tasker.h"
Tasker tasker;

#include <Adafruit_NeoPixel.h>
#define PIN  15
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // Configuracio Serial
  Serial.begin(115200);
  delay(100);
  Wire.begin();// Inicia I2C
  Serial.println("Test placa 2025");

  // Configurem Neopixel
  pixels.begin();   
  
  // Encenem leds
  pinMode(16, OUTPUT);
  digitalWrite(16 , LOW);
  pinMode(0, OUTPUT);
  digitalWrite(0 , LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2 , LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13 , HIGH);

  
  // Benvinguda del Neopixel
  
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show(); 
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show(); 
  delay(1000);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show(); 
  delay(1000);
  pixels.clear();
  pixels.show(); 
  delay(1000);
  
  tasker.setInterval(intermitaLeds,1000);
  tasker.setInterval(mesuraTemperatura,2000);
  tasker.setInterval(llumetesAleatories,1000);
  tasker.setInterval(valorAnalogic, 300);

}

void loop() 
{
  tasker.loop();
}

// Tasques

// Tasca 1
void intermitaLeds()
{
  digitalWrite(16, !digitalRead(16));
  digitalWrite(0, !digitalRead(0));
  digitalWrite(2, !digitalRead(2));
  digitalWrite(13, !digitalRead(13));
}

// Tasca 2
void mesuraTemperatura()
{
  Serial.println(temperature.readTemperatureC());
}

// Tasca 3
void llumetesAleatories()
{
  byte red = random(0,50);
  byte green = random(0,50);
  byte blue = random(0,50);
  pixels.setPixelColor(0, pixels.Color(green, red, blue));
  pixels.show(); 
}

// Tasca 4
void valorAnalogic()
{
  Serial.print("A0 = ");
  Serial.println( analogRead(A0) );
}
