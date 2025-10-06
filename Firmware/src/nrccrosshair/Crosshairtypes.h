#pragma once
#include <cstdint>
#include <memory>

#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/fsm/state.h>
#include <libriccore/fsm/statemachine.h>
#include <Sensors/sensorStructs.h>

/**
 * @brief Templated struct with type aliases inside to provide convient type access. Some of the template paramters might require
 * forward declaration to prevent cylic dependancies.
 *
 * @tparam CROSSHAIR_FLAGS_T Enum of system flags
 * 
 * Taken and modified from GREG firmware
 */
enum class CROSSHAIR_FLAGS : uint32_t
{
    // state flags
    STATE_DEFAULT = (1 << 0),
    STATE_ARMED = (1 << 1),           //Command recieved from pickle
    STATE_SEPARATION = (1 << 3),      //Rbus removed
    STATE_DEBUG = (1 << 4),  
};

template <typename CROSSHAIR_FLAGS_T>
struct CrosshairTypes
{
    using SystemStatus_t = SystemStatus<CROSSHAIR_FLAGS_T>;
    using State_t = State<CROSSHAIR_FLAGS_T>;
    using State_ptr_t = std::unique_ptr<State_t>;
    using StateMachine_t = StateMachine<CROSSHAIR_FLAGS_T>;
};

namespace Types
{
    using CrosshairTypes = CrosshairTypes<CROSSHAIR_FLAGS>;
    using LocalPyro_t = NRCRemotePyro<ArduinoGpio>;
    using LocalPyroAdapter_t = RemoteActuatorAdapter<Types::LocalPyro_t>;
    using LocalPyroMap_t = std::array<LocalPyroAdapter_t *, 1>;
};

class NRCCrosshair;
namespace Crosshair
{
    struct DefaultStateInit
    {
        Types::CrosshairTypes::SystemStatus_t &crosshairstatus;
    };
    
        struct PyroReadyInit
    {
        Types::CrosshairTypes::SystemStatus_t &crosshairstatus;
        Types::LocalPyroAdapter_t &PyroAdapter;
    }
}