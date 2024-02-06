#include "mbed.h"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include "SGP30.hpp"
#include "CANSmartyCoffre.hpp"


#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif


int main()
{
    PinName sda= PB_9;
    PinName sdl= PB_8;
    PinName rd= PA_11;
    PinName td= PA_12;

    std::cout<< "debut\n";

    SGP30 capteur;
    capteur.setSdl(sdl);        
    capteur.setSda(sda);        
    const char true_msg = 0xFF;
    const char false_msg = 0x00;
    CANMessage m1{0x013,&true_msg,1,CANData, CANStandard};
    CAN c1{rd,td,20000};
    uint8_t r= capteur.test_mesure();

    const uint8_t data[2]{0xBE,0xEF};

    char TVOC_mesure[2]{0,0};
    char CO2_mesure[2]{0,0};

    char TVOC_mesure_reel[2]{0,0};
    char CO2_mesure_reel[2]{0,0};
    uint8_t fct10 =capteur.initialiser_mesure(TVOC_mesure_reel, CO2_mesure_reel);
    printf(" retour de initialiser_mesure %d avec mesure TVOC [%d %d] et CO2 [%d %d] ; \n", fct10, TVOC_mesure_reel[0], TVOC_mesure_reel[1], CO2_mesure_reel[0], CO2_mesure_reel[1]);

    CANSmartyCoffre noeud(c1,1);

    noeud.StartPinging();

    while (1) {

       
        uint8_t fct2= capteur.measure_air_quality(TVOC_mesure, CO2_mesure);
       if ((TVOC_mesure[1]>160) || (TVOC_mesure[0]>1)) {
            noeud.SendDetectionSignal();
        }
        printf(" retour de measure_air_quality %d avec mesure TVOC [%d %d] et CO2 [%d %d] ; \n", fct2, TVOC_mesure[0], TVOC_mesure[1], CO2_mesure[0], CO2_mesure[1]);


        ThisThread::sleep_for(1s);

        std::cout<< "fin\n" ;

    }
    
}
