#include "mbed.h"

// Définition des directions possibles du joystick
enum JoystickDirection { Left, Right, Up, Down, None };

class Joystick {
public:
  // Constructeur prenant les broches du joystick comme paramètres
  Joystick(PinName xPin, PinName yPin);

  // Calibrage du joystick
  void calibrate();

  // Obtention de la valeur de l'axe X
  float getX();

  // Obtention de la valeur de l'axe Y
  float getY();

  // Détection de la direction sur l'axe Y
  JoystickDirection Ytriggered();

  // Détection de la direction sur l'axe X
  JoystickDirection Xtriggered();

private:
  // Entrées analogiques pour les axes X et Y
  AnalogIn xInput;
  AnalogIn yInput;

  // Valeurs minimales et maximales après calibrage
  float minX;
  float maxX;
  float minY;
  float maxY;

  // Seuils de déclenchement pour les directions sur les axes X et Y
  const float yTriggLimit = 0.9;
  const float xTriggLimit = 0.9;
};
