#include "nrccrosshair.h"
#include <libriccore/commands/simplecommandpacket.h>
#include <libriccore/riccorelogging.h>


#include "Default.h"
#include "Liftoff.h"
#include "Apogee.h"
#include "Separation.h"
#include "Pyroready.h"
#include "Debug.h"

NRCCrosshair::NRCCrosshair(RnpNetworkManager& networkmanager):
                    NRCRemoteActuatorBase(networkmanager),
                    _networkmanager(networkmanager),
                    //m_currSensor(),
                    m_QDrail(m_QDrail, PinMap::QDV, GeneralConfig::LOGIC_r1, GeneralConfig::LOGIC_r2),
                    m_pyro(PinMap::PyroNuke, PinMap::PyroCont, _networkmanager),
                    m_pyroAdapter(0,Pyro,[](const std::string& msg){RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(msg);})

{}; 


void NRCCrosshair::setup()
{
    m_pyro.setup();
    m_QDrail.setup();
    // Start FSM in default state
    m_StateMachine.initalize(std::make_unique<Default>(m_DefaultInitParams, *this));
}


void NRCCrosshair::update()
{
    _value = m_CrosshairStatus.getStatus()
    float altitude = m_BaroData.alt;
    m_StateMachine.update();
    m_QDrail.update();
    //m_currSensor.update();
}

void NRCCrosshair::DeployAltitude(){
    if (baroDara.alt > 450 && m_below500)
    {
        m_baroCounter++;
        m_below = false;
    }
    else if (m_BaroData.alt < 500){
        m_below500 = true;
    }

}

void NRCCrosshair::arm_base(int32_t arg){
    m_pyroAdapter.arm(arg);

    if (m_pyroAdapter.getState(.flagSet(LIBRRC::COMPONENT_STATUS_FLAGS::NOMINAL))&& isBaroApogeeReady())
    {
        NRCRemoteActuatorBase::arm_base(arg);
    }
    else{
        m_pyroAdapter.disarm()
    }
}

void NRCCrosshair::execute_impl(packetptr_t packetptr)
{
    SimpleCommandPacket cmd(*packetptr);

    switch (cmd.arg)
    {
    case 1: // Liftoff
        if (!m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_DEFAULT)) break;
        m_StateMachine.changeState(std::make_unique<Liftoff>(m_DefaultInitParams, m_baro));
        break;

    case 2: // Apogee
        if (!m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_LIFTOFF)) break;
        m_StateMachine.changeState(std::make_unique<Apogee>(m_DefaultInitParams));
        break;

    case 3: // Separation
        if (!m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_APOGEE)) break;
        m_StateMachine.changeState(std::make_unique<Separation>(m_PyroInitParams));
        break;

    case 4: // PyroReady
        if (!m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_SEPARATION)) break;
        m_StateMachine.changeState(std::make_unique<PyroReady>(m_PyroInitParams));
        break;

    case 5: // Debug
        if (!m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_DEFAULT)) break;
        m_StateMachine.changeState(std::make_unique<Debug>(m_DefaultInitParams));
        break;

    default:
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Unknown command ID!");
        break;
    }
}

void NRCCrosshair::extendedCommandHandler_impl(const NRCPacket::NRC_COMMAND_ID commandID, packetptr_t packetptr)
{
    SimpleCommandPacket command_packet(*packetptr);
    switch (static_cast<uint8_t>(commandID))
    {
    case 6:
    {
        if (m_CrosshairStatus.flagSet(CROSSHAIR_FLAGS::STATE_DEBUG))
        {
            //m_regAdapter.execute(command_packet.arg);
        }
        else
        {
            break;
        }
    }
    case 7: // command to set pressurise angle
    {
        if (command_packet.arg > (m_regPressuriseAngle + 50))
        {
            break;
        }
        else
        {
            m_regPressuriseAngle = command_packet.arg;
        }
        break;
    }

    default:
    {
        NRCRemoteActuatorBase::extendedCommandHandler_impl(commandID, std::move(packetptr));
        break;
    }
    }
}