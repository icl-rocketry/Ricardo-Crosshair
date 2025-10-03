#pragma once

namespace PinMap{

    static constexpr int BARO_MISO = 14;
    static constexpr int BARO_MOSI = 13;
    static constexpr int BARO_SCLK = 11;
    static constexpr int BARO_CS = 12;


    static constexpr int MagCs = 15;
    static constexpr int ImuCs_1 = 16;
    static constexpr int ImuCs_2 = 17;

    static constexpr int _SDA = 36;
    static constexpr int _SCL = 35;

    static constexpr int SD_SCLK = 48;
    static constexpr int SD_MISO = 47;
    static constexpr int SD_MOSI = 2;
    static constexpr int SD_CS = 5;


    static constexpr int TxCan = 18;
    static constexpr int RxCan = 17;

    static constexpr int PyroNuke = 38;
    static constexpr int PyroCont = 39,
    static constexpr int BatteryV = 15;
    static constexpr int QDV = 10;
    static constexpr int ID_Stat = 1;

    static constexpr int BB_CDC = 40;
    static constexpr int BB_INT = 41;
    static constexpr int Cell_switch = 42;
};


