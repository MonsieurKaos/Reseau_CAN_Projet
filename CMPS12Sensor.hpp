// CMPS12Sensor.hpp

#ifndef CMPS12SENSOR_H
#define CMPS12SENSOR_H

#include "mbed.h"

class CMPS12Sensor {
public:
    CMPS12Sensor(PinName tx, PinName rx);

    int readPitch();
    int readRoll();
    int readBearing();

private:
    BufferedSerial cmps12;
    const uint8_t PITCH_COMMAND = 0x14;
    const uint8_t ROLL_COMMAND = 0x15;
    const uint8_t BEARING_COMMAND = 0x12;
};

#endif
