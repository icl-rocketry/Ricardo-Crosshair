#include "crosshair.h"
#include <libriccore/commands/simplecommandpacket.h>
#include <libriccore/riccorelogging.h>
#include "Config/systemflags_config.h"

#include "States/Default.h"
#include "States/Liftoff.h"
#include "States/Apogee.h"
#include "States/Separation.h"
#include "States/Pyroready.h"
#include "States/Debug.h"

void NRCCrosshair::setup()
{
    // Start FSM in default state
    m_StateMachine.initalize(std::make_unique<Default>(m_DefaultInitParams));
}

void NRCCrosshair::update()
{
    m_StateMachine.update();
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