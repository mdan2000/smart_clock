#include "FastLED.h"


// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 512

// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 33

// Создаем переменную strip для управления нашей лентой.
CRGB strip[LED_COUNT];

#define ROWS
#define COLS
CRGB matrix[ROW][COLS];

int LED_BUILTIN = 2;
void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(strip, LED_COUNT);
}
void loop() {
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Blue; // Красный цвет.
    FastLED.show();
    delay(100);
    strip[i] = CRGB::Black;
    FastLED.show();
  }
  FastLED.show();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
  }
  // Передаем цвета ленте.
  FastLED.show();
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
