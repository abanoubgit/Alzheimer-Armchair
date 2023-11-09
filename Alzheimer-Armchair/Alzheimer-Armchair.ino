#include <ezButton.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Define SoftwareSerial object for DFPlayer Mini
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Constants for button and relay pins
const int BUTTON_PIN_1 = 12;
const int BUTTON_PIN_2 = 9;
const int BUTTON_PIN_3 = 8;
const int BUTTON_PIN_MAS = 7;
const int BUTTON_PIN_SPRAY = 6;
const int RELAY_PIN_MAS = 5;
const int RELAY_PIN_FANS = 4;
const int RELAY_PIN_SPRAY = 3;

// Analog sensor and LED pins
int sensorPin = A0;
int ledPin = 13;

// Variables for sensor and relay states
int sensorValue = 0;
int relayMsgState = LOW;
int relaySprayState = LOW;

// ezButton objects for each button
ezButton button1(BUTTON_PIN_1);
ezButton button2(BUTTON_PIN_2);
ezButton button3(BUTTON_PIN_3);
ezButton button4(BUTTON_PIN_MAS);
ezButton button5(BUTTON_PIN_SPRAY);

// Function prototypes
void mp3init();
void printDetail(uint8_t type, int value);

void setup() {
  // Initialize Serial and SoftwareSerial
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  // Set pin modes for LEDs and relays
  pinMode(ledPin, OUTPUT);
  pinMode(RELAY_PIN_SPRAY, OUTPUT);
  pinMode(RELAY_PIN_MAS, OUTPUT);
  pinMode(RELAY_PIN_FANS, OUTPUT);

  // Initialize DFPlayer Mini
  mp3init();

  // Set debounce time for buttons
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);
  button5.setDebounceTime(50);
}

void loop() {
  // Check if DFPlayer Mini has available data
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }

  // Read sensor value
  sensorValue = analogRead(sensorPin);

  // Control fan relay based on sensor value
  if (sensorValue > 500) {
    digitalWrite(RELAY_PIN_FANS, HIGH);
  } else {
    digitalWrite(RELAY_PIN_FANS, LOW);
  }

  // Handle button presses
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  button5.loop();

  if (button1.isPressed()) {
    Serial.println("The button1 is pressed");
    myDFPlayer.play(1);
  }

  if (button2.isPressed()) {
    Serial.println("The button2 is pressed");
    myDFPlayer.play(2);
  }

  if (button3.isPressed()) {
    Serial.println("The button3 is pressed");
    myDFPlayer.play(3);
  }

  if (button4.isPressed()) {
    Serial.println("The button4 is pressed");
    relayMsgState = !relayMsgState;
    digitalWrite(RELAY_PIN_MAS, relayMsgState);
  }

  if (button5.isPressed()) {
    Serial.println("The button5 is pressed");
    relaySprayState = !relaySprayState;
    digitalWrite(RELAY_PIN_SPRAY, relaySprayState);
  }
}

void mp3init() {
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    // Handle different DFPlayer Mini events
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
