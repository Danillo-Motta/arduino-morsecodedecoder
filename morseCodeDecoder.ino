#include <LiquidCrystal.h>

/* TODO
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
const unsigned long gapDurationTime = 400;
bool hasInput = false;
bool decoded = false;

//Decode variables
String morseInput = "";
String decodedWord = "";
String currentWord = "";

//Wordspace
const unsigned long spaceGapTime = 3000;
bool isTyping = false;

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
  
  lcd.setCursor(0, 0);
  lcd.print("Morse Decoder");
  lcd.setCursor(0, 1);
  lcd.print("Press to start");
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
        isTyping = true;

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
    //start a timer
    hasInput = false;
    isTyping = true;
    decoded = true;
    morseInput = "";
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(currentWord);
  }

  if ( decoded && isTyping && currentTime - lastButtonPress > spaceGapTime ) {
    isTyping = false;
    currentWord += " ";
    decoded = false;
    Serial.print("Space added");
  }
}