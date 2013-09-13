
#ifndef ASKUE_MACRO_H
#define ASKUE_MACRO_H


#include <stdint.h>
#include <stddef.h>

/*
 * СТРОКОВЫЕ БУФЕРЫ
 */
// специальные буферы (без учёта конечного символа)
#define TIME_STRBUF                 8
#define DATE_STRBUF                 10

/*
 * БУЛЕВСКИЕ КОНСТАНТЫ
 */
#define TRUE 1
#define FALSE 0

/*
 * Константы флагов завершения
 */
#ifndef ASKUE_SUCCESS
    #define ASKUE_SUCCESS 0
#endif

#ifndef ASKUE_ERROR
    #define ASKUE_ERROR -1
#endif

 
/*
 * ПРОВЕРКА БИТОВ
 */
// установить бит
#define SETBIT(bitfield, bitindex) \
	( ( bitfield ) |= 1 << ( bitindex ) )

// снять бит
#define UNSETBIT(bitfield, bitindex) \
	( ( bitfield ) &= ~( 1 << ( bitindex ) ) )

// переключить бит
#define TOGGLEBIT(bitfield, bitindex) \
	( ( bitfield ) ^= 1 << ( bitindex ) )

// выделить бит вернёт 1 если установлен
#define TESTBIT(bitfield, bitindex) \
	( !!( ( bitfield ) & ( 1 << ( bitindex ) ) ) )

#endif /* ASKUE_MACRO_H */
