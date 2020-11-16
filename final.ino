#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//mode i2c hardware
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

static const unsigned char MDlogo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x03, 0x00, 0xFE, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x03, 
  0x00, 0xFE, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0xFC, 0xFF, 0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x03, 0x00, 0x00, 0xFC, 0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFC, 0xFF, 0x07, 
  0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 
  0xFC, 0xFF, 0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x3F, 0x00, 0xFC, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0xFF, 0xFF, 0x0F, 
  0x80, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
  0x80, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x01, 0x80, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xFC, 0xFF, 0xFF, 0x1F, 
  0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 
  0xFC, 0xFF, 0xFF, 0x1F, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x03, 0x80, 0xFF, 0xFF, 0x1F, 0xC0, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0xFF, 0xFF, 0x1F, 
  0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 
  0xFC, 0xFF, 0xFF, 0x3F, 0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xC3, 0xFF, 0xFF, 0x0F, 0xFC, 0xFF, 0xFF, 0x3F, 0xE0, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0xFF, 0x3F, 
  0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0xC7, 0x81, 0xE3, 0xFF, 0x0F, 
  0x80, 0xFF, 0xFF, 0x3F, 0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0x03, 
  0x00, 0xC0, 0xFF, 0x1F, 0xFC, 0xFF, 0xFF, 0x7F, 0xF0, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0x01, 0x00, 0x80, 0xFF, 0x1F, 0xFC, 0xFF, 0xFF, 0x7F, 
  0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0x01, 0x00, 0x80, 0xFF, 0x1F, 
  0x80, 0xFF, 0xFF, 0x7F, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0x01, 
  0x00, 0x80, 0xFF, 0x3F, 0x80, 0xFF, 0xFF, 0x7F, 0xF0, 0xFF, 0xFF, 0xFF, 
  0x00, 0xF8, 0xFF, 0x03, 0x7E, 0xC0, 0xFF, 0x3F, 0xFC, 0xFF, 0xFF, 0xFF, 
  0xF8, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0x03, 0xFF, 0xC0, 0xFF, 0x3F, 
  0xFC, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xFF, 0x81, 
  0xFF, 0x81, 0xFF, 0x3F, 0x00, 0xFC, 0xFB, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 
  0x00, 0xF8, 0x7F, 0xC0, 0xFF, 0x03, 0xFF, 0x3F, 0x00, 0xFC, 0xFB, 0xFF, 
  0xFD, 0xFF, 0xFE, 0xFF, 0x00, 0xF8, 0x3F, 0xC0, 0xFF, 0x03, 0xFE, 0x3F, 
  0xFC, 0xFF, 0xFB, 0xFF, 0xFD, 0xFF, 0xFE, 0xFF, 0x00, 0xF8, 0x3F, 0xC0, 
  0xFF, 0x03, 0xFE, 0x3F, 0xFC, 0xFF, 0xFB, 0xFF, 0xFD, 0xFF, 0xFE, 0xFF, 
  0x00, 0xF8, 0x3F, 0xC0, 0xFF, 0x03, 0xFE, 0x3F, 0x80, 0xFF, 0xF3, 0xFF, 
  0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0xF8, 0x3F, 0xC0, 0xFF, 0x03, 0xFE, 0x3F, 
  0x80, 0xFF, 0xF3, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0xF8, 0x7F, 0xC0, 
  0xFF, 0x03, 0xFF, 0x3F, 0xFC, 0xFF, 0xF3, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x3F, 0xFC, 0xFF, 0xF3, 0xFF, 
  0xFF, 0x7F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0x03, 0xFF, 0xC0, 0xFF, 0x3F, 
  0x80, 0xFF, 0xE3, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0x03, 
  0x7E, 0xC0, 0xFF, 0x3F, 0x80, 0xFF, 0xE3, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0x01, 0x00, 0x80, 0xFF, 0x3F, 0xFC, 0xFF, 0xE3, 0xFF, 
  0xFF, 0x3F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0x01, 0x00, 0x80, 0xFF, 0x3F, 
  0xFC, 0xFF, 0xE3, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0x01, 
  0x00, 0x80, 0xFF, 0x3F, 0x80, 0xFF, 0xC3, 0xFF, 0xFF, 0x1F, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0x03, 0x00, 0xC0, 0xFF, 0x1F, 0x80, 0xFF, 0xC3, 0xFF, 
  0xFF, 0x1F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xC7, 0x81, 0xE3, 0xFF, 0x1F, 
  0xFC, 0xFF, 0xC3, 0xFF, 0xFF, 0x1F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0x81, 0xFF, 0xFF, 0x1F, 0xFC, 0xFF, 0xC3, 0xFF, 0xFF, 0x1F, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xC3, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0x83, 0xFF, 
  0xFF, 0x0F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 
  0x80, 0xFF, 0x83, 0xFF, 0xFF, 0x0F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x0F, 0xFC, 0xFF, 0x83, 0xFF, 0xFF, 0x0F, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFC, 0xFF, 0x83, 0xFF, 
  0xFF, 0x0F, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 
  0x80, 0xFF, 0x03, 0xFF, 0xFF, 0x07, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x03, 0x80, 0xFF, 0x03, 0xFF, 0xFF, 0x07, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFC, 0xFF, 0x03, 0xFF, 
  0xFF, 0x07, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 
  0xFC, 0xFF, 0x03, 0xFF, 0xFF, 0x07, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x00, 0xFC, 0xFF, 0x03, 0xFE, 0xFF, 0x03, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xFC, 0xFF, 0x03, 0xFE, 
  0xFF, 0x03, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 
  0xFC, 0xFF, 0x03, 0xFE, 0xFF, 0x03, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x1F, 0x00, 0xFC, 0xFF, 0x03, 0xFE, 0xFF, 0x03, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0xFC, 0xFF, 0x03, 0xFC, 
  0xFF, 0x01, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 
  0xFC, 0xFF, 0x03, 0xFC, 0xFF, 0x01, 0xFE, 0xFF, 0x00, 0xF8, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x01, 0x00, 0xFC, 0xFF, 0x03, 0xFC, 0xFF, 0x01, 0xFE, 0xFF, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, };

#define PIN            15
#define NUMPIXELS      25
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

const char *ssid     = "HAMMER INNOVATION LAB";
const char *password = "curiosityisgood";
//const long utcOffsetInSeconds = 25200;//WIB
const long utcOffsetInSeconds = 28800;//WITA
//const long utcOffsetInSeconds = 32400;//WIT

char daysOfTheWeek[7][12] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
int clockcounter=0;
int ledstate=1;
int buttonpressedstate=1;
int loopcounter=0;

void setup(){
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
  
  u8g2.begin(); 
  u8g2.clearBuffer();          
  u8g2.drawXBM(0,0, 128, 64, MDlogo);
  u8g2.sendBuffer(); 
  delay(2000);

  pinMode(14, INPUT_PULLUP);
  pixels.begin();
  ledmode1();
  
}

void loop() {
  
    if(clockcounter==0){
      clockcounter++;
      timeClient.update();
      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      Serial.print(", ");
      Serial.print(timeClient.getHours());
      Serial.print(":");
      Serial.print(timeClient.getMinutes());
      Serial.print(":");
      Serial.println(timeClient.getSeconds());
      //Serial.println(timeClient.getFormattedTime());
      updatediaplay(timeClient.getHours(),timeClient.getMinutes(),timeClient.getDay());
    }else{
      clockcounter++;
      if(clockcounter==239){
        clockcounter=0;
      }
    }


    //change led mode
    if (digitalRead(14) == LOW ){
      buttonpressedstate=0;
    }else if (digitalRead(14) == HIGH && buttonpressedstate==0){
      buttonpressedstate=1;
      if(ledstate==4){
        ledstate=1;
      }else{
        ledstate++;
      }
    }

  //led mode
  if(ledstate==1){
    ledmode1();
  }else if(ledstate==2){
    ledmode2();
  }else if(ledstate==3){
    ledmode3();
  }else if(loopcounter==4){
    ledmode4(loopcounter);
  }
 
   delay(250);
   loopcounter++;
}

void updatediaplay(int gethour,int getminute,int getday){
  String chartemp = String(gethour)+":"+String(getminute);
  String chartemp2 =daysOfTheWeek[getday];
  u8g2.clearBuffer();          
  u8g2.setFont(u8g2_font_fub35_tf); 
  //u8g2.drawStr(0,10,"test");  
  //u8g2.print(F("ORIGINAL IN FLASH"));
  u8g2.setCursor(0, 36);
  u8g2.print(chartemp);
  u8g2.setFont(u8g2_font_fub17_tf); 
  u8g2.setCursor(0, 60);
  u8g2.print(chartemp2);
  u8g2.sendBuffer();
}

void changeled(){
  if(ledstate==3){
    ledstate=1;
  }else{
    ledstate++;
  }
  if(ledstate==1){
    ledmode1();
  }else if(ledstate==2){
    ledmode2();
  }else if(ledstate==3){
    ledmode3();
  }
}

void ledmode1(){
  pixels.setPixelColor(0, pixels.Color(0,0,20)); // Moderately bright green color.
  for(int i=1;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(25,25,25)); // Moderately bright green color.
  }
  pixels.show();
}

void ledmode2(){
  pixels.setPixelColor(0, pixels.Color(0,0,20)); // Moderately bright green color.
  for(int i=1;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(50,50,50)); // Moderately bright green color.
  }
  pixels.show();
}

void ledmode3(){
  pixels.setPixelColor(0, pixels.Color(0,0,20)); // Moderately bright green color.
  for(int i=1;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(70,70,70)); // Moderately bright green color.
  }
  pixels.show();
}

void ledmode4(int countertemp){
  pixels.setPixelColor(0, pixels.Color(random(0,255),random(0,255),random(0,255))); 
  for(int i=1;i<NUMPIXELS;i++){
    if(countertemp%2==1){
      if(i%2==1){
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
      }else{
        pixels.setPixelColor(i, pixels.Color(0,0,0,0));
      }
    }else{
      if(i%2==0){
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
      }else{
        pixels.setPixelColor(i, pixels.Color(0,0,0,0));
      }
    }
  }
  pixels.show();
}

