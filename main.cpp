#include "mbed.h"
#include "Sharp.hpp"
#include <cstdio>
#include <cstdlib>
#include "CANSmartyCoffre.hpp"
#include "stm32l476xx.h"

// Configuration du contrôleur CAN avec les broches PA_11 et PA_12, vitesse de 20000 bits/s
CAN can1(PA_11, PA_12, 20000);

// Déclaration de l'interruption liée au bouton poussoir
InterruptIn Bouton(BUTTON1);

// Timer pour mesurer la durée envoi trame détection CAN
Timer timerAttente;

float dureeAttente;

bool CmdSuivant;

// Enumération des différents états du programme
enum Etat {ATTENTE, LECTURE, ENVOI_CAN};
Etat etatActuel = ATTENTE;

// Fonction appelée lors de l'appui sur le bouton poussoir
void startPress() {
    CmdSuivant = true;
}


int main() {

    // Initialisation du capteurs de distance 

    Sharp capteurDist(PC_1);

    // Association de la fonction startPress() à l'interruption de chute du bouton
    Bouton.fall(&startPress);

    // Variables pour stocker distance capteur
    float avgDistance;

    float DistMinDetect = 15.0;


    // Initialisation du nœud CAN pour la communication
    CANSmartyCoffre noeudDistance(can1, 1);
    noeudDistance.StartPinging(); // Démarrage de l'envoi de signaux CAN périodiques

    while(1) {

        switch(etatActuel) {
            case ATTENTE:
                if (CmdSuivant) {
                    CmdSuivant = false;
                    etatActuel = LECTURE;
                }
                break;

            case LECTURE:
                // Lecture de la distance du capteur 
                avgDistance = capteurDist;
                printf("Distance en cm : %d\n", static_cast<int>(avgDistance * 10)); // Affichage de la distance pour le test lecture
                
                // Si la distance est inférieure à 15 cm, passer à l'état d'envoi CAN
                if (avgDistance < DistMinDetect) {
                    etatActuel = ENVOI_CAN;
                }
                break;

            case ENVOI_CAN:
                printf("Envoi d'une trame de détection CAN\n"); // Envoi d'une trame de détection
                noeudDistance.SendDetectionSignal(); // Envoi d'un signal de détection via CAN
                timerAttente.start();
                // Mesure de la durée d'attente
                dureeAttente = std::chrono::duration<float>(timerAttente.elapsed_time()).count();

                // Si la durée d'attente dépasse 1.0 seconde, retour à l'état d'attente
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
