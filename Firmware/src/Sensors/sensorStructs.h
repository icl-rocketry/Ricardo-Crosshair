#pragma once
/*
Baro, INA current sense, and ADC sense to help with separation
*/

#include <cstdint>

namespace SensorStructs
{
    struct BARO_t
    {
        float alt;   // altitude 
        float temp;  // temperature 
        float press; // pressure 
    };

    struct ADC_V_RAIL_t
    {
        int volt;    // Voltage in mV
        int percent; // Percentage in reference to max voltage expeceted
    };

    struct INA_V_RAIL_t
    {
        int volt;    // Voltage in mV
        int current; // Current in mA
        int power;   // Power in mW
        int percent; // Percentage in reference to max voltage expeceted
    };

    struct STATE_t
    {
        // ADC GPIO monitoring
        ADC_V_RAIL_t BatteryV_LOG; // GPIO15
        ADC_V_RAIL_t DeployV_LOG;  // GPIO10

        // Barometer sensor
        BARO_t baro;

        // INA219 current sensor for pyro
        INA_V_RAIL_t deprail;
    };
}
