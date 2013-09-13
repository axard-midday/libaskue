/*
 * uint8_array.h
 *
 *  Created on: 05.12.2012
 *      Author: andrey
 */

#ifndef ASKUE_UINT8_ARRAY_H_
#define ASKUE_UINT8_ARRAY_H_

#include <stdint.h>

/*
 * Массив байт
 */
typedef struct _uint8_array_t
{
	size_t Size;
	uint8_t *Item;
} uint8_array_t;

/*
 * Создать массив
 */
void uint8_array_init ( uint8_array_t* u8a, size_t size );

/*
 * Удалить массив
 */
void uint8_array_destroy ( uint8_array_t* ptr );

/*
 * Изменить размер массива
 */
void uint8_array_resize ( uint8_array_t* ptr, size_t size );

/*
 * Добавить в конец данные
 */
void uint8_array_append ( uint8_array_t* dest, const uint8_t *data, size_t data_size );

/*
 * Перезаписать массив данными
 */
void uint8_array_update ( uint8_array_t *dest, const uint8_t *data, size_t data_size );

/*
 * Извлечь подмассив
 */
void uint8_array_extract ( uint8_array_t *dest, const uint8_array_t *src, size_t start, size_t stop );



#endif /* ASKUE_UINT8_ARRAY_H_ */
