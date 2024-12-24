#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP32Time.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

unsigned long setEpochTime();
void setInteralClk();

void testdrawstyles();      // Draw characters of the default font
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//needed to connect to the internet
const char* ssid     = "Home5146";
const char* password = "Qwerty13!";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Defining ESP32Time rtc
ESP32Time rtc;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  rtc.setTime(setEpochTime(), 0);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  testdrawstyles();
  delay(500);
}

void testdrawstyles(void) {

  //update text
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(rtc.getTime());
  display.println(rtc.getDayofWeek());
  display.print(rtc.getMonth());
  display.print(rtc.getDay());
  display.print(rtc.getYear());
  display.display();
}
//connecting to the net to get the time
unsigned long setEpochTime(){
   // Initialize a NTPClient to get time
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //show the connection status on the OLED 
    display.print(".");
    display.display();
  }
  Serial.print("Connected");
  delay(500);
  display.clearDisplay();
  timeClient.begin();
  timeClient.setTimeOffset(-28800);
  timeClient.update();
  //ESP32Time mainly uses Epoch Time which will make include dates wayyy easier
  unsigned long t_return = timeClient.getEpochTime();
  timeClient.end();
  WiFi.disconnect();
  return t_return;
}
//use the time collected to set the internal RTC
void setInteralClk(){

}