#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"

#include "Armed.h"
#include "system.h"

Armed::Armed(Crosshair::PyroReadyInit& PyroInitParams, RnpNetworkManager& networkmanager, NRCCrosshair& Crosshair): 
State(CROSSHAIR_FLAGS::STATE_ARMED, PyroInitParams.crosshairstatus),
  m_networkmanager(networkmanager),
  m_PyroAdapter(PyroInitParams.m_pyroAdapter),
  m_Crosshair(Crosshair)
{};

void Armed::initialize()
{
    Types::CrosshairTypes::State_t::initialize();
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Entered Armed state.");
    
    NRCRemoteActuatorBase::arm_base(arg);
    m_Crosshair.serviceSetup()
}

Types::CrosshairTypes::State_ptr_t Armed::update()
{
    // Update internals
    m_Crosshair.DeployAltitude();

    if (m_Crosshair.isBaroApogeeReady() && baro.alt < 500 && qdrail.v < QD_RAIL_LIMIT) {
        state.transition(separation);
    }
    
    return nullptr;
}

void Armed::exit()
{
    Types::CrosshairTypes::State_t::exit();
}
