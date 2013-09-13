#ifndef ASKUE_BCD_H_
#define ASKUE_BCD_H_

#include <stdint.h>

/*
 * BCD - двоично-десятичный код
 */

/*
 * Перевод BCD в обычный вид
 */
uint32_t bcd_to_dec ( uint32_t bcd );

/*
 * Перевод в BCD из обычного вида
 */
uint32_t dec_to_bcd ( uint32_t dec );

#endif /* ASKUE_BCD_H_ */
