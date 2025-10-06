#pragma once

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include "Crosshairtypes.h"

#include "nrccrosshair.h"

class Separation: public Types::CrosshairTypes ::State_t
{
    public:

    Separation(Crosshair::PyroReadyInit& PyroInitParams, RnpNetworkManager& networkmanager, NRCCrosshair& Crosshair);

    void initialize() override;

    Types::CrosshairTypes::State_ptr_t update() override;

    void exit() override;


    private:

    Crosshair::PyroReadyInit& m_PyroInitParams;
    RnpNetworkManager& m_networkmanager;
    NRCCrosshair& m_Crosshair;
    Types::LocalPyroAdapter_t& m_PyroAdapter;
    
    

    uint8_t m_IgnitionCalls;
    uint64_t m_ignitionTime;
    uint64_t m_pyroTime = 500; 
   
    int m_BaroCounter;
    bool m_below500;
  
   

};