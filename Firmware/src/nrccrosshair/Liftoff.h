#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "Crosshairtypes.h"
#include "Sensors/DPS368.h"
#include "Sensors/sensorStructs.h"

class Liftoff : public Types::CrosshairTypes::State_t
{
    public:
        Liftoff(Crosshair::DefaultStateInit& DefaultInitParams, DPS368& barometer);

        void initialize() override;
        Types::CrosshairTypes::State_ptr_t update() override;
        void exit() override;

    private:
        Types::CrosshairTypes::SystemStatus_t& m_crosshairstatus;
        DPS368& m_baro;
};

