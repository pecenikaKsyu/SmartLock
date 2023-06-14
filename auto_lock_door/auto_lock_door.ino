#include <Keypad.h>             
#include <Wire.h>               
#include <LiquidCrystal_I2C.h>  

LiquidCrystal_I2C lcd(0x3F, 20, 4);

char keys[4][3] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPin[4] = { 6, 7, 8, 9 };
byte colPin[3] = { 3, 4, 5 };

String password = "1234";  
int position = 0;          // Character's password positiion
int wrong = 0;             // Failed attempt counter
int redPin = 10;           // Locked door output
int greenPin = 11;         // Opened door output
int buzzer = 12;           // Alarm output
int total = 0;             // Failed attempt overall

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 3);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.init();       
  lcd.backlight();
  lcd.clear();      
  setLocked(true);  
  delay(20);
}

void loop() {
  lcd.clear();  
  lcd.print(" Enter the code : ");
  delay(100);
  char pressed = keypad.getKey();  
  String key[3];

  if (pressed) {
    if (pressed == '#') {  
      position = 0;        
      setLocked(true);     
      lcd.clear();         
    }

    else if (pressed == password[position]) {  
      key[position] = pressed;
      position++;  
    }

    else if (pressed != password[position]) { 
      wrong++;     
      position++;  
    }

    if (position == 4) {
      if (wrong > 0) { 
        total++;       
        wrong = 0;     
        position = 0;  
        lcd.clear();   
        lcd.setCursor(0, 1);
        lcd.print("  WRONG CODE!");
        lcd.setCursor(0, 2);
        lcd.print("  LEFT ATTEMPTS: ");
        lcd.setCursor(19, 2);
        lcd.print(3 - total);
        setLocked(true);  
      }

      else if (position == 4 && wrong == 0) { 
        buzzer_beep_unlock();  
      }

      if (total == 3) {      // 3 failed attempt in a row
        total = 0;           
        buzzer_beep_lock();  
      }
    }
  }
}

void setLocked(int locked) {      
  if (locked) {                   
    digitalWrite(redPin, HIGH);   
    digitalWrite(greenPin, LOW);  
    delay(2000);
  } else {                         
    digitalWrite(redPin, LOW);     
    digitalWrite(greenPin, HIGH);  
    delay(5000);
    digitalWrite(redPin, HIGH);   
    digitalWrite(greenPin, LOW);  
  }
}

void buzzer_beep_lock() { 
  lcd.clear();             
  lcd.setCursor(0, 1);
  lcd.print("   ACCESS DENIED ! ");
  digitalWrite(buzzer, HIGH);
  delay(10000);
  digitalWrite(buzzer, LOW);
}

void buzzer_beep_unlock() {  
  position = 0;              
  wrong = 0;                 
  lcd.clear();               
  lcd.setCursor(0, 1);
  lcd.print("      WELCOME!     ");
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  setLocked(false);  
}
