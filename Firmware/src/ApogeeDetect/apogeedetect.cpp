#include "apogeedetect.h"
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Arduino.h>

#include <libriccore/riccorelogging.h>

#include <sstream>

ApogeeDetect::ApogeeDetect(uint16_t sampleTime) :
    _sampleTime(sampleTime),
    valid_points(0),
    mlock(true),                 // intialise the mach lockout stuff
    _apogeeinfo({false, 0, 0})
{
}

// Function to update flight data values and return data for apogee prediction
const ApogeeInfo &ApogeeDetect::checkApogee(float altitude, float velocity, uint32_t time)
{
    if(!initialEntryTime)
    {
        initialEntryTime = time; // recording first time this method is called to scale the system better
    }

    if (millis() - prevCheckApogeeTime > _sampleTime)
    {
        uint32_t timeSinceEntry = time - initialEntryTime;

        uint32_t prevTime = time_array.pop_push_back(timeSinceEntry);
        float prevAltitude = altitude_array.pop_push_back(altitude);

        // Keep track of valid data points to prevent matrix singularities
        // from zero-filled initialized buffers.
        if (valid_points < altitude_array.size()) {
            valid_points++;
        }

        // Mach lock check:
        if (velocity >= mlock_speed)
        {
            if (!mlock){
                RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Mach Lock Triggered!");
            }
            mlock = true;
            prevMachLockTime = millis();
        }
        else if (mlock && ((millis() - prevMachLockTime) > mlock_time)) // Guarded with mlock boolean
        {
            mlock = false;
            RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Mach unlocked");
        }

        // Apogee detection:

        // Wait until we have at least 5 valid points to do a quadratic fit
        if (!(_apogeeinfo.reached) && valid_points >= 5)
        {
            quadraticFit((float)prevTime/1000.0f, (float)timeSinceEntry/1000.0f, prevAltitude, altitude);

            // Ensure the parabola opens downwards (coeffs(2) < 0) before calculating time_to_peak
            if (coeffs(2) < 0.0f)
            {
                float time_to_peak = -coeffs(1) / (2.0f * coeffs(2));

                // Prevent predicting negative times which will underflow into UINT32_MAX
                if (time_to_peak > 0.0f)
                {
                    _apogeeinfo.time = (uint32_t)((time_to_peak * 1000.0f) + initialEntryTime);

                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(std::to_string(_apogeeinfo.time));
                    RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("coeffs: " + std::to_string(coeffs(0)) + " " + std::to_string(coeffs(1)) + " " + std::to_string(coeffs(2)));

                    if ((millis() >= _apogeeinfo.time) && (millis() > 0) && (altitude > alt_min) && !mlock)
                    {
                        _apogeeinfo.altitude = coeffs(0) - (std::pow(coeffs(1), 2) / (4.0f * coeffs(2)));
                        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("predicted apogee " + std::to_string(_apogeeinfo.time));
                        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>(std::to_string(altitude - _apogeeinfo.altitude));

                        if ((altitude - _apogeeinfo.altitude) < alt_threshold)
                        {
                            _apogeeinfo.reached = true;
                        }
                    }
                }
            }
        }
        prevCheckApogeeTime = millis();
    }

    return _apogeeinfo;
}

void ApogeeDetect::updateSigmas(float oldTime, float newTime, float oldAlt, float newAlt)
{
    float newTime_2 = newTime * newTime;
    float newTime_3 = newTime_2 * newTime;
    float newTime_4 = newTime_3 * newTime;

    float oldTime_2 = oldTime * oldTime;
    float oldTime_3 = oldTime_2 * oldTime;
    float oldTime_4 = oldTime_3 * oldTime;

    sigmaTime += (newTime - oldTime);
    sigmaTime_2 += (newTime_2 - oldTime_2);
    sigmaTime_3 += (newTime_3 - oldTime_3);
    sigmaTime_4 += (newTime_4 - oldTime_4);

    sigmaAlt += (newAlt - oldAlt);

    sigmaAltTime += ((newAlt * newTime) - (oldAlt * oldTime));
    sigmaAltTime_2 += ((newAlt * newTime_2) - (oldAlt * oldTime_2));
}

void ApogeeDetect::quadraticFit(float oldTime, float newTime, float oldAlt, float newAlt)
{
    updateSigmas(oldTime, newTime, oldAlt, newAlt);

    // Matrix populated using actual valid_points instead of buffer capacity
    A << (float)valid_points, sigmaTime, sigmaTime_2,
        sigmaTime, sigmaTime_2, sigmaTime_3,
        sigmaTime_2, sigmaTime_3, sigmaTime_4;

    b << sigmaAlt, sigmaAltTime, sigmaAltTime_2;

    coeffs = A.colPivHouseholderQr().solve(b);
}