#include <WiFi.h>
#include<DHT.h>
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>                 
#include <LiquidCrystal_I2C.h>  

#define DHTPIN 21
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int buzzer = 19;

LiquidCrystal_I2C lcd(0x27, 16, 2); 
 
 
const char* ssid = "CHI LUAN IT";
const char* password = "lechiluan";
 
WiFiUDP ntpUDP;

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
 
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000);

char Time[ ] = "T:00:00:00";
char Date[ ] = "D:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
 
 
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);  
  
  Serial.begin(9600);
  dht.begin(); 
  
  lcd.begin(23, 22);                 // Initialize I2C LCD module (SDA = GPIO21, SCL = GPIO22)
  lcd.backlight();    
  lcd.createChar(1, degree);               
    
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    lcd.setCursor(0, 0);
    lcd.print("Connecting.....");
    lcd.setCursor(3, 1);
    lcd.print(ssid);
    delay(1000);
    lcd.clear();
  }  
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(3, 1);
  lcd.print(ssid);

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(buzzer, LOW);
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(buzzer, HIGH);
  delay(300); 
  lcd.clear();
  timeClient.begin();
}
 
 
void loop() {
  
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) {
 
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
 
 
 
    Time[9] = second_ % 10 + 48;
    Time[8] = second_ / 10 + 48;
    Time[6]  = minute_ % 10 + 48;
    Time[5]  = minute_ / 10 + 48;
    Time[3]  = hour_   % 10 + 48;
    Time[2]  = hour_   / 10 + 48;
 
 
 
    Date[2]  = day_   / 10 + 48;
    Date[3]  = day_   % 10 + 48;
    Date[5]  = month_  / 10 + 48;
    Date[6]  = month_  % 10 + 48;
    Date[10] = (year_   / 10) % 10 + 48;
    Date[11] = year_   % 10 % 10 + 48;
    if(Time[5] =='0' && Time[6] =='0' && Time[8] =='0' && Time[9]== '0'){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(buzzer, LOW);
      delay(300);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(buzzer, HIGH);
      delay(300); 
    }
   // Temp and Humi
    int t = dht.readTemperature();
    int h = dht.readHumidity();
    Serial.println(Time);
    Serial.println(Date);


    
    lcd.setCursor(0, 0);
    lcd.print(Time);
    lcd.setCursor(12, 0);
    lcd.print(t);
    lcd.write(1);
    lcd.print("C");
    
    lcd.setCursor(0, 1);
    lcd.print(Date);
    
    lcd.setCursor(13, 1);
    lcd.print(h);
    lcd.print("%");
    
    last_second = second_;
  }
  delay(1000);
}
