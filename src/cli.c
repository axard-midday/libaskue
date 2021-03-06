#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>

#include "cli.h"

// тип опции
static
cli_arg_type_t get_argtype ( const char *arg )
{
    if ( arg[ 0 ] == '-' )
    {
        if ( isalpha ( arg[ 1 ] ) &&
              ( arg[ 2 ] == '\0' ) )
        {
            return CLI_SHORT_ARG;
        }
        else if ( ( arg[ 1 ] == '-' ) && 
                   ( arg[ 2 ] != '\0' ) )
        {
            return CLI_LONG_ARG;
        }
        else
        {
            return CLI_UNKNOW_ARG;
        }
    }
    else
    {
        return CLI_UNKNOW_ARG;
    }
}

// проверка что опция последняя
static
int is_last_arg ( const cli_arg_t *Option )
{
    return Option->LongName == NULL &&
            Option->ShortName == 0 &&
            Option->ArgNeed == CLI_NO_ARG &&
            Option->ValNeed == CLI_NO_VAL &&
            Option->Handler == NULL &&
            Option->Var == NULL;
}

// проверка опции
int cmp_shortname ( cli_arg_t *Arg, int Name )
{
    return Arg->ShortName == Name;
}

int cmp_longname ( cli_arg_t *Arg, const char *Name )
{
    int R = 1;
    const char *ArgName = Arg->LongName;
    while ( *ArgName != '\0' && *ArgName != '=' && *Name != '\0' && R )
    {
        R = *ArgName == *Name;
        ArgName ++;
        Name ++;
    }
    return R;
}



static
cli_arg_t* find_arg_by_longname ( cli_arg_t *Args, const char *LongName )
{
    int i = 0;
    cli_arg_t *Result = NULL;
    
    while ( !is_last_arg ( Args + i ) && Result == NULL )
    {
        if ( cmp_longname ( Args + i, LongName ) )
            Result = Args + i;
        
        i++;
    }
    
    return Result;
}

static
cli_arg_t* find_arg_by_shortname ( cli_arg_t *Args, int ShortName )
{
    int i = 0;
    cli_arg_t *Result = NULL;
    
    while ( !is_last_arg ( Args + i ) && Result == NULL )
    {
        if ( cmp_shortname ( Args + i, ShortName ) )
            Result = Args + i;
        
        i++;
    }
    
    return Result;
}

// поиск аргумента
static
cli_arg_t* find_arg ( cli_arg_t *Args, const char *ArgName )
{
    cli_arg_t *Arg;
    // определение длинны аргумента
    switch ( get_argtype ( ArgName ) )
    {
        case CLI_LONG_ARG:
            Arg = find_arg_by_longname ( Args, ArgName + 2 );
            break;
        case CLI_SHORT_ARG:
            Arg = find_arg_by_shortname ( Args, ArgName[ 1 ] );
            break;
        default:
            Arg = NULL;
            break;
    }
    
    return Arg;
}

static
const char* find_val ( const char *argv )
{
    register const char *_r = argv;
    while ( *_r != '=' ) _r++;
    _r++;
    return _r;
}

static
size_t get_need_arg_amount ( const cli_arg_t *Args )
{
    size_t Result = 0;
    
    while ( !is_last_arg ( Args ) )
    {
        if ( Args->ArgNeed == CLI_REQUIRED_ARG )
        {
            Result++;
        }
        
        Args++;
    }
    
    return Result;
}

// найти аргумент
cli_result_t cli_parse ( cli_arg_t *Args, int argc, char** argv )
{
    cli_result_t Result = CLI_SUCCESS;
    // номер аргумента из полученного списка
    int i = 1;
    // кол-во необходимых аргументов
    size_t NeedArgAmount = get_need_arg_amount ( Args );
    while ( ( i < argc ) && ( Result == CLI_SUCCESS ) )
    {
        // найти аргумент
        cli_arg_t *Arg;
        
        switch ( get_argtype ( argv[ i ] ) )
        {
            case CLI_LONG_ARG:
                Arg = find_arg_by_longname ( Args, argv[ i ] + 2 );
                break;
            case CLI_SHORT_ARG:
                Arg = find_arg_by_shortname ( Args, argv[ i ][ 1 ] );
                break;
            default:
                Arg = NULL;
                Result = CLI_ERROR_ARGTYPE; // ошибка определения типа аргумента
                break;
        }
        
        if ( Arg != NULL )
        {
            // аргумент обязателен
            if ( Arg->ArgNeed == CLI_REQUIRED_ARG )
                NeedArgAmount--;
            
            // поиск значения аргумента
            const char *Val;
            if ( Arg->ValNeed != CLI_NO_VAL ) // обязателен или опционален
            {
                Val = find_val ( argv[ i ] );
                if ( Arg->ValNeed == CLI_REQUIRED_VAL && Val == NULL )
                {
                    Result = CLI_ERROR_NOVAL;
                }
                
                Result = ( Arg->Handler ( Arg->Var, Val ) == 0 ) ? CLI_SUCCESS : CLI_ERROR_HANDLER;
            }
            else
            {
                Val = NULL;
                Result = ( Arg->Handler ( Arg->Var, Val ) == 0 ) ? CLI_SUCCESS : CLI_ERROR_HANDLER;
            }
        }
        i++;
    }
    
    return ( !NeedArgAmount ) ? Result : CLI_ERROR_NEEDARG;
}
