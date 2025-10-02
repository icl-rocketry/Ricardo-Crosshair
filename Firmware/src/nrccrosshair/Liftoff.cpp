#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"

#include "liftoff.h"
#include "system.h"

Liftoff::Liftoff(Crosshair::DefaultStateInit& DefaultInitParams)
: State(CROSSHAIR_FLAGS::STATE_LIFTOFF, DefaultInitParams.crosshairstatus),
  m_DefaultInitParams(DefaultInitParams)
{}

void Liftoff::initialize()
{
    Types::CrosshairTypes::State_t::initialize();
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Entered Liftoff state.");
}

Types::CrosshairTypes::State_ptr_t Liftoff::update()
{
    return nullptr; // waiting for apogee 
}

void Liftoff::exit()
{
    Types::CrosshairTypes::State_t::exit();
}
