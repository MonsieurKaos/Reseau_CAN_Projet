#include "mbed.h"

enum JoystickDirection { Left, Right, Up, Down, None };

class Joystick {
public:
  Joystick(PinName xPin, PinName yPin);

  void calibrate();

  float getX();
  float getY();

  JoystickDirection Ytriggered();
  JoystickDirection Xtriggered();

private:
  AnalogIn xInput;
  AnalogIn yInput;

  float minX;
  float maxX;
  float minY;
  float maxY;

  const float yTriggLimit = 0.9;
  const float xTriggLimit = 0.9;
};
