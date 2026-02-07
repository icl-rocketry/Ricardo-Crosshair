/** 
 * @file TPS55288.h
 * @author William Richards
 * @brief Class to manage TPS55288 I2C based buck boost converter
 * just internal resistor divider currently
 * @version 0.1
 * @date 2026-01-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once
#include "config/types.h"
#include <Wire.h>
#include "Sensors/Vrailmonitor.h"

class TPS55288 
{
public:
    /**
     * @brief TPS55288 Class
     * @param Int Fault indication pin number
     * @param CDCpin Current sensing pin, voltage output proportional to current through sense resistor
     * @param Highresistor R1 in voltage divider used for VRailMonitor library
     * @param Lowresistor R2 in voltage divider used for VrailMonitor library
     * @param Vout Desired output voltage
     */

    TPS55288(TwoWire &i2c, Types::CoreTypes::SystemStatus_t& systemstatus, uint8_t INT,  uint8_t CDCpin, int Highresistor = 0, int Lowresistor = 1, float Vout = 5, uint8_t Device_address = 0x74);
    

    
    

    
    
    void setup();

    void enable();

    void update();

    void disable();

    enum class Registers : uint8_t
    {
        REF1 = 0x0,
        REF2 = 0x1,
        IOUT_LIMIT = 0x2,
        VOUT_SR = 0x3,
        VOUT_FS = 0x4,
        CDC = 0x5,
        MODE = 0x6,
        STATUS = 0x7,
    };

    void Writeregister(TPS55288::Registers address, uint8_t value);

    struct VoltageOutput
    {
        uint8_t VoltageMSB1;
        uint8_t VoltageLSB1;
    };

    VoltageOutput Vcalculation(uint8_t m_Vout);

private:

    std::function<std::string(uint8_t)> errFunc = [this](uint8_t code){
        switch (code)
        {
        case 0:
            return "success.";
        case 1:
            return "data too long to fit in transmit buffer.";
        case 2:
            return "received NACK on transmit of address.";
        case 3:
            return "received NACK on transmit of data.";
        case 4:
            return "other error.";
        case 5:
            return "timeout";
        default:
            return "unrecognised err code";
        }
    };

    VoltageOutput Out;

    //Output V from register + Output current from CDC ADC
    float OutputV;
    float OutputI;

    //System flags
    Types::CoreTypes::SystemStatus_t &m_systemstatus;

    //Desired voltage
    int m_Vout;

    //device address (useful when multiple items on the i2c bus)
    uint8_t m_deviceaddress;


    /** 
     * @brief register Values
     * 
     * @param VoltageMSB REF1 value
     * @param VoltageLSB REF2 value 
     * Default values set to enable 5V output
     * 
     * @param FeedbackRatio VOUT_FS value
     * Default value set to enable 5V output
     * 
     * 
     * 
     * @param Modes MODE value
     * Default value enables Buck Boost and enables the hiccup during output short circuit protection
     * 
     * 
     */
 
    uint8_t VoltageMSB = 0x0;
    uint8_t VoltageLSB = 11010010;
    uint8_t FeedbackRatio = 0x0 | (1 << 1) | (1 << 0);
    uint8_t Modes = 0x0 | (1 << 7) | (1 << 5);


    VRailMonitor m_CurrentReading;

    uint8_t INT;
    uint8_t m_interruptpin;
    
    TwoWire &m_i2c;

    uint8_t err = 0;
};