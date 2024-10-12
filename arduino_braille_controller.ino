#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define ACTIVE_ANGLE 5 // Angle for activating a Braille dot
#define INACTIVE_ANGLE 0 // Angle for deactivating a Braille dot

// Braille cell representation (Grade 2 Braille)
const byte brailleMap[256] = {
  // Letters (a-z)
  0b100000, 0b110000, 0b100100, 0b100110, 0b100010, 0b110100, 0b110110, 0b110010,
  0b010100, 0b010110, 0b101000, 0b111000, 0b101100, 0b101110, 0b101010, 0b111100,
  0b111110, 0b111010, 0b011100, 0b011110, 0b101001, 0b111001, 0b010111, 0b101101,
  0b101111, 0b101011,
  
  // Numbers (1-0)
  0b100000, 0b110000, 0b100100, 0b100110, 0b100010, 0b110100, 0b110110, 0b110010,
  0b010100, 0b010110,
  
  // Punctuation
  0b001011, // , (comma)
  0b000011, // ; (semicolon)
  0b000010, // : (colon)
  0b001101, // . (period)
  0b001100, // ! (exclamation mark)
  0b001110, // ? (question mark)
  0b000001, // ' (apostrophe)
  0b000111, // " (quotation mark)
  0b001111, // ( (left parenthesis)
  0b001111, // ) (right parenthesis)
  0b011011, // - (hyphen)
  
  // Grade 2 Contractions
  0b000100, // and
  0b111101, // for
  0b000101, // of
  0b011101, // the
  0b010010, // with
  0b110001, // ch
  0b100001, // gh
  0b110101, // sh
  0b100111, // th
  0b100110, // wh
  0b010111, // ou
  0b101111, // ow
  
  // Short-form words
  0b000011, // but
  0b001111, // can
  0b010001, // do
  0b000101, // every
  0b001110, // from
  0b011111, // go
  0b110001, // have
  0b100001, // just
  0b010101, // knowledge
  0b111111, // like
  0b110011, // more
  0b100111, // not
  0b100011, // people
  0b111011, // quite
  0b111101, // rather
  0b011011, // so
  0b011101, // that
  0b011110, // us
  0b011111, // very
  0b010010, // will
  0b100101, // it
  0b011010, // you
  0b011001, // as
  
  // Additional short-form words
  0b110111, // shall
  0b111011, // this
  0b110011, // which
  0b010110, // out
  0b111110, // still
  
  // Common letter groupings
  0b001000, // ea
  0b001100, // bb, cc, dd, ff, gg
  0b000110, // en
  0b000111, // er
  0b011000, // in
  0b001110, // ing
  0b000101, // st
  0b001111, // ar
  0b001010, // ow (as in "how")
  
  // Additional contractions
  0b001011, // ed
  0b001101, // ound
  0b001001, // ance, ence
  0b000011, // tion
  0b001100, // ness
  0b001000, // ful
  0b011100, // ity
  0b010011, // ment
  0b010101, // ation
  0b010001, // ally
  
  // More punctuation
  0b000011, // ; (semicolon)
  0b000010, // : (colon)
  0b000100, // . (decimal point)
  0b001011, // ' (apostrophe)
  0b000111, // " (quotation mark)
  0b001111, // ( (left parenthesis)
  0b001111, // ) (right parenthesis)
  0b001001, // / (slash)
  0b000001, // | (vertical line)
  0b001011, // \ (backslash)
  0b010011, // @ (at sign)
  0b000101, // # (number sign)
  0b001111, // $ (dollar sign)
  0b001101, // % (percent sign)
  0b001011, // ^ (caret)
  0b001111, // & (ampersand)
  0b001101, // * (asterisk)
  0b001011, // _ (underscore)
  0b000101, // + (plus sign)
  0b001001, // = (equals sign)
  0b001101, // ~ (tilde)
  0b000101, // < (less than)
  0b000101, // > (greater than)
  0b001101, // [ (left square bracket)
  0b001101, // ] (right square bracket)
  0b000101, // { (left curly brace)
  0b000101, // } (right curly brace)
};

void setup() {
  Serial.begin(9600);
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  delay(10);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    displayBraille(input);
  }
}

void displayBraille(String text) {
  int cellIndex = 0;
  for (int i = 0; i < text.length() && cellIndex < 6; i++) {
    char c = tolower(text.charAt(i));
    int brailleIndex = -1;
    
    if (c >= 'a' && c <= 'z') {
      brailleIndex = c - 'a';
    } else if (c >= '1' && c <= '9') {
      brailleIndex = c - '1' + 26;
    } else if (c == '0') {
      brailleIndex = 35;
    } else {
      // Handle punctuation and special characters
      switch(c) {
        case ',': brailleIndex = 36; break;
        case ';': brailleIndex = 37; break;
        case ':': brailleIndex = 38; break;
        case '.': brailleIndex = 39; break;
        case '!': brailleIndex = 40; break;
        case '?': brailleIndex = 41; break;
        case '\'': brailleIndex = 42; break;
        case '"': brailleIndex = 43; break;
        case '(': 
        case ')': brailleIndex = 44; break;
        case '-': brailleIndex = 45; break;
        // Add more cases for other punctuation as needed
      }
    }
    
    // Check for common letter groupings and contractions
    if (i < text.length() - 1) {
      String pair = text.substring(i, i+2).toLowerCase();
      if (pair == "ch") { brailleIndex = 53; i++; }
      else if (pair == "gh") { brailleIndex = 54; i++; }
      else if (pair == "sh") { brailleIndex = 55; i++; }
      else if (pair == "th") { brailleIndex = 56; i++; }
      else if (pair == "wh") { brailleIndex = 57; i++; }
      else if (pair == "ou") { brailleIndex = 58; i++; }
      else if (pair == "ow") { brailleIndex = 59; i++; }
      // Add more pairs as needed
    }
    
    if (brailleIndex != -1) {
      setBrailleChar(cellIndex, brailleIndex);
      cellIndex++;
    }
  }
  
  // Clear any remaining cells
  for (int i = cellIndex; i < 6; i++) {
    clearBrailleChar(i);
  }
}

void setBrailleChar(int charIndex, int brailleIndex) {
  byte braillePattern = brailleMap[brailleIndex];
  for (int i = 0; i < 6; i++) {
    int servoIndex = charIndex * 6 + i;
    if (bitRead(braillePattern, 5-i)) {
      setServoAngle(servoIndex, ACTIVE_ANGLE);
    } else {
      setServoAngle(servoIndex, INACTIVE_ANGLE);
    }
  }
}

void clearBrailleChar(int charIndex) {
  for (int i = 0; i < 6; i++) {
    int servoIndex = charIndex * 6 + i;
    setServoAngle(servoIndex, INACTIVE_ANGLE);
  }
}

void setServoAngle(int servoNum, int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNum, 0, pulse);
}
