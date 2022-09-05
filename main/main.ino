#include "FastLED.h"
#include "TextForRun.h"
#include "OutlookSetting.h"
#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define ENABLE_IMAP
#define ESP_MAIL_USE_PSRAM


// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 512

// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 33// Указываем, какое количество пикселей у нашей ленты.


// Define the global used SMTP Session object which used for SMTP transsport
SMTPSession smtp;

// Define the global used session config data which used to store the TCP session configuration
ESP_Mail_Session session;

IMAPSession imap;
IMAP_Config config;
int checkEmailIterator = 1;


const char* ssid     = "kek";
const char* password = "ordasosatt";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;
const int updateDelay = 80;

bool matrixForBottomRow[8][500];
String textForBottomRow = "       My Test String";
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
  if (!getLocalTime(&timeinfo)) {
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

void printPollingStatus(IMAPSession &imap)
{
  /* Declare the selected folder info class to get the info of selected mailbox folder */
  SelectedFolderInfo sFolder = imap.selectedFolder();

  /* Show the mailbox info */
  ESP_MAIL_PRINTF("\nMailbox status changed\n----------------------\nTotal Messages: %d\n", sFolder.msgCount());

  if (sFolder.pollingStatus().type == imap_polling_status_type_new_message)
  {

    ESP_MAIL_PRINTF("New message %d, has been addedd, reading message...\n", (int)sFolder.pollingStatus().messageNum);

    // if (sFolder.recentCount() > 0)
    //     ESP_MAIL_PRINTF("\nMesssage count which recent flag set: %d\n", sFolder.recentCount());

    // we need to stop polling before do anything
    imap.stopListen();

    // Get the UID of new message and fetch
    config.fetch.uid = imap.getUID(sFolder.pollingStatus().messageNum);
    MailClient.readMail(&imap, false);
  }
  else if (sFolder.pollingStatus().type == imap_polling_status_type_remove_message)
    ESP_MAIL_PRINTF("Message %d, has been removed\n\n", (int)sFolder.pollingStatus().messageNum);
  else if (sFolder.pollingStatus().type == imap_polling_status_type_fetch_message)
    ESP_MAIL_PRINTF("Message %d, has been fetched with the argument %s\n\n", (int)sFolder.pollingStatus().messageNum, sFolder.pollingStatus().argument.c_str());
}

void connectWiFi() {
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
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(strip, LED_COUNT);
  FastLED.clear();

  connectWiFi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Setting time");

  session.server.host_name = "outlook.office365.com"; //for outlook.com
  session.server.port = 993;
  session.login.email = outlookEmail;
  session.login.password = outlookPassword;

  // Connect to the server with the defined session and options

  // Open or select the mailbox folder to read the message
  session.network_connection_handler = connectWiFi;

  /* Connect to server with the session and config */
  Serial.println("Try to connect to imap");
  if (!imap.connect(&session, &config))
    return;
  Serial.println("Connecting to imap");

  /*  {Optional} */
  //printAllMailboxesInfo(imap);

  /* Open or select the mailbox folder to read or search the message */
  if (!imap.selectFolder(F("INBOX")))
    Serial.println("No folder INBOX");
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

  if (checkEmailIterator == 0) {
    Serial.println("Checking email");
    if (!imap.listen()) {
      imap.listen();
    }
      // Check the changes
      //if (imap.folderChanged())
        printPollingStatus(imap);
   // } else {
   //   Serial.println("Can't listen imap");
   // }
  }
  delay(updateDelay);
  checkEmailIterator++;
  checkEmailIterator %= 24;
}
