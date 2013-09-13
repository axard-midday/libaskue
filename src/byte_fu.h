#ifndef ASKUE_BYTE_FU_H_
#define ASKUE_BYTE_FU_H_

#include <stdint.h>

// Отрицательные значения параметра id позволяют оперировать с байтами
// с конца области памяти

// установить байт в области памяти
int set_byte ( void *mem, int memsize, int id, uint8_t data );

// получить байт из области памяти
int get_byte ( void *mem, int memsize, int id, uint8_t *data );

#endif /* ASKUE_BYTE_FU_H_ */
