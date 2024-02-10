#include "mbed.h"
#include "CMPS12Sensor.hpp"
#include "CANSmartyCoffre.hpp" 

// Initialisation des objets nécessaires
CMPS12Sensor cmps12(PC_1, PC_0); // Capteur CMPS12
CAN canBus(PA_11, PA_12); // Bus CAN
BufferedSerial pc(USBTX, USBRX, 9600); // Communication série

// Bouton pour définir un nouveau point de référence
InterruptIn button(BUTTON1);

// Mutex pour protéger l'accès aux variables partagées
Mutex mutex;

// Variables pour les valeurs de référence et actuelles
int8_t refPitch = 0, refRoll = 0, refBearing = 0;
int8_t bearing = 0, pitch = 0, roll = 0;
volatile bool updateReference = false;

// Création de l'objet CANSmartyCoffre avec le bus CAN et un ID de nœud
CANSmartyCoffre smartCoffre(canBus, 0x14);

void setNewReferenceFlag() {
    updateReference = true;
}

void checkForReferenceUpdate() {
    if (updateReference) {
        mutex.lock();
        refPitch = pitch;
        refRoll = roll;
        refBearing = bearing;
        updateReference = false;
        mutex.unlock();
    }
}

void sensorReadings() {
    while (true) {
        mutex.lock();
        pitch = cmps12.readPitch();
        roll = cmps12.readRoll();
        bearing = cmps12.readBearing();
        mutex.unlock();

        char buffer[64];
        int len = snprintf(buffer, sizeof(buffer), "Pitch: %d, Roll: %d, Bearing: %d\r\n", pitch, roll, bearing);
        pc.write(buffer, len);

        ThisThread::sleep_for(100ms);
    }
}

void canComms() {
    smartCoffre.StartPinging(); // Démarre le pinging avec CANSmartyCoffre

    while (true) {
        mutex.lock();
        int deltaPitch = pitch - refPitch;
        int deltaRoll = roll - refRoll;
        int deltaBearing = bearing - refBearing;
        mutex.unlock();

        char buffer[64];
        int len = snprintf(buffer, sizeof(buffer), "deltaPitch: %d, deltaRoll: %d, deltaBearing: %d\r\n", deltaPitch, deltaRoll, deltaBearing);
        pc.write(buffer, len);

        if (deltaPitch < -5 || deltaPitch > 5 || deltaRoll < -5 || deltaRoll > 5 || deltaBearing < -5 || deltaBearing > 5) {
            smartCoffre.SendDetectionSignal(); // Utilise CANSmartyCoffre pour envoyer un signal de détection
        }

        ThisThread::sleep_for(1000ms);
    }
}

int main() {
    button.fall(&setNewReferenceFlag);

    Thread threadSensor;
    Thread threadCan;

    threadSensor.start(sensorReadings);
    threadCan.start(canComms);

    while (true) {
        checkForReferenceUpdate();
        ThisThread::sleep_for(chrono::milliseconds(osWaitForever));
    }
}
