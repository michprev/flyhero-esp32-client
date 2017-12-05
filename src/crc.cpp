//
// Created by michal on 17.10.17.
//

#include "crc.h"


uint16_t crc16(uint8_t *data, uint8_t length)
{
    const uint16_t CRC_POLYNOME = 0x1021;
    uint16_t crc = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= (int16_t) (data[i]) << 8;

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ CRC_POLYNOME;
            else
                crc = (crc << 1);
        }
    }

    return crc;
}