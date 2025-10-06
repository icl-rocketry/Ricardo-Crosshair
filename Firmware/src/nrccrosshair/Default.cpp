#include "default.h"

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include <librrc/Local/remoteactuatoradapter.h> 

#include "system.h"

Default::Default(Crosshair::DefaultStateInit& DefaultInitParams, NRCCrosshair& Crosshair)
: State(CROSSHAIR_FLAGS::STATE_DEFAULT, DefaultInitParams.crosshairstatus),
  m_DefaultInitParams(DefaultInitParams)
{}

void Default::initialize()
{
    Types::CrosshairTypes::State_t::initialize(); // call parent initialize first!
    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Default state");
}

Types::CrosshairTypes::State_ptr_t Default::update()
{
    return nullptr; 
}

void Default::exit()
{
    Types::CrosshairTypes::State_t::exit(); // call parent exit last!
}
