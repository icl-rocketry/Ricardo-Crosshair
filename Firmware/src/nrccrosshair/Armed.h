#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "Crosshairtypes.h"
#include "Sensors/sensorStructs.h"

class Armed : public Types::CrosshairTypes::State_t
{
    public:
        Armed(Crosshair::PyroReadyInit& PyroInitParams, RnpNetworkManager& networkmanager, NRCCrosshair& Crosshair);

        void initialize() override;
        Types::CrosshairTypes::State_ptr_t update() override;
        void exit() override;

    private:
        RnpNetworkManager& m_networkmanager;
        NRCCrosshair& m_Crosshair;
        Types::CrosshairTypes::SystemStatus_t& m_crosshairstatus;
        Types::LocalPyroAdapter_t& m_PyroAdapter;
};

