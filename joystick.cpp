#include "joystick.hpp"
#include <iostream>

Joystick::Joystick(PinName xPin, PinName yPin)
    : xInput(xPin), yInput(yPin), minX(0.0), maxX(1.0), minY(0.0), maxY(1.0) {}

void Joystick::calibrate() {
  std::cout << "Calibrating..." << std::endl;
  std::cout << "Don't move for default calibration" << std::endl;

  minX = 0.36;
  maxX = 1;
  minY = 0.36;
  maxY = 1;

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

  std::cout << "maxX " << maxX << std::endl;
  std::cout << "minX " << minX << std::endl;
  std::cout << "maxY " << maxY << std::endl;
  std::cout << "minY " << minY << std::endl;
  std::cout << "Calibration complete." << std::endl;
}

float Joystick::getX() {
  float xValue = xInput.read();
  return (xValue - minX) / (maxX - minX);
}

float Joystick::getY() {
  float yValue = yInput.read();
  return (yValue - minY) / (maxY - minY);
}

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
