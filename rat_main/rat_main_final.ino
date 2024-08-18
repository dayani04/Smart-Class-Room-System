#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Pin definitions
#define ir1 9
#define ir2 8
#define relayFan1 11
#define relayFan2 10
#define relayLight1 4
#define relayLight2 5
#define relayLight3 6
#define relayLight4 7
#define relayLock 12 

int sensorState1 = 0;
int sensorState2 = 0;
int count = 0;
bool authorized = false;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define ACCESS_DELAY 2000 

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(relayFan1, OUTPUT);
  pinMode(relayFan2, OUTPUT);
  pinMode(relayLight1, OUTPUT);
  pinMode(relayLight2, OUTPUT);
  pinMode(relayLight3, OUTPUT);
  pinMode(relayLight4, OUTPUT);
  pinMode(relayLock, OUTPUT);  

  digitalWrite(relayLock, LOW); 

  pinMode(ir1, INPUT_PULLUP);
  pinMode(ir2, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("No Student     ");
  delay(200);

  // Initialize fingerprint sensor
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    lcd.setCursor(0, 0);
    lcd.print("Fingerprint OK");
    delay(2000);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Fingerprint err");
    while (1) { delay(1); }
  }
}

void loop() {
  if (!authorized) {
    lcd.setCursor(0, 0);
    lcd.print("Scan fingerprint");

    if (getFingerPrint() != -1) {
      authorized = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Granted");
      digitalWrite(relayLock, HIGH);
      delay(ACCESS_DELAY);
      digitalWrite(relayLock, LOW);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("COUNTER");
      lcd.setCursor(0, 1);
      lcd.print("No Student     ");
    }
    delay(1000);
  } else {
    sensorState1 = digitalRead(ir1);
    sensorState2 = digitalRead(ir2);

    if (sensorState1 == LOW) {
      count++;
      delay(500);
    }

    if (sensorState2 == LOW) {
      count--;
      delay(500);
    }

    if (count <= 0) {
      lcd.setCursor(0, 1);
      lcd.print("No students    ");
    } else if (count > 0 && count < 10) {
      lcd.setCursor(0, 1);
      lcd.print("students:   ");
      lcd.setCursor(12, 1);
      lcd.print(count);
      lcd.setCursor(13, 1);
      lcd.print("  ");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("students:   ");
      lcd.setCursor(12, 1);
      lcd.print(count);
    }

    if (count == 0) {
      digitalWrite(relayFan1, LOW);
      digitalWrite(relayFan2, LOW);
      digitalWrite(relayLight1, LOW);
      digitalWrite(relayLight2, LOW);
      digitalWrite(relayLight3, LOW);
      digitalWrite(relayLight4, LOW);
    } else if (count > 0 && count <= 6) {
      digitalWrite(relayFan1, HIGH);
      digitalWrite(relayFan2, LOW);
      digitalWrite(relayLight1, HIGH);
      digitalWrite(relayLight2, HIGH);
      digitalWrite(relayLight3, LOW);
      digitalWrite(relayLight4, LOW);
    } else if (count > 6) {
      digitalWrite(relayFan1, HIGH);
      digitalWrite(relayFan2, HIGH);
      digitalWrite(relayLight1, HIGH);
      digitalWrite(relayLight2, HIGH);
      digitalWrite(relayLight3, HIGH);
      digitalWrite(relayLight4, HIGH);
    }

    delay(1000); 
  }
}

// Returns -1 if failed, otherwise returns ID #
int getFingerPrint() {
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // Found a match!
  return finger.fingerID;
}
