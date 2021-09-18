/* TeensyMM_Input_Display_Board demo
 * Copyright (c) 2021 LAtimes2
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// This program demonstrates all the features of the SparkFun MicroMod Input and Display Carrier Board (DEV-16985).
// It is written for the MicroMod Teensy Processor (DEV-16402).

// This board has the following capabilities. Each of them is demonstrated with this demo:
//     2.4" TFT ILI9341 display
//     various buttons/joystick
//     microSD slot
//     6 APA102 LEDs
//     Buzzer

// Libraries are installed using Tools -> Manage Libraries...

//
// Common : used by all items except buzzer
//
#include <Wire.h>

//
// Display
//
#include <ILI9341_t3.h>

#define TFT_DC 5
#define TFT_CS 4
#define TFT_BACKLIGHT 3

int backlightValue = 0;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

//
// Buttons
//
#include <SparkFun_MicroMod_Button.h>

#define Button_A      0x01
#define Button_B      0x02
#define Button_Up     0x04
#define Button_Down   0x08
#define Button_Left   0x10
#define Button_Right  0x20
#define Button_Center 0x40

MicroModButton button;

//
// LEDs
//
#include <Adafruit_DotStar.h>

#define NUMPIXELS 6 // Number of LEDs in strip
#define DATAPIN    41
#define CLOCKPIN   40

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

//
// Buzzer
//
#define BUZZERPIN 2

//
// microSD card
//
#include <SD.h>

#define MICROSD_CS 10

Sd2Card card;

void setup() {

  //
  // Common : setup
  //
  Wire.begin(); //Join I2C bus

  //
  // Display : setup
  //
  tft.begin();

  tft.invertDisplay (true);

  // rotate 180 degrees
  tft.setRotation(2);

  //
  // Buttons : setup
  //
  button.begin();

  //
  // LEDs : setup
  //
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  //
  // microSD : setup
  //
  if (!card.init(SPI_HALF_SPEED, MICROSD_CS)) {
    // card did not initialize
  }

  //
  // general
  //
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.setCursor (0, 0);
  tft.println("     TeensyMM");
  tft.println(" Input and Display");
  tft.println("   Board Demo");
  tft.println("");
  tft.setTextColor(ILI9341_LIGHTGREY);
  tft.print  ("Press up/down button");
  tft.println ("   to choose item");
  tft.println("");
  tft.println("Press center button");
  tft.println ("     to select");
  tft.println("");
  tft.println("Press left button");
  tft.println ("  to exit menu");
  tft.println("");
  tft.setTextColor(ILI9341_YELLOW);
  tft.println("  Press any button");
  tft.println ("    to continue");

  // wait for any button to be clicked
  while (!button.getClicked());
}

void loop() {

  int selectedIndex = 0;

  String menu[] = { "Display", "Buttons", "LEDs", "microSD", "Buzzer", "" };

  while (true) {
    selectedIndex = doMenu (menu, selectedIndex);

    switch (selectedIndex) {
      case 0: demoDisplay(); break;
      case 1: demoButtons(); break;
      case 2: demoLEDs(); break;
      case 3: demoMicroSD(); break;
      case 4: demoBuzzer(); break;
      default: selectedIndex = 0; break;
    }
  }
}

//
// Display : demo
//
void demoDisplay() {
  int selectedIndex = 0;

  String menu[] = { "Backlight", "Sleep(3s)", "" };

  while (selectedIndex >= 0) {
    selectedIndex = doMenu (menu, selectedIndex);

    switch (selectedIndex) {
      case 0: demoDisplayBacklight(); break;
      case 1: demoDisplaySleep(); break;
    }
  }
}

void demoDisplayBacklight() {
  // Button Up will make it brighter, Button Down dimmer.
  // Range is 0 (brightest) to 255 (dimmest)

  bool done = false;
  int previousValue = -1;

  while (!done) {
    if (backlightValue != previousValue) {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
      tft.setTextSize(2);
      tft.setCursor (0, 50);
      tft.println(" Press Up : brighter");
      tft.print(" Press Down : dimmer");

      tft.setTextSize(3);
      tft.setCursor (40, 150);
      tft.print("Backlight");
      tft.setCursor (40, 190);
      tft.print("Value:");

      tft.setCursor (150, 190);

      tft.print(backlightValue);
      tft.print("   ");

      previousValue = backlightValue;
    }

    if (button.getPressed() & Button_Up) {
      if (backlightValue > 4) {
        backlightValue -= 5;
      }
      analogWrite(TFT_BACKLIGHT, backlightValue);
    }

    if (button.getPressed() & Button_Down) {
      if (backlightValue < 251) {
        backlightValue += 5;
      }
      analogWrite(TFT_BACKLIGHT, backlightValue);
    }

    if (button.getClicked() & Button_Left) {
      done = true;
    }

    delay(100);
  }
}

void demoDisplaySleep() {
  // Turns off the display for 3 seconds

  tft.sleep(true);
  delay(3000);
  tft.sleep(false);
}

//
// Buttons : demo
//
void demoButtons() {
  int selectedIndex = 0;

  String menu[] = { "Press", "Click", "" };
  //// TODO: add items: String menu[] = { "Press", "Click", "Interrupt", "Debounce", "" };

  while (selectedIndex >= 0) {
    selectedIndex = doMenu (menu, selectedIndex);

    switch (selectedIndex) {
      case 0: demoButtonsPress(); break;
      case 1: demoButtonsClick(); break;
    }
  }
}

void demoButtonsPress() {
  bool done = false;
  int pressed = 0;
  int previousValue = -1;

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor (0, 50);
  tft.println(" Press and release");
  tft.print(" Left to exit");
  tft.setTextSize(3);

  while (!done) {

    // set background color to red for each button when it is pressed
    tft.setCursor (45, 150);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_Up ? ILI9341_RED : ILI9341_BLACK);
    tft.print("Up");

    tft.setCursor (45, 230);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_Down ? ILI9341_RED : ILI9341_BLACK);
    tft.print("Dn");

    tft.setCursor (10, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_Left ? ILI9341_RED : ILI9341_BLACK);
    tft.print("L");

    tft.setCursor (55, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_Center ? ILI9341_RED : ILI9341_BLACK);
    tft.print("C");

    tft.setCursor (100, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_Right ? ILI9341_RED : ILI9341_BLACK);
    tft.print("R");

    tft.setCursor (160, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_A ? ILI9341_RED : ILI9341_BLACK);
    tft.print("A");

    tft.setCursor (200, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, pressed & Button_B ? ILI9341_RED : ILI9341_BLACK);
    tft.print("B");

    while (pressed == previousValue) {
      // read the buttons
      pressed = button.getPressed();

      delay(100);
    }

    previousValue = pressed;

    if (button.getClicked() & Button_Left) {
      done = true;
    }
  }
}

void demoButtonsClick() {
  bool done = false;
  int clicked = 0;
  int previousValue = -1;
  int leftPressedTime = 0;

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor (0, 50);
  tft.println(" Press and release");
  tft.print(" Left to exit");
  tft.setTextSize(3);

  while (!done) {

    // set background color to red for each button when it is clicked (released)
    tft.setCursor (45, 150);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Up ? ILI9341_RED : ILI9341_BLACK);
    tft.print("Up");

    tft.setCursor (45, 230);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Down ? ILI9341_RED : ILI9341_BLACK);
    tft.print("Dn");

    tft.setCursor (10, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Left ? ILI9341_RED : ILI9341_BLACK);
    tft.print("L");

    tft.setCursor (55, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Center ? ILI9341_RED : ILI9341_BLACK);
    tft.print("C");

    tft.setCursor (100, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Right ? ILI9341_RED : ILI9341_BLACK);
    tft.print("R");

    tft.setCursor (160, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_A ? ILI9341_RED : ILI9341_BLACK);
    tft.print("A");

    tft.setCursor (200, 190);
    tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_B ? ILI9341_RED : ILI9341_BLACK);
    tft.print("B");

    delay(500);
    clicked = 0;

    while (clicked == previousValue) {
      // read the buttons
      clicked = button.getClicked();

      delay(100);
    }

    previousValue = clicked;

    if (clicked & Button_Left) {
      tft.setCursor (10, 190);
      tft.setTextColor(ILI9341_LIGHTGREY, clicked & Button_Left ? ILI9341_RED : ILI9341_BLACK);
      tft.print("L");

      delay(500);

      done = true;
    }
  }
}

//
// LEDs : demo
//
void demoLEDs() {
  // turns each LED on when selected

  const int MEDIUM_RED   = 0x800000;
  const int MEDIUM_GREEN = 0x008000;
  const int MEDIUM_BLUE  = 0x000080;
  const int OFF          = 0x000000;
  
  int selectedIndex = 0;
  int previousIndex = 0;

  String menu[] = { "0", "1", "2", "3", "4", "5", "" };

  while (selectedIndex >= 0) {
    previousIndex = selectedIndex;

    selectedIndex = doMenu(menu, selectedIndex);

    strip.setPixelColor(previousIndex, OFF);

    switch (selectedIndex) {
      case 0: strip.setPixelColor(0, MEDIUM_RED); break;
      case 1: strip.setPixelColor(1, MEDIUM_GREEN); break;
      case 2: strip.setPixelColor(2, MEDIUM_BLUE); break;
      case 3: strip.setPixelColor(3, MEDIUM_RED); break;
      case 4: strip.setPixelColor(4, MEDIUM_GREEN); break;
      case 5: strip.setPixelColor(5, MEDIUM_BLUE); break;
    }

    strip.show();                     // Refresh strip
  }
}

//
// microSD : demo
//
void demoMicroSD() {
  // Indicates if a card is detected

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor (0, 50);
  
  if (!card.init(SPI_HALF_SPEED, MICROSD_CS)) {
    // card did not initialize
    tft.println(" Card not found.");
    tft.println(" Insert card and");
    tft.println(" try again");
  } else {
    tft.println(" Yeah!");
    tft.println(" Card found");
  }

  while (!(button.getClicked() & Button_Left));
}

//
// Buzzer : demo
//
void demoBuzzer() {
  // Play a tone for 1 second

  int frequency = 400;
  int lengthMsec = 1000;

  tone(BUZZERPIN, frequency, lengthMsec);
}

int doMenu (String menu[], int selectedIndex) {
  // returns the selected menu item, or -1 to exit the menu

  bool done = false;
  int index = 0;

  while (!done) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setTextSize(3);
    tft.setCursor (0, 0);

    index = 0;
    while (menu[index] != "") {
      tft.println("");
      if (index == selectedIndex) {
        tft.setTextColor(ILI9341_YELLOW);
        tft.print("-> ");
      } else {
        tft.print("   ");
      }
      tft.setTextColor(ILI9341_LIGHTGREY);
      tft.println(menu[index]);
      ++index;
    }
  
    // wait for a button to be clicked
    uint8_t clicked = 0;
    while (!clicked) {
      clicked = button.getClicked();
    }
  
    if (clicked & Button_Up) {
      if (selectedIndex > 0) {
        --selectedIndex;
      }
    }
  
    if (clicked & Button_Down) {
      if (menu[selectedIndex+1] != "") {
        ++selectedIndex;
      }
    }
  
    if (clicked & Button_Center) {
      done = true;
    }
  
    if (clicked & Button_Left) {
      // exit menu
      selectedIndex = -1;
      done = true;
    }
  }

  return selectedIndex;
}
