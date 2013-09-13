/*
 * rs232_port.c
 *
 *  Created on: 05.12.2012
 *      Author: andrey
 */



//#include <glib.h>

#include <assert.h>
#include <unistd.h> /* Стандартные функции Unix */
#include <sys/ioctl.h>
#include <fcntl.h> /* Функции контроля файлового доступа */
#include <termios.h> /* управление POSIX терминалом */
#include <signal.h>/*для обработки сигналов таймера*/
#include <sys/time.h>
#include <stdio.h>
#include <sys/select.h>

#include <errno.h>
#include <string.h>

#include "rs232.h"

#define speed_str_array rs232_speed
static
speed_t speed_array[] =
{
	B0,
	B50,
	B75,
	B110,
	B134,
	B150,
	B200,
	B300,
	B600,
	B1200,
	B1800,
	B2400,
	B4800,
	B9600,
	B19200,
	B38400,
	B57600,
	B115200,
	B230400,
};


void rs232_close( int rs232_fd )
{
	close ( rs232_fd );
}

int rs232_open(const char* rs232_file_name)
{
	return open ( rs232_file_name, O_RDWR | O_NOCTTY | O_NONBLOCK );
}

int rs232_init( int rs232_fd, struct termios *T )
{
	if ( isatty ( rs232_fd ) ) //проверка соответствия поданного дескриптора устройству tty
	{
		//получить старые настройки com-порта
		if( !tcgetattr ( rs232_fd, T ) )
		{
			T -> c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

			T -> c_oflag = 0;

			T -> c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

			T -> c_cflag &= ~(CSIZE | CSTOPB );

			T -> c_cflag |= CS8 | CREAD;

			T -> c_cflag &= ~PARENB;

			for ( int i = 0; i < NCCS; i++ )
				T -> c_cc[ i ] = 0;

			T -> c_cc[ VMIN ]  = 1;
			T -> c_cc[ VTIME ] = 1;

			return 0;
		}
	}

	return -1;
}

int rs232_set_speed( struct termios *T, const char *speed )
{
	int index = 0;
	while ( speed_str_array[ index ] != NULL )
	{
		if ( !strcmp ( speed, speed_str_array[ index ] ) )
		{
			break;
		}
		else
			index++;
	}
	 // установка скорости ввода/вывода
	if ( ( cfsetispeed ( T, speed_array[ index ] ) < 0 ) ||
		 ( cfsetospeed ( T, speed_array[ index ] ) < 0 ) )
		return -1;
	else
		return 0;

}

void rs232_set_parity ( struct termios *T, const char *parity )
{
	if ( !strcmp ( "no", parity ) )
	{
		T -> c_iflag &= ~INPCK; //непроверять чётность на входе
		T -> c_cflag &= ~PARENB; //не генерировать бит чётности
	}
	else if ( !strcmp ( "odd", parity ) )
	{
		T -> c_iflag |= INPCK; //включить проверку чётности на входе
		T -> c_cflag |= PARENB; //генерить проверку чётности
		T -> c_cflag |= PARODD; //переключить на нечётность
	}
	else if ( !strcmp ( "even", parity ) )
	{
		T -> c_iflag |= INPCK; //включить проверку чётности на входе
		T -> c_cflag |= PARENB; //генерить проверку чётности
	}

}

void rs232_set_stopbits ( struct termios *T, const char *stopbits )
{
	if ( !strcmp ( "1", stopbits ) )
	{
		T -> c_cflag &= ~( CSTOPB ); //один стоп бит
	}
	else if ( !strcmp ( "2", stopbits ) )
	{
		T -> c_cflag |= CSTOPB; //два стоп бита
	}

}

void rs232_set_databits ( struct termios *T, const char *databits )
{
	switch ( databits[0] )
	{
		case '8':
			T -> c_cflag &= ~( CSIZE );
			T -> c_cflag |= CS8;
			break;
		case '7':
			T -> c_cflag &= ~( CSIZE );
			T -> c_cflag |= CS7;
			break;
		case '6':
			T -> c_cflag &= ~( CSIZE );
			T -> c_cflag |= CS6;
			break;
		case '5':
			T -> c_cflag &= ~( CSIZE );
			T -> c_cflag |= CS5;
			break;
		default:
			break;
	}
}

int rs232_apply( int rs232_fd, struct termios *T )
{
	return tcsetattr ( rs232_fd, TCSAFLUSH, T );
}

#undef speed_str_array

