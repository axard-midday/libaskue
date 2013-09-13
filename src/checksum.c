#include <stdint.h>
#include <stdlib.h>

/*
 * Функции расчёта контрольных сумм
 */
uint16_t checksum_crc16 ( const uint8_t *data, size_t len )
{
    uint16_t crc_16 = 0x0000ffff;

    for ( size_t i = 0; i < len; i++ )
    {
        crc_16 ^= data[i];
        for ( size_t j = 0; j < 8; j++ )
		{
            if( crc_16 & 0x0001 )
                crc_16 = ( ( crc_16 >> 1 ) & 0x7fff) ^ 0xa001;
            else
                crc_16 = ( crc_16 >> 1 ) & 0x7fff;
		}
    }

    return crc_16;
}

uint8_t checksum_simple ( const uint8_t *data, size_t len)
{
    uint8_t crc = 0;

 	for ( size_t i = 0; i < len; i++ )
       		crc += data[i];

    return crc;
}

