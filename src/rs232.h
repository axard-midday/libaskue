/*
 * rs232_port.h
 *
 *  Created on: 05.12.2012
 *      Author: andrey
 */

#ifndef ASKUE_RS232_PORT_H_
#define ASKUE_RS232_PORT_H_


/*
 * Общие библиотеки
 */

#include <unistd.h> /* Стандартные функции Unix */
#include <sys/ioctl.h>
#include <fcntl.h> /* Функции контроля файлового доступа */
#include <termios.h> /* управление POSIX терминалом */
#include <signal.h>/*для обработки сигналов таймера*/
#include <sys/time.h>

/*
 * Местные библиотеки
 */

#include "uint8_array.h"
#include "macro.h"

/*
 * Основные функции
 */

/*
 * Открыть порт 
 */
int rs232_open(const char* rs232_file_name);

/*
 * Закрыть порт
 */
void rs232_close( int rs232_fd );

/*
 * установить для структуры настроек "сырой" режим
 */
int rs232_init( int rs232_fd, struct termios *T );

/*
 * установить скорость
 */
int rs232_set_speed( struct termios *T, const char *speed );

/*
 * установить чётность
 */
void rs232_set_parity ( struct termios *T, const char *parity );

/*
 * Установить кол-во стоп битов
 */
void rs232_set_stopbits ( struct termios *T, const char *stopbits );

/*
 * установить кол-во битов данных
 */
void rs232_set_databits ( struct termios *T, const char *databits );

/*
 * применить настройки
 */
int rs232_apply( int rs232_fd, struct termios *T ); 

#endif /* RS232_PORT_H_ */
