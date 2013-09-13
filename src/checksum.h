/*
 * checksum.h
 *
 *  Created on: 05.12.2012
 *      Author: andrey
 */

#ifndef ASKUE_CHECKSUM_H_
#define ASKUE_CHECKSUM_H_

#include <stdint.h>
#include <stdlib.h>

uint16_t checksum_crc16 ( const uint8_t *data, size_t len );

uint8_t checksum_simple ( const uint8_t *data, size_t len);

#endif /* ASKUE_CHECKSUM_H_ */
