#ifndef CANSMARTYCOFFRE_HPP
#define CANSMARTYCOFFRE_HPP

#include "mbed.h"
#include <cstdio>
#include "rtos.h"

class CANSmartyCoffre {

public:
    CANSmartyCoffre(CAN&, int);
    ~CANSmartyCoffre();

    void StartPinging();
    
    void StopPinging();

    void SendDetectionSignal();

    void SendDisarmedSignal();

    // Complique de recuperer le retour d'un thread en utilisant la queue, à voir + tard

private:
    CAN& commCan;

    Thread thread;
    EventQueue queue;
    int eventID;

    bool SendCanMessage(CANMessage);

    int id_noeud;
};

#endif //CANSMARTYCOFFRE_HPP