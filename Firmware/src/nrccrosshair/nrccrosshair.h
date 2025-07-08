#pragma once

#include <libriccore/fsm/statemachine.h>
#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <librrc/Remote/nrcremoteactuatorbase.h>
#include <librrc/Local/remoteactuatoradapter.h>
#include <librrc/Local/remotesensoradapter.h>

#include <librrc/HAL/localpwm.h>

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>

#include "config/types.h"
#include "config/pinmap_config.h"
#include "config/services_config.h"
#include "Sensors/ADS131M06.h"

#include <librrc/Remote/nrcremoteservo.h>
#include <librrc/Remote/nrcremoteptap.h>
#include <librrc/Remote/nrcremotepyro.h>

#include "Crosshairtypes.h"