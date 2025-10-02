#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "crosshairtypes.h"

class Apogee : public Types::CrosshairTypes::State_t
{
    public:
        Apogee(Crosshair::DefaultStateInit& DefaultInitParams);

        void initialize() override;

        Types::CrosshairTypes::State_ptr_t update() override;

        void exit() override;


    private:
        Crosshair::DefaultStateInit& m_DefaultInitParams;
        DPS368& Baro;
};