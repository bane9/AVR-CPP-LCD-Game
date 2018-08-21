# AVR-CPP-LCD-Game
C++ port of of my chrome's dinosaur game.

Initial project here: https://www.hackster.io/brzi/google-chrome-dinosaur-game-on-16x2-lcd-shield-b52de2

This version is re-written in C++ and adds several things:
  
  1. C++ library for driving the LCD. This library was made specifically for Arduino Uno board with DFRobot LCD Keypad shield, though it can be easily ported to what ever it will be running on.
  
  2. Cleaner code and easy to use functions
  
  3. Scores don't print zeroes before the actual score prints
  
  4. Ghosting isn't as visible
