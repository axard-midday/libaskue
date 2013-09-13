#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>

#include "cli.h"

// выделить длинную опцию
static
size_t cli_get_long_option_len ( const char *argv )
{
    size_t len = strlen ( argv ) - 1;
    while ( len > 0 && argv[ len ] != '=' )
    {
        len--;
    }
    len -= 2; // отсечь знак "--"
    
    return len;
}

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

// получить аргумент для короткой опции
static
const char* get_short_opt_arg ( int *index, int argc, char **argv )
{
    int _i = *index;
    if ( ( _i + 1 ) < argc &&
         get_argtype ( argv[ _i + 1 ] ) == CLI_UNKNOW_ARG )
    {
        *index += 2;
        return argv[ _i + 1 ];
    }
    else
    {
        *index += 1;
        return NULL;
    }
}

// получить аргумент для длинной опции
static
const char* get_long_opt_arg ( const char *argv )
{
    const char *argument = strchr ( argv, '=' );
    
    if ( argument != NULL )
    {
        return argument + 1;
    }
    else
    {
        return argument;
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
        if ( Args->ValNeed == CLI_REQUIRED_VAL )
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
        cli_arg_t *Arg = find_arg ( Args, argv[ i ] );
        if ( Arg == NULL ) // ошибка поиска аргумента
        {
            Result = CLI_ERROR_ARGTYPE;
        }
        else
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
