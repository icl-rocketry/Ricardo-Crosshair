#pragma once

#include <librrc/Remote/nrcremoteactuatorbase.h>
#include <librrc/Remote/nrcremotepyro.h>
#include <librrc/Helpers/sensorpoller.h>
#include <librnp/rnp_networkmanager.h>
#include <libriccore/fsm/statemachine.h>
#include <libriccore/systemstatus/systemstatus.h>

#include "crosshairtypes.h"
#include "Config/systemflags_config.h"
#include "Config/services_config.h"

#include "Sensors/DPS368.h"
#include "Sensors/INA219.h"

#include "States/Default.h"
#include "States/Liftoff.h"
#include "States/Apogee.h"
#include "States/Separation.h"
#include "States/Pyroready.h"
#include "States/Debug.h"

class NRCCrosshair : public NRCRemoteActuatorBase<NRCCrosshair>
{
public:
    NRCCrosshair(RnpNetworkManager& nm, Types::LocalPyro_t& pyro, INA219& cs, DPS368& baro)
        : NRCRemoteActuatorBase(nm),
          m_networkmanager(nm),
          m_pyro(pyro),
          m_pyroAdapter(0, m_pyro, [](const std::string& msg){ RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg); }),
          m_currSensor(cs),
          m_baro(baro),
          m_CrosshairStatus(),
          m_DefaultInitParams{m_CrosshairStatus},
          m_PyroInitParams{m_CrosshairStatus, m_pyroAdapter}
    {}

    void setup();
    void update();

protected:
    void execute_impl(packetptr_t packetptr) override;

private:
    RnpNetworkManager& m_networkmanager;

    // Hardware
    Types::LocalPyro_t& m_pyro;
    Types::LocalPyroAdapter_t m_pyroAdapter;
    INA219& m_currSensor;
    DPS368& m_baro;

    // FSM
    Types::CrosshairTypes::StateMachine_t m_StateMachine;
    Types::CrosshairTypes::SystemStatus_t m_CrosshairStatus;

    Crosshair::DefaultStateInit m_DefaultInitParams;
    Crosshair::PyroReadyInit m_PyroInitParams;
};
