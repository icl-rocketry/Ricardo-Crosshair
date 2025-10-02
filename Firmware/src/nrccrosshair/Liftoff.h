#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "Crosshairtypes.h"
#include "nrccrosshair.h"

class Liftoff : public Types::CrosshairTypes::State_t
{
    public:
        Liftoff(Crosshair::DefaultStateInit & DefaultInitParams)
}