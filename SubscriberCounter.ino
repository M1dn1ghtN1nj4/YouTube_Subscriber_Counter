#include <Wire.h>
#include <ESP8266WiFi.h>
#include <YoutubeApi.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

const char* ssid     = "SSID-HERE";      // SSID of local network
const char* password = "PASSWORD-HERE";  // Password on network
String API_KEY = "API-KEY-HERE";         // YouTube Data API v3 key generated here: https://console.developers.google.com
String CHANNEL_ID = "CHANNEL-ID-HERE";   // YouTube channel id

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

// Time between API requests
// Make sure this is OVER about 9 seconds (9000) otherwise
// you will exceed your 10000 request quota.  You can hit your
// 10000 requests if you set it to 8.64 seconds.  So to stay
// in the clear, keep this around 20000 to 30000 (20s to 30s).
unsigned long api_mtbs = 30000;
unsigned long api_lasttime;    //last time api request was made
long subs = 0;

void setup() {
  lcd.begin(20,4);
  lcd.clear();
  //Serial.begin(9600);
  int cursorPosition=0;

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.hostname("ESP8266SubscriberCounter");
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  lcd.print("Connecting");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(cursorPosition,1); 
    lcd.print(".");
    cursorPosition++;
  }

  lcd.clear();
  lcd.print("Connected!");
  delay(2000);

if(api.getChannelStatistics(CHANNEL_ID))
    {
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("YOUR-CHANNEL-NAME-HERE");
      lcd.setCursor(0,1);
      lcd.println("------------------------");
      lcd.setCursor(1,2);
      lcd.print("Subscribers:");
      lcd.setCursor(0,3);
      lcd.print("Total views:");
    }
}

void loop() {
if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      lcd.setCursor(15,2);
      lcd.print(api.channelStats.subscriberCount);
      lcd.setCursor(14,3);
      lcd.print(api.channelStats.viewCount);
    }
    api_lasttime = millis();
  }
}
