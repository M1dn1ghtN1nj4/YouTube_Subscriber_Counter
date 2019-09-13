#include <Wire.h>
#include <ESP8266WiFi.h>
#include <YoutubeApi.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

const char* ssid     = "SSID-HERE";       // SSID of local network
const char* password = "PASSWORD-HERE";   // Password on network
String API_KEY = "API-KEY-HERE";            // YouTube Data API v3 key generated here: https://console.developers.google.com
String CHANNEL_ID = "CHANNEL-ID-HERE";   // YouTube channel id

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address



WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 5000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done
long subs = 0;

void setup() {
  Serial.begin(9600);
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.hostname("ESP8266SubscriberCounter"); //This changes the hostname of the ESP8266 to display neatly on the network router.
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  //Set up the LCD
  lcd.begin(20,4); 
  lcd.setCursor(0,0);
  delay(2000);
  lcd.clear();
}






void loop() {
   
if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      //Serial.println("---------Stats---------");
      //Serial.print("Subscriber Count: ");
      //Serial.println(api.channelStats.subscriberCount);
      //Serial.print("View Count: ");
      //Serial.println(api.channelStats.viewCount);
      //Serial.print("Comment Count: ");
      //Serial.println(api.channelStats.commentCount);
      //Serial.print("Video Count: ");
      //Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      //Serial.println("------------------------");
      //SaveStats();
      lcd.clear();
      lcd.print(api.channelStats.subscriberCount);

      
    }
    api_lasttime = millis();
  }
}

void SaveStats() {
  lcd.print(String(api.channelStats.subscriberCount));
  lcd.print(api.channelStats.subscriberCount);
  }
