// CMPS12Sensor.cpp

#include "CMPS12Sensor.hpp"

CMPS12Sensor::CMPS12Sensor(PinName tx, PinName rx) : cmps12(tx, rx, 9600) {}

int CMPS12Sensor::readPitch() {
    cmps12.write(&PITCH_COMMAND, 1);
    ThisThread::sleep_for(10ms); // Donne le temps au capteur de répondre
    int8_t pitch = 0;
    if (cmps12.readable()) {
        cmps12.read(&pitch, 1);
    }
    return pitch;
}

int CMPS12Sensor::readRoll() {
    cmps12.write(&ROLL_COMMAND, 1);
    ThisThread::sleep_for(10ms);
    int8_t roll = 0;
    if (cmps12.readable()) {
        cmps12.read(&roll, 1);
    }
    return roll;
}

int CMPS12Sensor::readBearing() {
    cmps12.write(&BEARING_COMMAND, 1);
    ThisThread::sleep_for(10ms);
    int8_t bearing = 0;
    if (cmps12.readable()) {
        cmps12.read(&bearing, 1);
    }
    return bearing;
}
