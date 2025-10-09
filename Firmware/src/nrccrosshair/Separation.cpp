#include "Separation.h"

#include <memory>

#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

#include "Config/systemflags_config.h"
#include "Config/types.h"
#include <librrc/Local/remoteactuatoradapter.h>

#include "system.h"

// Transition into separation state when RBUS voltage drops below 

Separation::Separation(Crosshair::PyroReadyInit& PyroInitParams,RnpNetworkManager& networkmanager, NRCCrosshair& Crosshair):
State(CROSSHAIR_FLAGS::STATE_SEPARATION,PyroInitParams.crosshairstatus),
m_PyroInitParams(PyroInitParams),
m_networkmanager(networkmanager),
m_Crosshair(Crosshair),
m_PyroAdapter(PyroInitParams.m_pyroAdapter),
m_BaroCounter(PyroInitParams.m_baroCounter),
m_below500(PyroInitParams.m_below500)

{};

void Separation::initialize()
{
    Types::CrosshairTypes::State_t::initialize();

    m_PyroAdapter.arm(0); 

    m_IgnitionCalls = 0; //Initialise Ignition calls
    m_ignitionTime = millis();
};

Types::CrosshairTypes::State_ptr_t Separation::update()
{
    // Check ignition calls and fire pyro accordingly

    if (m_IgnitionCalls > 0) //Ignition has been called already
    {
        if(millis() - m_ignitionTime < m_pyroTime)
        {
            return nullptr;
        }
        else
        {
            m_PyroAdapter.disarm();
            return std::make_unique<Default>(m_PyroInitParams, m_networkmanager, m_Crosshair);
        }
    
    }


    /*
        When barocounter is >= 1, and Separation state is true, pyro will fire
        barcounter only adds once altitude is greater than 450m, 
        Separation state is only true when separation has occured,
        Pyro will only be armed
        need to think abt whether there is armed flag or smth from the m_Crosshair.m_pyroAdapter i can pass in

        This should be sorted, using a baro counter of greater than or equal to 2 to trigger 
    */
    else if (m_IgnitionCalls == 0 && m_BaroCounter>= 2 && m_below500)
    {
        
        m_Crosshair.m_pyroAdapter.execute(m_pyroTime);
        m_IgnitionCalls++;


    }

    return nullptr;
}

void Separation::exit()
{
    Types::CrosshairTypes::State_t::exit();
};