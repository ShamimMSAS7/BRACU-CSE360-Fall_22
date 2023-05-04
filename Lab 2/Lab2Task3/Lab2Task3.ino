#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.init();
  lcd.backlight(); 
  dht.begin();

}

void loop() {
  float h = dht.readHumidity();  
  //Read temperature as Celsius (the default)     
  float t = dht.readTemperature(); 
  //Read temperature as Fahrenheit (isFahrenheit = True)     
  float f =  dht.readTemperature(true); 

  //Check if any reads failed and exit early (to try again)

  if(isnan(h) || isnan(t) || isnan(f)){
    lcd.setCursor(0,0);        
    lcd.print(F("Failed"));
    return;
  }

  lcd.setCursor(0,0);      
  lcd.print("Humidity is: ");
  lcd.setCursor(0,1);         
  lcd.print(h);
  lcd.print(F("% "));
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0,0);  
  lcd.print("Temperature is: ");
  lcd.setCursor(0,1);        
  lcd.print(t);
  lcd.print(F(" deg C"));
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0,0);      
  lcd.print("Temperature is: ");
  lcd.setCursor(0,1);        
  lcd.print(f);
  lcd.print(F("F "));
  delay(5000);
  lcd.clear();

}
