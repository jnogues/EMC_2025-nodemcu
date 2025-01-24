// Tester placa nodeMcu
#include <Temperature_LM75_Derived.h>
Generic_LM75_12Bit temperature;//12 bits 0.0625ºC 220ms

#define TASKER_MAX_TASKS 10
#include "Tasker.h"
Tasker tasker;

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 15
CRGB leds[NUM_LEDS];

void setup() 
{
  // Configuracio Serial
  Serial.begin(115200);
  delay(100);
  Wire.begin();// Inicia I2C
  Serial.println("Test placa 2025");
  
  // Encenem leds
  pinMode(16, OUTPUT);
  digitalWrite(16 , LOW);
  pinMode(0, OUTPUT);
  digitalWrite(0 , LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2 , LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13 , HIGH);

  // Configurem Neopixel  
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  // Benvinguda del Neopixel
  delay(1000);
  leds[0].setRGB( 0,0,0);
  FastLED.show(); 
  delay(5000);
  leds[0].setRGB( 255,0,0);
  FastLED.show();
  delay(1000);
  leds[0].setRGB( 0,255,0);
  FastLED.show();
  delay(1000);
  leds[0].setRGB( 0,0,255);
  FastLED.show();
  delay(1000);
  
  tasker.setInterval(intermitaLeds,1000);
  tasker.setInterval(mesuraTemperatura,2000);
  tasker.setInterval(llumetesAleatories,3000);
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
  byte red = random(0,100);
  byte green = random(0,100);
  byte blue = random(0,100);
  leds[0].setRGB( red,green,blue); 
  FastLED.show(); 
}

// Tasca 4
void valorAnalogic()
{
  Serial.print("A0 = ");
  Serial.println( analogRead(A0) );
}
