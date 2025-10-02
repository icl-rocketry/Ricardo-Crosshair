#include "Apogee.h"

#include <memory>
#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "system.h"

Apogee::Apogee(Crosshair::DefaultStateInit& DefaultInitParams)
: State(CROSSHAIR_FLAGS::STATE_APOGEE, DefaultInitParams.crosshairstatus),
  m_DefaultInitParams(DefaultInitParams)
{}

void Apogee::initialize()
{
    Types::CrosshairTypes::State_t::initialize();
}

Types::CrosshairTypes::State_ptr_t Apogee::update()
{
    return nullptr;
}

void Apogee::exit()
{
    Types::CrosshairTypes::State_t::exit();
}
