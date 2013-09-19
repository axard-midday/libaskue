#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <errno.h>

#include "cli.h"
#include "port.h"
#include "types.h"
#include "my.h"
#include "macro.h"

// передать строковый аргумент по указателю
static
int __cli_get_str ( void *ptr, const char *arg )
{
    *( const char ** ) ptr = arg;
    return ( *( const char ** ) ptr ) == NULL;
}

static
void catch_error ( char **Str, cli_result_t Result )
{
    switch ( Result )
    {
        case CLI_ERROR_NOVAL: ( *Str ) = mystrdup ( "Нет требуемого за аргументом значения." );
            break;
        case CLI_ERROR_HANDLER: ( *Str ) = mystrdup ( "Ошибка обработчика." );
            break;
        case CLI_ERROR_NEEDARG: ( *Str ) = mystrdup ( "Не достаточно обязательных аргументов." );
            break;
        case CLI_ERROR_ARGTYPE: ( *Str ) = mystrdup ( "Ошибка определения типа аргументов." );
            break;
        default:
            ( *Str ) = NULL;
            break;
    }
}

// найти аргументы порта
static
cli_result_t cli_find_port_args ( const char **File, 
                                    const char **Speed, 
                                    const char **DBits,
                                    const char **SBits,
                                    const char **Parity,
                                    int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {
        { "port_file", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, File },
        { "port_speed", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, Speed },
        { "port_dbits", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, DBits },
        { "port_sbits", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, SBits },
        { "port_parity", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, Parity },
        CLI_LAST_ARG
    };

    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int port_init_by_cli ( a_port_t *Port, char **Error, int argc, char **argv )
{
    const char *File, *Speed, *Parity, *DBits, *SBits;
    cli_result_t cr = cli_find_port_args ( &File, &Speed, &DBits, &SBits, &Parity, argc, argv );
    if ( cr == CLI_SUCCESS )
    {
        if ( port_init ( Port, File, Speed, DBits, SBits, Parity ) == ASKUE_ERROR )
        {
            if ( asprintf ( Error, "Ошибка открытия порта: %s.", strerror ( errno ) ) == -1 )
            {
                ( *Error ) = NULL;
            }
            
            return ASKUE_ERROR;
        }
        else
        {
            ( *Error ) = NULL;
            return ASKUE_SUCCESS;
        }
    }
    else 
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
}

static
int __cli_get_lint ( void *ptr, const char *arg )
{
    char *next;
    // строка в цифру
    *( long int * ) ptr = strtol ( arg, &next, 10 );
    // проверка на ошибки
    return arg == ( const char* ) next;
}

// найти аргументы порта
static
cli_result_t cli_find_device_args ( const char **Device,
                                      long int *Timeout,
                                      int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {   
        { "device", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, Device },
        { "timeout", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_lint, Timeout },
        CLI_LAST_ARG
    };
    
    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int device_init_by_cli ( const char **Device, long int *Timeout, char **Error, int argc, char **argv )
{
    cli_result_t cr = cli_find_device_args ( Device, Timeout, argc, argv );
    if ( cr != CLI_SUCCESS )
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
    else
    {
        ( *Error ) = NULL;
        return ASKUE_SUCCESS;
    }
}

// найти аргумент и параметр передаваемы скрипту
static
cli_result_t cli_find_parametr_args ( const char **Parametr,
                                        const char **Argument,
                                        int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {   
        { "parametr", 0, CLI_OPTIONAL_ARG, CLI_REQUIRED_VAL, __cli_get_str, Parametr },
        { "argument", 0, CLI_OPTIONAL_ARG, CLI_REQUIRED_VAL, __cli_get_str, Argument },
        CLI_LAST_ARG
    };
    
    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int parametr_init_by_cli ( const char **Parametr, const char **Arguments, char **Error, int argc, char **argv )
{
    cli_result_t cr = cli_find_parametr_args ( Parametr, Arguments, argc, argv );
    if ( cr != CLI_SUCCESS )
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
    else
    {
        ( *Error ) = NULL;
        return ASKUE_SUCCESS;
    }
}
// добыть таймаут
static
int __cli_get_size ( void *ptr, const char *arg )
{
    char *next;
    // строка в цифру
    *( size_t* ) ptr = ( size_t ) strtoul ( arg, &next, 10 );
    // проверка на ошибки
    return arg == ( const char* ) next;
}

// найти аргументы журнала
static
cli_result_t cli_find_journal_args ( const char **File,
                                       const char **Flashback,
                                       int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {   
        { "journal_file", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, File },
        { "journal_flashback", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_size, Flashback },
        CLI_LAST_ARG
    };
    
    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int journal_init_by_cli ( a_journal_t *Journal, char **Error, int argc, char **argv )
{
    const char *File, *Flashback;
    cli_result_t cr = cli_find_journal_args ( &File, &Flashback, argc, argv );
    if ( cr == CLI_SUCCESS )
    {
        if ( sqlite3_open ( File, &(Journal->DB) ) != SQLITE_OK )
        {
            if ( asprintf ( Error, "Ошибка открытия журнала: %s.", sqlite3_errmsg ( Journal->DB ) ) == -1 )
            {
                ( *Error ) = NULL;
            }
            
            sqlite3_close ( Journal->DB );
            Journal->DB = NULL;
            
            return ASKUE_ERROR;
        }
        else
        {
            ( *Error ) = NULL;
            return ASKUE_SUCCESS;
        }
    }
    else
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
}

// найти аргументы лога
static
cli_result_t cli_find_log_args ( const char **Log,
                                   int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {   
        { "log_file", 0, CLI_REQUIRED_ARG, CLI_REQUIRED_VAL, __cli_get_str, Log },
        CLI_LAST_ARG
    };
    
    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int log_init_by_cli ( FILE **Log, char **Error, int argc, char **argv )
{
    const char *File;
    cli_result_t cr = cli_find_log_args ( &File, argc, argv );
    if ( cr == CLI_SUCCESS )
    {
        ( *Log ) = fopen ( File, "a" );
        if ( ( *Log ) == NULL )
        {
            if ( asprintf ( Error, "Ошибка открытия лога: %s.", strerror ( errno ) ) == -1 )
            {
                ( *Error ) = NULL;
            }
            
            return ASKUE_ERROR;
        }
        else
        {
            ( *Error ) = NULL;
            return ASKUE_SUCCESS;
        }
    }
    else
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
}

static
int __cli_get_int ( void *ptr, const char *arg )
{
    char *next;
    // строка в цифру
    *( int* ) ptr = 1;
    // проверка на ошибки
    return arg == ( const char* ) next;
}

// найти аргументы флагов
static
cli_result_t cli_find_flag_args ( int *Protocol,
                                    int *Verbose,
                                    int argc, char **argv )
{
    // аргументы
    cli_arg_t Args[] =
    {   
        { "protocol", 0, CLI_OPTIONAL_ARG, CLI_NO_VAL, __cli_get_int, Protocol },
        { "verbose", 0, CLI_OPTIONAL_ARG, CLI_NO_VAL, __cli_get_int, Verbose },
        CLI_LAST_ARG
    };
    
    // произвести разбор опций
    return cli_parse ( Args, argc, argv );
}

int flag_init_by_cli ( int *Protocol, int *Verbose, char **Error, int argc, char **argv )
{
    cli_result_t cr = cli_find_flag_args ( Protocol, Verbose, argc, argv );
    if ( cr != CLI_SUCCESS )
    {
        catch_error ( Error, cr );
        return ASKUE_ERROR;
    }
    else
    {
        ( *Error ) = NULL;
        return ASKUE_SUCCESS;
    }
}










