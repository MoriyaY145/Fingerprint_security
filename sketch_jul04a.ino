#include "TFT9341Touch.h"
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

//Servo
Servo servo; // Declare servo object globally

// TFT settings
tft9341touch LcdTouch (10, 9, 7, 2); //cs, dc ,tcs, tirq

//Serial
SoftwareSerial mySerial(3, 4);

//fingerPrint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Define my password
String password = "147";
String enteredPassword = ""; // Variable to store the entered password

// The number of the button pressed
int ButtonNum;
// Variables to pick up the place of the touch
uint16_t x,y;

// Defining functions and screens
void screenMain();
void drawCodeScreen();
void AdminOptions();
void enrollFingerprint();
int generateFingerprintID();
void deleteFingerprint();
int identifyFingerprintForDelete();
void identifyFingerprint();


void setup() {
  Serial.begin(9600);

  const int SERVO_PIN = 5; // Servo pin
  servo.attach(SERVO_PIN); // Attach servo to pin 9

  // set the data rate for the sensor serial port
  finger.begin(57600);

  // Screen settings
  LcdTouch.begin();
  LcdTouch.clearButton();
  LcdTouch.setRotation(2); //0,1,2,3
  LcdTouch.setTextSize(2);
  LcdTouch.setCursor(40, 40);
  LcdTouch.set(3780, 372, 489, 3811); //Calibration

  // Initial screen - home screen
  screenMain();
}

void loop() {

  if (LcdTouch.touched()) {
    
    LcdTouch.readTouch();
    x = LcdTouch.xTouch;
    y = LcdTouch.yTouch;

    ButtonNum = LcdTouch.ButtonTouch(x, y);
    if (ButtonNum == 18) {
        screenMain();  // Return to the home screen or to the management menu
    }
    if (ButtonNum > 0 && ButtonNum < 10) {
        enteredPassword += String(ButtonNum);
        LcdTouch.fillRect(25, 200, 150, 30, WHITE); // Adjusted y coordinate to move the password display area up
        LcdTouch.print(25, 200, enteredPassword, 2, BLACK, WHITE); // Adjusted y coordinate to move the password display area up
    }
    if (ButtonNum == 10) {
      if (enteredPassword == password) {
          LcdTouch.print(25, 180, const_cast<char*>("Password correct!"), 2, BLACK, GREEN);
          enteredPassword = "";
          AdminOptions();
       }
      else {
        LcdTouch.print(25, 180, const_cast<char*>("Incorrect password!"), 2, BLACK, RED);
        enteredPassword = ""; // Clear the entered password
        LcdTouch.fillRect(25, 200, 150, 30, WHITE); // Clear the entered password display area
       }
      LcdTouch.fillRect(25, 240, 200, 30, WHITE); // Clear the entered password display area
   }
    if (ButtonNum == 11) { // "Return" button
      if (enteredPassword.length() > 0) {
        enteredPassword.remove(enteredPassword.length() - 1);
        LcdTouch.fillRect(25, 200, 150, 30, WHITE); // Clear the current display
        LcdTouch.print(25, 200, enteredPassword, 2, BLACK, WHITE); // Display the updated password
      }
    }
    if (ButtonNum == 12)
    {
            drawCodeScreen();
    }
    if (ButtonNum == 13)
    {
           identifyFingerprint();
    }
    if (ButtonNum == 15)
    {
           enrollFingerprint();
    }
    else if (ButtonNum == 14)
    {
            deleteFingerprint();
    }
    if (ButtonNum == 16)
    {
            AdminOptions();
    }
  }
}


// screens
void screenMain() {
  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  LcdTouch.setTextSize(3);
  LcdTouch.setCursor(100, 40);
  
  LcdTouch.printheb(70, 50, const_cast<char*>("!!emocleW"), 2, WHITE);
  
  LcdTouch.drawButton(12, 10, 170, 300, 50, 20, PURPLE, WHITE, const_cast<char*>("Admin login"), 2);
  LcdTouch.drawButton(13, 10, 100, 300, 50, 20, PURPLE, WHITE, const_cast<char*>("Entrance to the library"), 2);
  while (!LcdTouch.touched());
}


void drawCodeScreen() {
  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  LcdTouch.setTextSize(3);
  LcdTouch.setCursor(100, 40);
  LcdTouch.drawButton(18, 250, 195, 60, 50, 10, BLUE, WHITE, const_cast<char*>("back"), 2);
 
  for (int i = 1; i <= 9; i++) {
    int col = (i - 1) % 3;
    int row = (i - 1) / 3;
    int x = 10 + col * 80;
    int y = 10 + row * 60; // Adjusted y coordinate to move buttons up
    char s[2];
    itoa(i, s, 10);
    LcdTouch.drawButton(i, x, y, 50, 50, 10, GREEN, WHITE, s, 2);
  }
  
  // Adjusted y coordinate for "Enter" and "Return" buttons
  LcdTouch.drawButton(10, 230, 50, 90, 50, 10, BLUE, WHITE, const_cast<char*>("Enter"), 2);
  LcdTouch.drawButton(11, 230, 110, 90, 50, 10, RED, WHITE, const_cast<char*>("Return"), 2);
  while (!LcdTouch.touched());
}


// Admin options
void AdminOptions() {
  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  LcdTouch.setTextSize(3);
  LcdTouch.setCursor(100, 40);
  
  LcdTouch.drawButton(18, 20, 20, 60, 50, 10, BLUE, WHITE, const_cast<char*>("back"), 2);
  LcdTouch.drawButton(14, 10, 170, 300, 50, 20, PURPLE, WHITE, const_cast<char*>("Deleting an existing user"), 2);
  LcdTouch.drawButton(15, 10, 100, 300, 50, 20, PURPLE, WHITE, const_cast<char*>("New user registration"), 2);
  while (!LcdTouch.touched());
}


// Fingerprint capture without ID
void enrollFingerprint() {

  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  
  LcdTouch.drawButton(16, 250, 195, 60, 50, 10, BLUE, WHITE, const_cast<char*>("back"), 2);

  LcdTouch.print(5, 10, "Ready to enroll a fingerprint!", 2, WHITE);

  int id = generateFingerprintID();  // Creating a dynamic fingerprint ID

  while (!getFingerprintEnroll(id));
}

uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  // Printing a message on a touch screen
  LcdTouch.print(5, 30, "Waiting for valid finger", 2, WHITE);

  // Looping until a valid fingerprint image is received
  while ((p != FINGERPRINT_OK)&&(!LcdTouch.touched())) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        LcdTouch.print(5, 50, "Image taken", 2, WHITE);
        break;
      case FINGERPRINT_NOFINGER:
        delay(100);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        LcdTouch.print(5, 50, "Communication error", 2, WHITE);
        break;
      case FINGERPRINT_IMAGEFAIL:
        LcdTouch.print(5, 50, "Imaging error", 2, WHITE);
        break;
      default:
        LcdTouch.print(5, 50, "Unknown error", 2, WHITE);
        break;
    }
  }
  // Convert the image to a workable structure
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      LcdTouch.print(5, 70, "Image converted", 2, WHITE);
      break;
    case FINGERPRINT_IMAGEMESS:
      LcdTouch.print(5, 70, "Image too messy", 2, WHITE);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      LcdTouch.print(5, 70, "Communication error", 2, WHITE);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      LcdTouch.print(5, 70, "Could not find fingerprint features", 2, WHITE);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      LcdTouch.print(5, 70, "Could not find fingerprint features", 2, WHITE);
      return p;
    default:
      LcdTouch.print(5, 70, "Unknown error", 2, WHITE);
      return p;
  }
  // Tell the user to remove the finger from the sensor
  LcdTouch.print(5, 90, "Remove finger", 2, WHITE);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  p = -1;
  // Tell the user to place the same finger on the sensor again
  LcdTouch.print(5, 110, "Place same finger again", 2, WHITE);
  // looping until the same fingerprint image is received again
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        LcdTouch.print(5, 130, "Image taken", 2, WHITE);
        break;
      case FINGERPRINT_NOFINGER:
        delay(100);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        LcdTouch.print(5, 130, "Communication error", 2, WHITE);
        break;
      case FINGERPRINT_IMAGEFAIL:
        LcdTouch.print(5, 130, "Imaging error", 2, WHITE);
        break;
      default:
        LcdTouch.print(5, 130, "Unknown error", 2, WHITE);
        break;
    }
  }
  // Convert the second image to a structure that can be worked with
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      LcdTouch.print(5, 150, "Image converted", 2, WHITE);
      break;
    case FINGERPRINT_IMAGEMESS:
      LcdTouch.print(5, 150, "Image too messy", 2, WHITE);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      LcdTouch.print(5, 150, "Communication error", 2, WHITE);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      LcdTouch.print(5, 150, "Could not find fingerprint features", 2, WHITE);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      LcdTouch.print(5, 150, "Could not find fingerprint features", 2, WHITE);
      return p;
    default:
      LcdTouch.print(5, 150, "Unknown error", 2, WHITE);
      return p;
  }
  
// Create a fingerprint model from the given images
LcdTouch.print(5, 170, "Creating model for you", 2, WHITE);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    LcdTouch.print(5, 190, "Prints matched!", 2, WHITE);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    LcdTouch.print(5, 190, "Communication error", 2, WHITE);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    LcdTouch.print(5, 190, "Fingerprints did not match", 2, WHITE);
    return p;
  } else {
    LcdTouch.print(5, 190, "Unknown error", 2, WHITE);
    return p;
  }

  // Saving the model in the permanent memory of the sensor
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    LcdTouch.print(5, 210, "Stored!", 2, WHITE);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    LcdTouch.print(5, 210, "Communication error", 2, WHITE);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    LcdTouch.print(5, 210, "Could not store in that location", 2, WHITE);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    LcdTouch.print(5, 210, "Error writing to flash", 2, WHITE);
    return p;
  } else {
    LcdTouch.print(5, 210, "Unknown error", 2, WHITE);
    return p;
  }
  while (!LcdTouch.touched());
}

int generateFingerprintID() {
  static int currentID = 1;  // Starts at 1 and increases each time a new fingerprint is captured
  return currentID++;
}


// delete fingerprint
void deleteFingerprint() {
  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  
  LcdTouch.drawButton(16, 250, 195, 60, 50, 10, BLUE, WHITE, const_cast<char*>("back"), 2);
  
  int id = identifyFingerprintForDelete();  // Fingerprint ID For deletion, the function returns the ID of the fingerprint

  if (id == -1) {
    LcdTouch.print(5, 190, "Fingerprint not found", 2, WHITE);
    return;
  }
  
// Deleting the fingerprint
  int p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    LcdTouch.print(5, 170, "Deleted successfully!", 2, WHITE);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    LcdTouch.print(5, 170, "Communication error", 2, WHITE);
  } else if (p == FINGERPRINT_BADLOCATION) {
    LcdTouch.print(5, 170, "Could not delete from that location", 2, WHITE);
  } else if (p == FINGERPRINT_FLASHERR) {
    LcdTouch.print(5, 170, "Error writing to flash", 2, WHITE);
  } else {
    LcdTouch.print(5, 170, "Unknown error", 2, WHITE);
  }
  while (!LcdTouch.touched());
}

//A function that performs a search and returns the ID of the found fingerprint
int identifyFingerprintForDelete() {
  int p = -1;
  int id = -1;

  // Search for the fingerprint in the database
  LcdTouch.print(5, 30, "Place finger", 2, WHITE);

  while ((p != FINGERPRINT_OK)&&(!LcdTouch.touched())) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        LcdTouch.print(5, 50, "Image taken", 2, WHITE);
        break;
      case FINGERPRINT_NOFINGER:
        delay(100);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        LcdTouch.print(5, 50, "Communication error", 2, WHITE);
        break;
      case FINGERPRINT_IMAGEFAIL:
        LcdTouch.print(5, 50, "Imaging error", 2, WHITE);
        break;
      default:
        LcdTouch.print(5, 50, "Unknown error", 2, WHITE);
        break;
    }
  }

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    LcdTouch.print(5, 70, "Error converting image", 2, WHITE);
    return -1;
  }

  LcdTouch.print(5, 70, "Remove finger", 2, WHITE);
  delay(2000);
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  LcdTouch.print(5, 90, "Place same finger again", 2, WHITE);
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        LcdTouch.print(5, 110, "Image taken", 2, WHITE);
        break;
      case FINGERPRINT_NOFINGER:
        delay(100);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        LcdTouch.print(5, 110, "Communication error", 2, WHITE);
        break;
      case FINGERPRINT_IMAGEFAIL:
        LcdTouch.print(5, 110, "Imaging error", 2, WHITE);
        break;
      default:
        LcdTouch.print(5, 110, "Unknown error", 2, WHITE);
        break;
    }
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    LcdTouch.print(5, 130, "Error converting image", 2, WHITE);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    id = finger.fingerID;
  } else {
    LcdTouch.print(5, 150, "Fingerprint not found", 2, WHITE);
  }

  return id;
}

// Fingerprint recognition and engine activation refused
void identifyFingerprint() {
  LcdTouch.clearButton();
  LcdTouch.fillScreen(BLACK);
  LcdTouch.setTextColor(WHITE, BLACK);
  
  LcdTouch.drawButton(18, 250, 195, 60, 50, 10, BLUE, WHITE, const_cast<char*>("back"), 2);
  
  LcdTouch.print(5, 10, "Place finger to identify", 2, WHITE);

  int p = -1;
 if (LcdTouch.touched()) {
  LcdTouch.readTouch();
 }
      x = LcdTouch.xTouch;
      y = LcdTouch.yTouch;
     int ButtonNum = LcdTouch.ButtonTouch(x, y);
  while ((p != FINGERPRINT_OK) && (!LcdTouch.touched()))
  {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        LcdTouch.print(5, 50, "Image taken", 2, WHITE);
        break;
      case FINGERPRINT_NOFINGER:
        delay(100);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        LcdTouch.print(5, 50, "Communication error", 2, WHITE);
        break;
      case FINGERPRINT_IMAGEFAIL:
        LcdTouch.print(5, 50, "Imaging error", 2, WHITE);
        break;
      default:
        LcdTouch.print(5, 50, "Unknown error", 2, WHITE);
        break;
    }
  }

  // Convert an image to a structure that you can work with
  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    LcdTouch.print(5, 70, "Error converting image", 2, WHITE);
    return;
  }

  // Quick search in the database
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    // Display the ID if a match is found
    int id = finger.fingerID;
    int score = finger.confidence;
    LcdTouch.print(5, 90, "Fingerprint found!", 2, WHITE);
    LcdTouch.print(5, 110, "ID: " + String(id), 2, WHITE);
    
    LcdTouch.print(5, 150, "The door opens!!", 2, WHITE);

    // Move the servo to 180 degrees, hold for 3 seconds, then move back to 0 degrees
    servo.write(90); // Move servo to 180 degrees
    delay(3000); // Hold for 3 seconds
    servo.write(0); // Move servo back to 0 degrees


  } 
  else {
    // If no match is found, display an error message
    LcdTouch.print(5, 90, "Fingerprint not found", 2, WHITE);
  }
}

