#ifndef _POSIX_SOURCE
    #define _POSIX_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h> /* Стандартные функции Unix */
#include <sys/ioctl.h>
#include <termios.h> /* управление POSIX терминалом */
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>

#include "rs232.h"
#include "uint8_array.h"
#include "port.h"


// обработчик прерывания по сигналу от таймера
static 
void alarm_handler(int signo) { ; }

// перевод милисекунд
static 
struct timeval msec_to_timeval ( long int _msec_timeout )
{
	ldiv_t raw = ldiv ( _msec_timeout, 1000 );
	return ( struct timeval ) { .tv_sec = raw.quot, .tv_usec = raw.rem * 1000 };
}

// запуск таймера
static 
int start_reading_timer ( struct timeval TVal )
{
	//назначить обработчик прерывания таймера
	if( signal(SIGALRM, alarm_handler) == SIG_ERR )
	{
		return -1;
	}
	else
	{
		struct itimerval iTVal;
		iTVal.it_interval.tv_sec = 0; //нет периода повторения
		iTVal.it_interval.tv_usec = 0;
		iTVal.it_value = TVal; //значение для отсчёта

        return ( setitimer(ITIMER_REAL, &iTVal, NULL) < 0 ) ? -1 : 1;
	}
}

// остановка таймера
static 
int stop_reading_timer ( void )
{
	// назначить обработчик прерывания таймера
    struct itimerval iTVal;
    // нет периода повторения
    iTVal.it_interval.tv_sec = 0; 
    iTVal.it_interval.tv_usec = 0;
    // значение для отсчёта
    iTVal.it_value.tv_sec = 0; 
    iTVal.it_value.tv_usec = 0; 

    return ( setitimer(ITIMER_REAL, &iTVal, NULL) < 0 ) ? -1 : 1;
}

// таймер "сгорел"
static
int is_fired ( struct itimerval *TVal )
{
    return (TVal->it_value.tv_sec == 0) && (TVal->it_value.tv_usec == 0);
}

// ошибка чтения
static 
int is_read_error ( int Ret )
{
    return ( Ret < 0 ) && ( errno != 0 );
}

// проверка сгорания таймера
static 
int is_fired_reading_timer( void )
{
	struct itimerval TVal;
    return ( !getitimer(ITIMER_REAL, &TVal) ) ? is_fired ( &TVal ) : 0;
}

// закрыть порт
int port_destroy ( askue_port_t *Port )
{
    return close ( Port->RS232 );
}

// настроить порт
int port_init ( askue_port_t *Port, const char *file, const char *speed, const char *dbits, const char *sbits, const char *parity )
{
    int RS232 = rs232_open ( file );
    if ( rs232_init ( RS232, &( Port->Termios ) ) )
    {
        rs232_close ( RS232 );
        return -1;
    }
    
    rs232_set_databits ( &( Port->Termios ), dbits );
    rs232_set_stopbits ( &( Port->Termios ), sbits );
    rs232_set_parity ( &( Port->Termios ), parity );
    rs232_set_speed ( &( Port->Termios ), speed );
    
    if ( rs232_apply ( RS232, &( Port->Termios ) ) )
    {
        rs232_close ( RS232 );
        return -1;
    }
    return 0;
}

static
int __port_read ( const askue_port_t *Port, uint8_array_t *u8a )
{
    int Amount = 0;
    ioctl ( Port->RS232, FIONREAD, &Amount );
    if( Amount > 0 ) //есть доступные символы
    {
        char Buffer[ Amount ];
        int RetVal;
        if ( ( RetVal = read ( Port->RS232, Buffer, ( size_t ) Amount ) ) <= 0 )
        {
            return RetVal;
        }
        else
        {
            uint8_array_append ( u8a, Buffer, Amount );
            return Amount;
        }
    }
    else
    {
        return -1;
    }
}

// читать из порта
int port_read ( const askue_port_t *Port, uint8_array_t *u8a, long int timeout, size_t Amount )
{
    if ( start_reading_timer ( msec_to_timeval ( timeout ) ) )
    {
        int Ret;
        
        if ( Amount > 0 ) // есть слежение по кол-ву принятых байт
            do {
                Ret = __port_read ( Port, u8a );
            } while ( !is_read_error ( Ret ) &&
                       !is_fired_reading_timer () &&
                       u8a->Size < Amount ); 
        else // нет слежения по кол-ву принятых байт
            do {
                Ret = __port_read ( Port, u8a );
            } while ( !is_read_error ( Ret ) &&
                       !is_fired_reading_timer () );
                        
        stop_reading_timer ();       
          
        return Ret;
    }
    else
    {
        return 0;
    }
}

// писать в порт
int port_write ( const askue_port_t *Port, const uint8_array_t *u8a )
{
    return write ( Port->RS232, u8a->Item, u8a->Size );
}
