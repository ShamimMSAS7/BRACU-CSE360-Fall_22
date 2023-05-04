#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  lcd.init();
  lcd.backlight(); 

}

void loop() {
  lcd.setCursor(0,0);             
  lcd.print("Hello World");
  lcd.setCursor(0,1);                
  lcd.print("Welcome to CSE360"); 
  delay(5000);
  lcd.clear();

  lcd.setCursor(0,0);            
  lcd.print("it's second lab");
  lcd.setCursor(0,1);            
  lcd.print("on Saturday"); 
  delay(5000);
  lcd.clear();

}
