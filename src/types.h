#ifndef ASKUE_TYPES_H_
#define ASKUE_TYPES_H_

#include "macro.h"

typedef struct a_value_s
{
    double Base;    // основание степени
    int Exp;        // показатель степени
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

#endif /* ASKUE_TYPES_H_ */
