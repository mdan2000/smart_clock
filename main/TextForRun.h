#include <FastLED.h>
#include "font.h"

#define LED_PIN 33

#define LED_COUNT 512
#define MATRIX_WIDTH 32         
#define MATRIX_HEIGHT 16 
#define BRIGHTNESS 30

//константы и переменные для работы с текстом
const String myText=" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя";

int getIndexOfPixel(int x, int y) {
  int res = 0;
  if (y > MATRIX_HEIGHT / 2) {
    res += 256;
    res += (MATRIX_WIDTH - x) * (MATRIX_HEIGHT / 2);
    y -= (MATRIX_HEIGHT / 2);
    if (x % 2 == 0) {
      res += (y - 1);
    } else {
      res += (MATRIX_HEIGHT / 2) - y;
    }
  } else {
    res = (x - 1) * (MATRIX_HEIGHT / 2);
    if (x % 2 == 0) {
      res += y - 1;
    } else {
      res += (MATRIX_HEIGHT / 2) - y;
    }
  }
  return res;
}

void setChar(bool* matrix, char c, int x, int y, int m) {
  int pos = myText.indexOf(c);
  //const uint8_t charByHex[5] = fontHEX[pos];
  for (int dx = 0; dx <= 4; dx++) {
    uint8_t val = fontHEX[pos][dx];
    for (int dy = 7; dy >= 0; dy--) {
      int pix = getIndexOfPixel(x + dx, y + dy);
      //Serial.println((y + dy)*m + x + dx);
      if (val % 2 == 0) {
        matrix[(y + dy - 1)*m + x + dx - 1] = false;
      } else {
        matrix[(y + dy - 1)*m + x + dx - 1] = true;
      }
      val /= 2;
    }
  }
}

void displayMediumChr(CRGB strip[], char c, int x, int y) {
  int pos = myText.indexOf(c);
  //const uint8_t charByHex[5] = fontHEX[pos];
  for (int dx = 0; dx <= 4; dx++) {
    uint8_t val = fontHEX[pos][dx];
    for (int dy = 7; dy >= 0; dy--) {
      int pix = getIndexOfPixel(x + dx, y + dy);
      if (val % 2 == 0) {
        strip[pix] = CRGB::Black;
      } else {
        strip[pix] = CRGB::Red;
      }
      val /= 2;
    }
  }
  FastLED.show();
}

void displayMediumString(CRGB strip[], String s, int x, int y) {
  const int chrWidth = 5;
  if (x + s.length() * chrWidth - 1 > MATRIX_WIDTH) {
    return;
  } else {
    for (int i = 0; i < s.length(); ++i) {
      displayMediumChr(strip, s[i], x + i * 5, y);
    }
  }
}

void getMatrixForMediumRow(bool* myRow, String s, int matrixWidth) {
  for (int i = 0; i < s.length(); ++i) {
    setChar(myRow, s[i], 5 * i + 1, 1, matrixWidth);
  }
}

void displayMatrixForMediumRow(CRGB* strip, bool* myRow, int matrixWidth, int row, int iteration) {
  for (int y = 1; y <= MATRIX_HEIGHT/2; ++y) {
      for (int x = 1; x <= MATRIX_WIDTH; ++x) {
        if (myRow[(y - 1)*matrixWidth + ((x - 1 + iteration) % matrixWidth)]) {
          strip[getIndexOfPixel(x, row + y)] = CRGB::Red;
          //Serial.print(1);
        } else {
          strip[getIndexOfPixel(x, row + y)] = CRGB::Black;
          //Serial.print(0);
        }
      }
      //Serial.println();
    }
    //Serial.println("Next Matrix");
    FastLED.show();
}

/*void displayMediumRunRow(CRGB* strip, bool* myRow, String s, int row) {
  const int m = s.length() * 5;
  Serial.println("Before making matrix");
  Serial.println("Fuck this shit");
  for (int i = 0; i < s.length(); ++i) {
    Serial.println(i);
    setChar(myRow, s[i], 5 * i + 1, 1, m);
  }
  /*Serial.println("After making matrix");
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < s.length() * 5; ++j) {
      if (myRow[i][j]) {
        Serial.print("1");
      } else {
        Serial.print("0");
      }
      delay(10);
    }
    Serial.println();
  }
  Serial.println("allmatrix");
  return;
  for (int i = 0; i < s.length() * 5; ++i) {
    for (int y = 1; y <= MATRIX_HEIGHT/2; ++y) {
      for (int x = 1; x <= MATRIX_WIDTH; ++x) {
        if (myRow[y - 1][(x - 1 + i) % m]) {
          strip[getIndexOfPixel(x, row + y)] = CRGB::Red;
          //Serial.print(1);
        } else {
          strip[getIndexOfPixel(x, row + y)] = CRGB::Black;
          //Serial.print(0);
        }
      }
      //Serial.println();
    }
    //Serial.println("Next Matrix");
    FastLED.show();
    delay(80);
  }
}*/
