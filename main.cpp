#define COMBO_SIZE 4
#define NBR_TRY 5

#include "mbed.h"
#include <iostream>

#include "joystick.hpp"

const PinName joystickXPin = A0;
const PinName joystickYPin = A1;

int main() {
  Joystick joystick(joystickXPin, joystickYPin);
  joystick.calibrate();

  JoystickDirection combo[COMBO_SIZE] = {Up, Down, Left, Right};

  int comboStreak = 0;
  int loose = 0;

  while (true) {
    JoystickDirection X_dir = joystick.Xtriggered();
    JoystickDirection Y_dir = joystick.Ytriggered();

    if (X_dir == combo[comboStreak] || Y_dir == combo[comboStreak]) {
      std::cout << "Nice job !" << std::endl;
      comboStreak++;
    } else if (X_dir != None || Y_dir != None) {
      std::cout << "Streak killed" << std::endl;
      comboStreak = 0;
      loose++;
    }

    if (comboStreak >= COMBO_SIZE) {
      std::cout << "Combo finished" << std::endl;
      loose = 0;
      comboStreak = 0;
      // trame win
    }

    if (loose >= NBR_TRY) {
      std::cout << "You lose" << std::endl;
      loose = 0;
      comboStreak = 0;
      // trame loose
    }

    // trame keep alive

    ThisThread::sleep_for(200ms);
  }
}
