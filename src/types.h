#ifndef ASKUE_TYPES_H_
#define ASKUE_TYPES_H_

#include <termios.h>
#include <sqlite3.h>

#include "macro.h"

typedef struct a_value_s
{
    int Base;    // основание степени
    int Exp;     // показатель степени
} a_value_t;

typedef struct a_time_s
{
    char Value[ TIME_STRBUF + 1 ];
} a_time_t;

typedef struct a_date_s
{
    char Value[ DATE_STRBUF + 1 ];
} a_date_t;

typedef uint64_t a_device_t;

typedef struct a_type_s
{
    char Value[ 32 ];
} a_type_s;

typedef struct a_record_s
{
    a_device_t Device;
    a_value_t Value;
    a_type_s Type;
    a_date_t Date;
    a_time_t Time;
} a_record_t;

typedef struct a_key_s
{
    a_device_t Device;
    a_type_s Type;
    a_date_t Date;
    a_time_t Time;
} a_key_t;

#ifndef _ASKUE_SQLBUFLEN
    #define _ASKUE_SQLBUFLEN 256
#endif

#ifndef _ASKUE_TBUFLEN
    #define _ASKUE_TBUFLEN 256
#endif

typedef struct a_journal_s
{
    size_t Flashback;
    sqlite3 *DB;
    char SQL[ _ASKUE_SQLBUFLEN ];
    char Error[ _ASKUE_TBUFLEN ];
} a_journal_t;

typedef struct a_port_s
{
    int RS232;
    struct termios Termios;
} a_port_t;

#endif /* ASKUE_TYPES_H_ */
