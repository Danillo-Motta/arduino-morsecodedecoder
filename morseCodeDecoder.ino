#include <LiquidCrystal.h>

/* TODO
After certain time add a space for a new word
A button to clear the screen

*/

//Pin Variables
const int button = 8;
const int led = 12;
const int buzzer = 13;

//Button Variables
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;

//Detect dots and dashes variables
unsigned long pressStart;
unsigned long pressEnd;
unsigned long clickDuration;
unsigned long lastButtonPress;
const unsigned long clickDurationTime = 150;

//Decode gap variables
const unsigned long gapDurationTime = 500;
bool hasInput = false;
bool decoded = false;

//Decode variables
String morseInput = "";
String decodedWord = "";
String currentWord = "";

LiquidCrystal lcd(11, 10, 5, 4, 3, 2);

String morseTable[] = {
  ".-","-...","-.-.","-..",".","..-.","--.","....",
  "..",".---","-.-",".-..","--","-.","---",".--.","--.-",
  ".-.","...","-","..-","...-",".--","-..-","-.--","--.."
};
char letterTable[] = {
  'A','B','C','D','E','F','G','H','I','J','K','L',
  'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
};

void setup()
{
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentTime = millis();
  int reading = digitalRead(button);

  lcd.setCursor(0, 1);
  lcd.print(currentWord);

  if (reading != lastButtonState) {
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        pressStart = currentTime;
        hasInput = true;
        decoded = false;

        digitalWrite(led, HIGH);
        tone(buzzer, 700);
      }
      
      if (buttonState == LOW) {
        pressEnd = currentTime;
        clickDuration = pressEnd - pressStart;
        lastButtonPress = currentTime;
        digitalWrite(led, LOW);
        noTone(buzzer);

        if (clickDuration > clickDurationTime) {
         // Serial.print("- ");
          morseInput += "-";
        } else {
          //Serial.print(". ");
          morseInput += ".";
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(morseInput);
      }
    }
  }
  lastButtonState = reading;

  if ( hasInput == true && !decoded && buttonState != HIGH && currentTime - lastButtonPress > gapDurationTime ) {
    //Serial.println(morseInput);
    int tableSize = 26;

    for (int i = 0; i < tableSize; i++) {
      if (morseTable[i] == morseInput) {
         decodedWord =  letterTable[i];
         currentWord += decodedWord;
         break;
      }
    }
    hasInput = false;
    decoded = true;
    morseInput = "";
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(currentWord);
  }
}