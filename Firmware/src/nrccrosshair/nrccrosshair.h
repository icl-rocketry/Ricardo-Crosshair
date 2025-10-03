#pragma once

#include <librrc/Remote/nrcremoteactuatorbase.h>
#include <librrc/Remote/nrcremotepyro.h>
#include <librrc/Helpers/sensorpoller.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/fsm/statemachine.h>
#include <libriccore/systemstatus/systemstatus.h>

#include "Crosshairtypes.h"
#include "config/systemflags_config.h"
#include "config/services_config.h"
#include "config/pinmap_config.h"
#include "config/general_config.h"

#include "Sensors/DPS368.h"
#include "Sensors/INA219.h"
#include "Sensors/Vrailmonitor.h"
#include "Sensors/sensorStructs.h"

#include <SPI.h>
#include <Wire.h>

#include "Default.h"
#include "Liftoff.h"
#include "Apogee.h"
#include "Separation.h"
#include "Pyroready.h"
#include "Debug.h"

class NRCCrosshair : public NRCRemoteActuatorBase<NRCCrosshair>
{
    public:
    NRCCrosshair(RnpNetworkManager& networkmanager, DPS368& Barometer, SensorStructs::BARO_t& Barometer_data):
        m_networkmanager(networkmanager),
        m_Baro(Barometer),
        m_BaroData(Barometer_data)
        {};

    void setup();
    void update();

    bool isBaroApogeeReady() const { return m_baroCounter >= 1;}

    protected:
    void execute_impl(packetptr_t packetptr) override;
    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;

    private:
    RnpNetworkManager& m_networkmanager;


    // Hardware
    Types::LocalPyro_t& m_pyro;
    Types::LocalPyroAdapter_t m_pyroAdapter;
    INA219& m_currSensor;
    VRailMonitor& m_QDrail;
    DPS368& m_Baro;
    SensorStructs::BARO_t& m_BaroData;

    // FSM
    Types::CrosshairTypes::StateMachine_t m_StateMachine;
    Types::CrosshairTypes::SystemStatus_t m_CrosshairStatus;
    Crosshair::DefaultStateInit m_DefaultInitParams;
    Crosshair::PyroReadyInit m_PyroInitParams;

    // Initial constants
    int m_baroCounter = 0;
    bool m_below500 = true;
};
