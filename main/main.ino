#include "FastLED.h"
#include "TextForRun.h"
#include <WiFi.h>

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 512

// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 33// Указываем, какое количество пикселей у нашей ленты.


const char* ssid     = "AnyhipeTrain";
const char* password = "TheBestWi-FiAtB212(2)";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;
const int updateDelay = 80;

bool matrixForBottomRow[8][500];
String textForBottomRow = "        My Test String";
int bottomRowWidth = 0;
int bottomRowIteration = 0;

// Создаем переменную strip для управления нашей лентой.
CRGB strip[LED_COUNT];

bool IS_DISPLAY_TIME = true;
bool IS_NEED_UPDATE_ROW_FOR_BOTTOM_LINE = true;
bool NEED_DISLAPY_BOTTOM_ROW = false;

String makeTwoDigitsString(int i) {
  if (i < 10) {
    return "0" + String(i); 
  } else {
    return String(i);
  }
}

void displayTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  String res = "";
  if (timeinfo.tm_sec % 2 == 0) {
    res = makeTwoDigitsString(timeinfo.tm_hour) + " " + makeTwoDigitsString(timeinfo.tm_min);
  } else {
    res = makeTwoDigitsString(timeinfo.tm_hour) + ":" + makeTwoDigitsString(timeinfo.tm_min);
  }
  displayMediumString(strip, res, 4, 9);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(strip, LED_COUNT);
  FastLED.clear();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

String getStringForBottomLine() {
  return "My Test String";
}

int test = 0;

void loop() {
  if (IS_DISPLAY_TIME) {
    displayTime();
  }
  if (IS_NEED_UPDATE_ROW_FOR_BOTTOM_LINE) {
    bottomRowWidth = textForBottomRow.length() * 5;
    getMatrixForMediumRow(matrixForBottomRow[0], textForBottomRow, bottomRowWidth);
    IS_NEED_UPDATE_ROW_FOR_BOTTOM_LINE = false;
    NEED_DISLAPY_BOTTOM_ROW = true;
    bottomRowIteration = 0;
  }
  if (NEED_DISLAPY_BOTTOM_ROW) {
    displayMatrixForMediumRow(strip, matrixForBottomRow[0], bottomRowWidth, 0, bottomRowIteration++);
    bottomRowIteration %= bottomRowWidth;
    if (bottomRowIteration == 0) {
      NEED_DISLAPY_BOTTOM_ROW = false;
    }
  }
  delay(updateDelay);
}
