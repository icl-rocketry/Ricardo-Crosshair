#pragma once

#include <libriccore/riccoresystem.h>

#include <memory>

#include <librrc/Remote/nrcremotepyro.h>
#include <libriccore/networkinterfaces/can/canbus.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"

#include "Commands/commands.h"

#include "Storage/sdfat_store.h"
#include "Storage/sdfat_file.h"

#include "Sensors/INA219.h"
#include "Sensors/DPS368.h"
#include "Sensors/Vrailmonitor.h"

#include "nrccrosshair/nrccrosshair.h"

#include <SPI.h>
#include <Wire.h>
class System : public RicCoreSystem<System,SYSTEM_FLAG,Commands::ID>
{
    public:

        System();
        
        void systemSetup();

        void systemUpdate();

        void serviceSetup();

        void setupI2C();

        TwoWire I2C:

        CanBus<SYSTEM_FLAG> canbus;
        INA219 PyroCurrent;
        VRailMonitor Logicrail;
        VRailMonitor QDrail;
        DPS368 Baro;
        SensorStructs::BARO_t BaroData;
        SPIClass SPIBaro;
        SPIClass SPISD;
        //Buck BB;
        NRCCrosshair Crosshair;

    private:
        void initializeLoggers();
        void logReadings();
        void setupSPI();
        void logreadings();

        const std::string log_path = "/Logs";
        const std::string config_path = "/Config";

        SdFat_Store SD;
    

};

