#include "joystick.hpp"
#include <iostream>

// Implémentation des méthodes de la classe Joystick

// Constructeur initialisant les broches du joystick
Joystick::Joystick(PinName xPin, PinName yPin)
    : xInput(xPin), yInput(yPin), minX(0.0), maxX(1.0), minY(0.0), maxY(1.0) {}

// Calibration du joystick pour définir les valeurs min et max
void Joystick::calibrate() {
  std::cout << "Calibrage en cours..." << std::endl;
  std::cout << "Ne pas bouger pour la calibration par défaut" << std::endl;

  // Valeurs par défaut
  minX = 0.36;
  maxX = 1;
  minY = 0.36;
  maxY = 1;

  // Calibration de l'axe X
  for (int i = 0; i < 500; ++i) {
    float xValue = xInput.read();
    std::cout << xValue << std::endl;

    if (xValue < minX) {
      minX = xValue;
    } else if (xValue > maxX) {
      maxX = xValue;
    }
    ThisThread::sleep_for(10ms);
  }

  // Calibration de l'axe Y
  for (int i = 0; i < 500; ++i) {
    float yValue = yInput.read();
    std::cout << yValue << std::endl;
    if (yValue < minY) {
      minY = yValue;
    } else if (yValue > maxY) {
      maxY = yValue;
    }
    ThisThread::sleep_for(10ms);
  }

  // Affichage des valeurs calibrées
  std::cout << "maxX " << maxX << std::endl;
  std::cout << "minX " << minX << std::endl;
  std::cout << "maxY " << maxY << std::endl;
  std::cout << "minY " << minY << std::endl;
  std::cout << "Calibration terminée." << std::endl;
}

// Obtention de la valeur normalisée de l'axe X
float Joystick::getX() {
  float xValue = xInput.read();
  return (xValue - minX) / (maxX - minX);
}

// Obtention de la valeur normalisée de l'axe Y
float Joystick::getY() {
  float yValue = yInput.read();
  return (yValue - minY) / (maxY - minY);
}

// Détection de la direction sur l'axe Y
JoystickDirection Joystick::Ytriggered() {
  float value = this->getY();
  if (value >= yTriggLimit) {
    return Up;
  } else if (value <= 1 - yTriggLimit) {
    return Down;
  } else {
    return None;
  }
}

// Détection de la direction sur l'axe X
JoystickDirection Joystick::Xtriggered() {
  float value = this->getX();
  if (value >= xTriggLimit) {
    return Left;
  } else if (value <= 1 - xTriggLimit) {
    return Right;
  } else {
    return None;
  }
}
