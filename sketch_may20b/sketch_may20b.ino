#include <Keypad.h>
#include <ESP32Servo.h>

// Keypad Vars
// Define Keypad Symbols
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect Pins
byte rowPins[4] = {14, 27, 26, 25};
byte colPins[4] = {13, 21, 22, 23};

// Keypad Var
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);
// Server Var
Servo myservo; // create servo object to control a servor

// Pin Vars
int servoPin = 19;
int buzzerPin =18;

// Save the correct password
char passWord[] = {"1234"};

//Setup Code, to run once:
void setup() {
  digitalWrite(buzzerPin, LOW);
  myservo.setPeriodHertz(50); // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500); // attaches the servo on servoPin to the servo object
  myservo.write(0);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("ESP32 is ready!");
  digitalWrite(buzzerPin, LOW);
}

// Main Code, to run repeatedly:
void loop() {
  digitalWrite(buzzerPin, LOW);
  // Vars
  static char keyIn[4]; // Save the input character
  static byte keyInNum = 0; // Save the number of intput characters
  char keyPressed = myKeypad.getKey();
  
  if (keyPressed) {
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    // Save the input characters
    keyIn[keyInNum++] = keyPressed;
    // Judge the correctness after input
    if (keyInNum == 4) {
      bool isRight = true;
      for (int i = 0; i < 4; i++) {
        if (keyIn[i] != passWord[i])
          isRight = false;
      }
      if (isRight) {
        Serial.println("Password right!");
        myservo.write(90); // open switch
        delay(2000);
        myservo.write(0); // close switch
        delay(100);
        digitalWrite(buzzerPin, LOW);
      } else {
        digitalWrite(buzzerPin, HIGH); // Make a wrong password prompt tone
        delay(100);
        digitalWrite(buzzerPin, LOW);
        Serial.println("Password error!");
      }
      digitalWrite(buzzerPin, LOW);
      keyInNum = 0; // Reset the number of the input characters to 0
    }
  }
}
