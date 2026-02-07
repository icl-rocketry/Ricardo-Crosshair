Information regarding TPS55288 hardware implementation on Crosshair V1.0 (as of 25/01/2026).

EN pin voltage divider:
Input voltage at ~3.4V is below the EN voltage, thus 1S lipo at low battery cannot be used for the pyro channel

ISP/ISN pins:
Consider routing as differential pair

Register addresses:
Written in Hex, e.g. 0x0, 0x1, 0x2, ...
When writing them to I2C, type is uint8_t (max 8 bits)