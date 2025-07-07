#pragma once
#include <cstdint>
#include <memory>

#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/fsm/state.h>
#include <libriccore/fsm/statemachine.h>

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
    STATE_LIFTOFF = (1 << 1),           //Command recieved from pickle
    STATE_APOGEE = (1 << 2),            //Command recieved from pickle
    STATE_SEPARATION = (1 << 3),        //Command recieved from pickle
    STATE_PYROREADY = (1 << 4),
    STATE_DEBUG = (1 << 5),  
    /*
    // Local fuel tank codes
    ERROR_FUELTANKP_LOCAL = (1 << 8),     // Abort triggered by some local fuel tank error
    ERROR_FTP_LOCAL_DC = (1 << 9),        // Trigerred by disconnect
    ERROR_FTP_LOCAL_COVP = (1 << 10),     // Critical Overpressure
    ERROR_FTP_LOCAL_HOVP = (1 << 11),     // Half abort overpressure
    // Remote fuel tank codes
    ERROR_FUELTANKP_REMOTE = (1 << 12),    // Abort triggered by some remote fuel tank error
    ERROR_FTP_REMOTE_DC = (1 << 13),        // Disconnect
    ERROR_FTP_REMOTE_NORESPONSE = (1 << 14),// No response
    ERROR_FTP_REMOTE_COVP = (1 << 15),       // Critical Overpressure
    ERROR_FTP_REMOTE_HOVP = (1 << 16),
    // Ox tank error codes
    ERROR_OXTANKP_REMOTE = (1 << 17),      // Some ox tank error
    ERROR_OXP_REMOTE_DC = (1 << 18),        // Disconnect
    ERROR_OXP_REMOTE_NORESPONSE = (1 << 19),// No response
    ERROR_OXP_REMOTE_COVP = (1 << 20),       // Critical Overpressure
    ERROR_OXP_REMOTE_HOVP = (1 << 21),       
    // Nitrogen tank error codes
    ERROR_N2P_REMOTE = (1 << 22),          // Some NITROGEN tank error
    ERROR_N2P_REMOTE_DC = (1 << 23),        // Disconnect
    ERROR_N2P_REMOTE_NORESPONSE = (1 << 24),// No response */
};

template <typename CROSSHAIR_FLAGS_T>
struct CrosshairTypes
{
    using SystemStatus_t = SystemStatus<CROSSHAIR_FLAGS_T>;
    using State_t = State<CROSSHAIR_FLAGS_T>;
    using State_ptr_t = std::unique_ptr<State_t>;
    using StateMachine_t = StateMachine<CROSSHAIR_FLAGS_T>;
};