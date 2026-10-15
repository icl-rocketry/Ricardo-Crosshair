#include "Armed.h"

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"

#include "system.h"

/**
 * @brief The Armed state will evaluate state changes based on data read in
 * from main crosshair module.
 *
 * It will transition to deploy state if needed.
 */
Armed::Armed(Types::CoreTypes::SystemStatus_t& systemstatus, Types::CoreTypes::CommandHandler_t& commandhandler, NRCCrosshair& crosshair):
        State(SYSTEM_FLAG::STATE_ARMED,systemstatus),
        systemstatus(systemstatus),
        commandhandler(commandhandler),
        apogeeDetector(20),
        crosshair(crosshair) {};

void Armed::initialize() {
    State::initialize(); // call parent initialize first!

    armStartTime = millis();
};

Types::CoreTypes::State_ptr_t Armed::update() {
    // if (millis() - prevLogMessageTime > 1000) {
    //     RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Armed heartbeat! Alt : " + std::to_string(crosshair.smoothedBaroAlt) + ", prs : " + std::to_string(crosshair.baroData.press) + ", rbus voltage : " + std::to_string(crosshair.qdRailVoltage) + ", lowvt : " + std::string(crosshair.lowVoltageTriggered ? "true" : "false") + ", overdep : " + std::string(hasGoneOverDeploymentAlt ? "true" : "false"));
    //     prevLogMessageTime = millis();
    // }

    const float altitude = crosshair.smoothedBaroAlt;

    static uint64_t apogeeTime = 0;
    if (millis() - apogeeTime > 20) {
        if (apogeeTime != 0 && m_inDangerZone) {
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Alt: " + std::to_string(altitude));
            const ApogeeInfo& info = apogeeDetector.checkApogee(altitude, 0, millis());
            if (info.reached) {
                m_apogeeReached = true;
            }
        }

        m_prevAltitude = altitude;
        apogeeTime = millis();
    }

    static bool printApogee = true;
    if (printApogee && m_apogeeReached) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Apogee Reached");
        printApogee = false;
    }

    if (altitude >= GeneralConfig::ARMING_ALTITUDE) {
        m_inDangerZone = true;
    }

    static bool printDangerZone = true;
    if (printDangerZone && m_inDangerZone) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Danger Zone");
        printDangerZone = false;
    }

    m_stageDeployed = crosshair.lowVoltageTriggered;
    m_stageDeployed = true;

    // Test conditions for deployment
    if (m_inDangerZone && m_stageDeployed && m_apogeeReached && altitude <= GeneralConfig::DEPLOYMENT_ALTITUDE) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Deploying!");
        return std::make_unique<Deploy>(systemstatus, commandhandler, crosshair);
    }

    m_prevTime = millis();

    return nullptr;
};

void Armed::exit() {
    Types::CoreTypes::State_t::exit(); // call parent exit last!
};