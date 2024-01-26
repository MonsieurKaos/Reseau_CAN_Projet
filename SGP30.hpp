#ifndef __SGP30_H_
#define __SGP30_H_

#include "mbed.h"
#include <cstdint>
#include <iostream>
#include <string>



class SGP30 {

    private:
        char address = 0x58;
        uint32_t frequency {100000};
        uint32_t temps{500};
        PinName sda;
        PinName sdl;


    public:
        SGP30()= default;
        virtual ~SGP30()= default;

        PinName getSda() const noexcept {return this->sda;}
        void setSda(PinName sda) noexcept { this->sda = sda;}

        PinName getSdl() const noexcept {return this->sdl;}
        void setSdl(PinName sdl) noexcept { this->sdl = sdl;}

        char getAddress() const noexcept {return this->address;}

        uint32_t getFrequency() const noexcept{return this->frequency;}
        void setFrequency(uint32_t frequency) noexcept {if ((frequency>0)&&(frequency<400000))this->frequency=frequency;}

        uint32_t getTemps()const noexcept{return this->temps;}
        void setTemps(uint32_t temps)  noexcept {this->temps= temps;}

        char Crc(const uint8_t* data);

        uint8_t init_air_quality();
        uint8_t measure_air_quality( char TVOC_mesure[2], char CO2_mesure[2]);
        uint8_t test_mesure( );

        uint8_t initialiser_mesure(char TVOC_mesure[2], char CO2_mesure[2]);

};


#endif 