#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ir1 11
#define ir2 12

int count = 0;

void setup() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("No Visitors     ");
  delay(2000);  // Increased delay to better see the initial message
  lcd.clear();
}

void loop() {
  // Detect a person entering
  if (digitalRead(ir1) == HIGH) {
    // Wait for the person to reach the second sensor
    while (digitalRead(ir1) == HIGH) {
      // do nothing, just wait
    }
    if (digitalRead(ir2) == HIGH) {
      // Wait for the person to completely pass the second sensor
      while (digitalRead(ir2) == HIGH) {
        // do nothing, just wait
      }
      count++;
      updateDisplay();
    }
  }

  // Detect a person leaving
  if (digitalRead(ir2) == HIGH) {
    // Wait for the person to reach the first sensor
    while (digitalRead(ir2) == HIGH) {
      // do nothing, just wait
    }
    if (digitalRead(ir1) == HIGH) {
      // Wait for the person to completely pass the first sensor
      while (digitalRead(ir1) == HIGH) {
        // do nothing, just wait
      }
      count--;
      updateDisplay();
    }
  }
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("Count: ");
  lcd.print(count);
}
