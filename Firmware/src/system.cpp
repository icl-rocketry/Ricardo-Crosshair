#include "system.h"

#include <memory>
#include <string>

#include <libriccore/riccoresystem.h>

#include <HardwareSerial.h>

#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Config/general_config.h"
#include "Config/services_config.h"

#include "Loggers/TelemetryLogger/telemetrylogframe.h"

#include "Commands/commands.h"
#include <librnp/default_packets/simplecommandpacket.h>

#include "States/idle.h"

#include "Storage/sdfat_store.h"
#include "Storage/sdfat_file.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System():
RicCoreSystem(Commands::command_map,Commands::defaultEnabledCommands,Serial),
SPISD(0),
SD(SPISD,PinMap::SD_CS,SD_SCK_MHZ(20),false,&systemstatus),
SPIBaro(1),
Baro(SPIBaro, Types::CoreTypes::SystemStatus_t, PinMap::BARO_CS)
//BB(SPIBB,PinMap::BB_CS,SD_SCK_MHZ(20),false,&systemstatus),
Logicrail(Logicrail, PinMap::BatteryV, GeneralConfig::LOGIC_r1, GeneralConfig::LOGIC_r2),
Crosshair(networkmanager, Baro)

{};


void System::systemSetup(){
    
    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);

    delay(2000);

    setupPins();
    setupI2C();
    setupSPI();

    
    initializeLoggers();

    SPISD.begin(PinMap::SD_SCLK,PinMap::SD_MISO,PinMap::SD_MOSI);
    SPISD.setFrequency(SD_SCK_MHZ(20));

    SPIBaro.begin(PinMap::BARO_SCLK, PinMap::BARO_MISO, PinMap::BARO_MOSI);
    SPISD.setFrequency(SD_SCK_MHZ(20));

    pinMode(PinMap::sdcs,OUTPUT);
    digitalWrite(PinMap::sdcs,HIGH);
   
    //intialize rnp message logger
    loggerhandler.retrieve_logger<RicCoreLoggingConfig::LOGGERS::SYS>().initialize(networkmanager);

    //initialize statemachine with idle state
    statemachine.initalize(std::make_unique<Idle>(systemstatus,commandhandler));
    //any other setup goes here

    networkmanager.setNodeType(NODETYPE::HUB);
    networkmanager.setNoRouteAction(NOROUTE_ACTION::BROADCAST,{1,3});
    networkmanager.addInterface(&canbus);

    uint8_t Crosshairservice = (uint8_t) Services::ID::Crosshair;
    networkmanager.registerService(Crosshairservice,NRCCrosshair.getThisNetworkCallback());

    canbus.setup();
    SD.setup();
    Baro.setup();
    Logicrail.setup();
    Crosshair.setup();
};


void System::systemUpdate(){
    Crosshair.update()
    Logicrail.update()
    Baro.update()
};