#ifndef LIBASKUE_INIT_H_
#define LIBASKUE_INIT_H_

#include <stdio.h>
#include "types.h"

// функции настройки из аргументов командной строки
// настройка порта
int port_init_by_cli ( a_port_t *Port, char **Error, int argc, char **argv );
// устройства
int device_init_by_cli ( const char **Device, long int *Timeout, char **Error, int argc, char **argv );
// параметров
int parametr_init_by_cli ( const char **Parametr, const char **Arguments, char **Error, int argc, char **argv );
// журнала
int journal_init_by_cli ( a_journal_t *Journal, char **Error, int argc, char **argv );
// лога
int log_init_by_cli ( FILE **Log, char **Error, int argc, char **argv );
// флагов
int flag_init_by_cli ( int *Protocol, int *Verbose, char **Error, int argc, char **argv );


#endif /* LIBASKUE_INIT_H_ */
