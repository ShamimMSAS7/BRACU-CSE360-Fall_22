#include <Adafruit_Fingerprint.h>


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif




#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
Servo myservo1;
int vacantSlot = 4;
int irPin1 = 7;  // This is our input pin (IR LED1 at pin 7)
int irPin2 = 4;
int sensorInside = HIGH;  // HIGH at No Obstacle
int sensorOutside = HIGH;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {

  lcd.init();    //initialize
  lcd.backlight();  
  myservo1.attach(9);
  myservo1.write(0);
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  Serial.begin(9600);
  finger.begin(57600);

  // if (finger.verifyPassword()) {
  //   Serial.println("Found fingerprint sensor!");
  // } else {
  //   Serial.println("Did not find fingerprint sensor :(");
  //   while (1) { delay(1); }
  // }

}

void loop() {
  lcd.setCursor(0,0);         //coloum=0, row=0
  lcd.print("Number of vacant");
  lcd.setCursor(0,1);           //coloum=0, row=1
  lcd.print("slot is: ");
  lcd.print(vacantSlot);

  if (vacantSlot == 0){
    Serial.println("There is no vacant slot.");
  } 
  else {
    Serial.print("Number of vacant slot is ");
    Serial.println(vacantSlot);
  }

  sensorInside = digitalRead(irPin1);

  sensorOutside = digitalRead(irPin2);

  if (sensorInside == LOW){
    Serial.println("A car is trying to get out");
    if (matchFingerPrint()){

      Serial.println("Barrier Open");
      myservo1.write(100);
      while (1){
        sensorOutside = digitalRead(irPin2);
        if (sensorOutside == LOW){
          vacantSlot += 1;
          if (vacantSlot > 4) {
            vacantSlot = 4;
          }
          break;
        }
      }
      Serial.print("Updated number of vacant slot is ");
      Serial.println(vacantSlot);
      delay(2000);
    }
    
    
    sensorInside = 1;
    sensorOutside = 1;
  }

  if (sensorOutside == LOW){
    Serial.println("A car is trying to get in");
    if (vacantSlot>0){
      if (matchFingerPrint()){
      
        Serial.println("There are empty slots in the parking area");
        Serial.println("Barrier Open");
        myservo1.write(100);
        while (1){
          sensorInside = digitalRead(irPin1);
          if (sensorInside == LOW){
            vacantSlot -= 1;
            break;
          }
        }

        Serial.print("Updated number of vacant slot is ");
        Serial.println(vacantSlot);
        delay(2000);
      }
    
      sensorInside = 1;
      sensorOutside = 1;
    }
    else {
      Serial.println("Sorry you can't get in now.");
    }

  }
  myservo1.write(0);

  delay(4000);

}






uint8_t matchFingerPrint() {
  Serial.println("Give Fingerprint");
  uint8_t p;
  while(1){
    uint8_t p = finger.getImage();
    if (p==FINGERPRINT_OK){
      Serial.println("Image taken");
      break;
    }
  }
  
  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return false;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return false;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return false;
    default:
      Serial.println("Unknown error");
      return false;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Fingerprint Matched");
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    return true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return false;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return false;
  } else {
    Serial.println("Unknown error");
    return false;
  }

  // found a match!
  
}



