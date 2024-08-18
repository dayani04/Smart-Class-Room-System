#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define sensorPin1 11
#define sensorPin2 12

#define relayFan1 9
#define relayFan2 10
#define relayLight1 4
#define relayLight2 5
#define relayLight3 6
#define relayLight4 7

#define relayLock   8

int sensorState1 = 0;
int sensorState2 = 0;
int count = 0;

void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(relayFan1, OUTPUT);
  pinMode(relayFan2, OUTPUT);
  pinMode(relayLight1, OUTPUT);
  pinMode(relayLight2, OUTPUT);
  pinMode(relayLight3, OUTPUT);
  pinMode(relayLight4, OUTPUT);
  pinMode(relayLock, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("No Visitors     ");
  delay(200);

  finger.begin(57600);
  delay(5);
  if (!finger.verifyPassword()) {
    while (1) {
      delay(1);
    }
  }
  
  digitalWrite(relayLock, LOW); // Ensure lock is initially locked
}

void loop() {  
  if (getFingerPrint() != -1) {
    digitalWrite(relayLock, HIGH); // Unlock the door
    delay(3000);
    digitalWrite(relayLock, LOW); // Lock the door after 3 seconds  
  }  
  
  delay(50);          
  
  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);

  if (sensorState1 == LOW) {
    count++; 
    delay(500);
  }

  if (sensorState2 == LOW ) { // Ensure count doesn't go negative
    count--; 
    delay(500);
  }

  if (count == 0) {
    digitalWrite(relayFan1, LOW); 
    digitalWrite(relayFan2, LOW);
    digitalWrite(relayLight1, LOW); 
    digitalWrite(relayLight2, LOW); 
    digitalWrite(relayLight3, LOW); 
    digitalWrite(relayLight4, LOW); 

    lcd.setCursor(0, 1);
    lcd.print("No visitors    ");

  } else if (count > 0 && count <= 6) {
    digitalWrite(relayFan1, HIGH);
    digitalWrite(relayFan2, LOW);
    digitalWrite(relayLight1, HIGH); 
    digitalWrite(relayLight2, HIGH); 
    digitalWrite(relayLight3, LOW); 
    digitalWrite(relayLight4, LOW); 

    lcd.setCursor(0, 1);
    lcd.print("Visitors:   ");
    lcd.print(count);

  } else if (count > 6) {
    digitalWrite(relayFan1, HIGH);
    digitalWrite(relayFan2, HIGH);
    digitalWrite(relayLight1, HIGH); 
    digitalWrite(relayLight2, HIGH); 
    digitalWrite(relayLight3, HIGH); 
    digitalWrite(relayLight4, HIGH); 

    lcd.setCursor(0, 1);
    lcd.print("Visitors:   ");
    lcd.print(count);
  }}


