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

#include "rs232.h"
#include "uint8_array.h"
#include "port.h"


// открыть порт
static
int __port_open ( askue_port_t *Port )
{
    Port->In = fdopen ( Port->RS232, "r" );
    if ( Port->In == NULL )
    {
        return -1;
    }
    Port->Out = fdopen ( Port->RS232, "w" );
    if ( Port->Out == NULL )
    {
        fclose ( Port->In );
        return -1;
    }
    return 0;
}

// закрыть порт
static
int __port_close ( askue_port_t *Port )
{
    if ( fclose ( Port->In ) )
        return -1;
    if ( fclose ( Port->Out ) )
        return -1;
    
    return 0;
}

// закрыть порт
int port_destroy ( askue_port_t *Port )
{
    if ( !__port_close ( Port ) )
    {
        close ( Port->RS232 );
        return 0;
    }
    else
        return -1;
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
    
    Port->RS232 = RS232;
    if ( __port_open ( Port ) )
    {
        close ( Port->RS232 );
        return -1;
    }
    else
    {
        return 0;
    }
}

static
int __port_read ( const askue_port_t *Port, uint8_array_t *u8a )
{
    int Amount = 0;
    ioctl ( Port->RS232, FIONREAD, &Amount );
    if( Amount > 0 ) //есть доступные символы
    {
        char Buffer[ Amount ];
        if ( fread ( Buffer, sizeof ( char ), ( size_t ) Amount, Port->In ) == 0 )
        {
            if ( feof ( Port->In ) ||
                 ferror ( Port->In ) )
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            uint8_array_update ( u8a, Buffer, Amount );
            return Amount;
        }
    }
    else
    {
        return -1;
    }
}

// читать из порта
int port_read ( const askue_port_t *Port, uint8_array_t *u8a, long int timeout )
{
    ldiv_t Sec = ldiv ( timeout, 1000 );
    struct timeval Timeout = { Sec.quot, Sec.rem * 1000 };
    
    fd_set ReadSet;
    FD_ZERO ( &ReadSet );
    FD_SET ( Port->RS232, &ReadSet );

    int RetVal = select( Port->RS232 + 1, &ReadSet, NULL, NULL, &Timeout);
    
    if ( RetVal == 0 )
    {
        return 0;
    }
    else if ( RetVal == -1 )
    {
        return -1;
    }
    else
    {
        assert ( FD_ISSET ( Port->RS232, &ReadSet ) );
        return __port_read ( Port, u8a );
    }
}

// писать в порт
int port_write ( const askue_port_t *Port, const uint8_array_t *u8a )
{
    size_t wb = fwrite ( u8a->Item, sizeof ( uint8_t ), u8a->Size, Port->Out );
    if ( wb == 0 )
    {
        if ( ferror ( Port->Out ) )
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else 
    {
        return wb;
    }
}
