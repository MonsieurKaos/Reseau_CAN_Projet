#include "CANSmartyCoffre.hpp"
#include "joystick.hpp"
#include "mbed.h"
#include <iostream>

#define COMBO_SIZE 4
#define NBR_TRY 5

CAN can1(PA_11, PA_12, 20000);

const PinName joystickXPin = A0;
const PinName joystickYPin = A1;

int main() {

  // Initialisation du nœud CAN pour la communication
  CANSmartyCoffre noeud(can1, 2);

  // Démarrage de la surveillance du nœud
  noeud.StartPinging();

  // Initialisation du joystick
  Joystick joystick(joystickXPin, joystickYPin);
  joystick.calibrate();

  // Définition de la séquence de combinaison
  JoystickDirection combo[COMBO_SIZE] = {Up, Down, Left, Right};

  int comboStreak = 0;
  int loose = 0;

  while (true) {
    // Lecture des directions du joystick
    JoystickDirection X_dir = joystick.Xtriggered();
    JoystickDirection Y_dir = joystick.Ytriggered();

    // Vérification de la séquence de combinaison
    if (X_dir == combo[comboStreak] || Y_dir == combo[comboStreak]) {
      std::cout << "Bien joué !" << std::endl;
      comboStreak++;
    } else if (X_dir != None || Y_dir != None) {
      std::cout << "Séquence interrompue" << std::endl;
      comboStreak = 0;
      loose++;
    }

    // Vérification de la réussite de la combinaison
    if (comboStreak >= COMBO_SIZE) {
      std::cout << "Combinaison réussie" << std::endl;
      loose = 0;
      comboStreak = 0;

      // Envoi du signal de victoire
      noeud.SendDisarmedSignal();
    }

    // Vérification du nombre d'échecs
    if (loose >= NBR_TRY) {
      std::cout << "Vous avez perdu" << std::endl;
      loose = 0;
      comboStreak = 0;

      // Envoi du signal de détection d'échec
      noeud.SendDetectionSignal();
    }

    // Envoi du signal de maintien de connexion
    ThisThread::sleep_for(200ms);
  }
}