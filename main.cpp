#include "mbed.h"
#include "Sharp.hpp"
#include <cstdio>
#include <cstdlib>
#include "CANSmartyCoffre.hpp"
#include "stm32l476xx.h"

CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);

InterruptIn Bouton(BUTTON1);
Timer timerAttente;
float dureeAttente;

bool CmdSuivant;

enum Etat {ATTENTE, LECTURE, ENVOI_CAN};
Etat etatActuel = ATTENTE;

void startPress() {
    CmdSuivant = true;
}

int map(float x, int in_min, int in_max, int out_min, int out_max) {
    return static_cast<int>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min); //static_cast arrondi vers 0
}

int main() {

    Sharp capteurDroit(PA_0);
    Sharp capteurGauche(PC_1);

    Bouton.fall(&startPress);

    float avgDistanceGauche;
    int xCoordToSend;
    float avgDistanceDroite;
    int yCoordToSend;

    CANSmartyCoffre noeudDistance(can1,1);

    noeudDistance.StartPinging();

    while(1){
         // Intégrer ici la routine keepAlive
        switch(etatActuel) {
        case ATTENTE:
        
            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = LECTURE;
                
            }
            break;


        case LECTURE:

            avgDistanceGauche = capteurGauche;
            printf("Distance en cm  %d\n", static_cast<int>(avgDistanceGauche*10)); // pour test doonée et calibrer distance
            
        
            if (avgDistanceGauche<15) {
                etatActuel = ENVOI_CAN;
            }
            break;

        case ENVOI_CAN:
                
                printf("Envoi d'une trame de detection CAN \n"); // envoyé ici trame de détection
                noeudDistance.SendDetectionSignal();
                timerAttente.start();
                dureeAttente = std::chrono::duration<float>(timerAttente.elapsed_time()).count();
            
                if (dureeAttente >= 1.0) {
                timerAttente.stop();
                timerAttente.reset();
                etatActuel = ATTENTE;
            }
            break;


   }
        ThisThread::sleep_for(10ms);
    
}
}