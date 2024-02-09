#include "CANSmartyCoffre.hpp"

CANSmartyCoffre::CANSmartyCoffre(CAN& can1, int id_noeud) : commCan(can1) {
    this->id_noeud = id_noeud;

    // Lance le thread d'émission avec la queue d'événements
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
}

CANSmartyCoffre::~CANSmartyCoffre() {
    // Interrompt la queue d'événements, ce qui arrête aussi le thread
    queue.break_dispatch();

    thread.join();
}

bool CANSmartyCoffre::SendCanMessage(CANMessage msg){
    if (commCan.write(msg)) {
        printf("ping\n");
        return true;
    } else {
    printf("ping raté\n");
    return false;
    }
}

void CANSmartyCoffre::StartPinging() {
    // Construction du message (trame)
    CANMessage msg;
        msg.id = id_noeud;
        msg.len = 1;
        msg.data[0] = id_noeud;
        msg.format = CANStandard;
        msg.type = CANData;

    // fonction lambda  utilisée pour capturer msg et le passer à SendPing.
    // La lambda est ensuite passée à call_every comme fonction periodique.
    eventID = queue.call_every(1000, [this, msg]() { this->SendCanMessage(msg); });
}

void CANSmartyCoffre::StopPinging() {
    // Interrompt le pinging periodique
    queue.cancel(eventID);
}

void CANSmartyCoffre::SendDetectionSignal() {
    CANMessage msg;
        msg.id = id_noeud+10;
        msg.len = 1;
        msg.data[0] = 0x01;
        msg.format = CANStandard;
        msg.type = CANData;

    queue.call([this, msg]() { this->SendCanMessage(msg); });
}

void CANSmartyCoffre::SendDisarmedSignal() {
    CANMessage msg;
        msg.id = id_noeud+10;
        msg.len = 1;
        msg.data[0] = 0xFF;
        msg.format = CANStandard;
        msg.type = CANData;

    queue.call([this, msg]() { this->SendCanMessage(msg); });
}


    