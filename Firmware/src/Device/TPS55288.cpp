#include "TPS55288.h"
TPS55288::TPS55288(TwoWire &i2c, Types::CoreTypes::SystemStatus_t &systemstatus, uint8_t INT, uint8_t CDCpin, int Highresistor = 0, int Lowresistor = 1, float Vout = 5, uint8_t Device_address):
    m_interruptpin(INT),
    m_systemstatus(systemstatus),
    m_Vout(Vout),
    m_CurrentReading("Current output", CDCpin, Highresistor, Lowresistor),
    m_i2c(i2c),
    m_deviceaddress(Device_address){};


//probably redo code to take input into setup, rather than the object declaration itself
//removed 


//investigate whether to be uint8_t or void function

/*

    Vref = Vout * ratio
    Ratios: 
    00: 0.2256
    01: 0.1128
    10: 0.0752
    11: 0.0564 

    Vref from 45mV to 1.2V with 1.129mV steps

*/
TPS55288::VoltageOutput TPS55288::Vcalculation(uint8_t m_Vout){
    uint8_t Voltagestep;
    



    //Modes = 0x0;
    Out;
    return Out; //Modes, VoltageMSB, VoltageLSB; //investigate how to return data as a function
};

void TPS55288::Writeregister(TPS55288::Registers address, uint8_t value)
{
    m_i2c.beginTransmission(m_deviceaddress);
    m_i2c.write(static_cast<uint8_t>(address));
    m_i2c.write(value);
    if ((err = m_i2c.endTransmission()) != 0) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Crosshair Failed to send I2C to BuckBoost! err : " + std::to_string(err) + " -- " + errFunc(err));
        return;
    }

};


void TPS55288::setup()
{
    //removed i2c declaration in TPS5528 library, should be completed during the setup of the i2c itself 
    /*if (!i2c.begin(m_SDA, m_SCL, m_freq)) {
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Couldnt start I2C");
    return;
    }

    delay(500);*/

    //Vcalculation();
    //i2c.write(MODE);
    //i2c.write(Modes);
    
    //Figure out how to write LSB and MSB to the registers
};