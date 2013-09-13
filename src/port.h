#ifndef ASKUE_PORT_H_
#define ASKUE_PORT_H_

#include <stdio.h>
#include <termios.h>

#include "uint8_array.h"

typedef struct _askue_port_t
{
    int RS232;
    FILE *In;
    FILE *Out;
    struct termios Termios;
} askue_port_t;

// закрыть порт
int port_destroy ( askue_port_t *Port );
// настроить порт
int port_init ( askue_port_t *Port, const char *file, const char *speed, const char *dbits, const char *sbits, const char *parity );
// читать из порта
int port_read ( const askue_port_t *Port, uint8_array_t *u8a, long int timeout );
// писать в порт
int port_write ( const askue_port_t *Port, const uint8_array_t *u8a );

#endif /* ASKUE_PORT_H_ */
