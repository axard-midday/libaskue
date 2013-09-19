#ifndef ASKUE_PORT_H_
#define ASKUE_PORT_H_

#include <stdlib.h>

#include "uint8_array.h"
#include "types.h"

// закрыть порт
int port_destroy ( a_port_t *Port );
// настроить порт
int port_init ( a_port_t *Port, const char *file, const char *speed, const char *dbits, const char *sbits, const char *parity );
// читать из порта
// если Amount == 0, то отслеживание по кол-ву принятых байт не производится
int port_read ( const a_port_t *Port, uint8_array_t *u8a, long int timeout, size_t Amount );
// писать в порт
int port_write ( const a_port_t *Port, const uint8_array_t *u8a );

#endif /* ASKUE_PORT_H_ */
