#include "mbed.h"
#include <iostream>
#include "CANSmartyCoffre.hpp"
#include "joystick.hpp"
#include "mbed.h"

#define COMBO_SIZE 4
#define NBR_TRY 5

CAN can1 (MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);

const PinName joystickXPin = A0;
const PinName joystickYPin = A1;

int main() {

    // Constructeur prend l'objet CAN et l'identifiant du noeud (int) exemple 5 pour empreinte
    CANSmartyCoffre noeud(can1,2);

    noeud.StartPinging();
/*
    if (detection > seuil_min) {
        noeud.SendDetectionSignal();
    }

    if (deverouillage_OK){
        noeud.SendDisarmedSignal();
    }*/


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
      noeud.SendDisarmedSignal();
      printf("envoi signal Disarmed \n");

      // trame win
    }

    if (loose >= NBR_TRY) {
      std::cout << "You lose" << std::endl;
      loose = 0;
      comboStreak = 0;
      noeud.SendDetectionSignal();
      printf("envoi signal Detection \n");
    }

    // trame keep alive

    ThisThread::sleep_for(200ms);
  }
}
