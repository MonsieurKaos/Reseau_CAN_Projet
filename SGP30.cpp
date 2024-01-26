#include "SGP30.hpp"
#include <MbedCRC.h>
#include <cstdint>


char SGP30::Crc(const uint8_t* data){        
    uint32_t result2;
    MbedCRC<0x31,8> crc(0xFF,0x00,false,false);
    crc.compute((void *)data, 2, &result2);         
    char result_char{static_cast<char>((result2<<16)>>16)};
    return result_char;
}

uint8_t SGP30::test_mesure(){

    char address_read= address << 1;
    char cmd[2];
    cmd[0]=0x20;
    cmd[1]=0x32;
    char data[3];

    I2C i2c1(sda,sdl);
    i2c1.frequency(frequency);    
    i2c1.start();
    
    uint8_t retour1 = i2c1.write(address_read,cmd,2,false);
    if (retour1 != 0) return 1;   

    ThisThread::sleep_for(temps);
    
    uint8_t retour2 = i2c1.read(address_read,data,3,false);
    if(retour2 != 0) return 2;     
    i2c1.stop();
    if ( (static_cast<uint8_t>(data[0])== 0xD4)&& (static_cast<uint8_t>(data[1])==0x00)){
        return 0;      
    }
    return 3;       

        
}

uint8_t SGP30::init_air_quality(){
    char address_read= address << 1;
    char cmd[2];
    cmd[0]=0x20;
    cmd[1]=0x03;

    I2C i2c1(sda,sdl);
    i2c1.frequency(frequency);   
    i2c1.start();
    uint8_t retour1 = i2c1.write(address_read,cmd,2,false);
    if (retour1 != 0) return 1;
    ThisThread::sleep_for(10ms); 
    return 0;


}

uint8_t SGP30::measure_air_quality( char * TVOC_mesure, char * CO2_mesure){
    char address_read= address << 1;
    char cmd[2];
    cmd[0]=0x20;
    cmd[1]=0x08;
    char data[6]{0};

    I2C i2c1(sda,sdl);
    i2c1.frequency(frequency);     
    i2c1.start();
    uint8_t retour1 = i2c1.write(address_read,cmd,2,false);
    if (retour1 != 0) return 1;
    ThisThread::sleep_for(12ms); 
    i2c1.start();
    uint8_t retour2 = i2c1.read(address_read,data,6,false);
    if(retour2 != 0) return 2;    

    uint8_t data_uint8_t[2]{static_cast<uint8_t>(data[0]), static_cast<uint8_t>(data[1])}; 
    if (data[2]== SGP30::Crc(data_uint8_t)){       
        *(CO2_mesure+0)= data[0];
        *(CO2_mesure+1)= data[1];
    } else return 3;

    data_uint8_t[0]= static_cast<uint8_t>(data[3]);
    data_uint8_t[1]= static_cast<uint8_t>(data[4]);

    if (data[5]== SGP30::Crc(data_uint8_t)){     
        *(TVOC_mesure+0)= data[3];
        *(TVOC_mesure+1)= data[4];
    } else return 5;
    i2c1.stop();
    return 0;
}


uint8_t SGP30::initialiser_mesure( char * TVOC_mesure, char * CO2_mesure){
    init_air_quality();
    for(int i=0; i<16; i++){
        measure_air_quality( TVOC_mesure, CO2_mesure);
        ThisThread::sleep_for(1s); 
    }
    measure_air_quality( TVOC_mesure, CO2_mesure);
    return 0;
}
