#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "types.h"
#include "macro.h"

// Journal API:
// write

// запись ошибки sqlite3_exec в буфер
static
int get_error_msg ( char *Buffer, const char *EStr )
{
    if ( snprintf ( Buffer, _ASKUE_TBUFLEN, "Ошибка sqlite3: %s.", EStr ) <=0 )
        return ASKUE_ERROR;
    else
        return ASKUE_SUCCESS;
}

// поймать ошибку от sqlite3_exec
static
int catch_error ( a_journal_t *Journal, int ECode, const char *EStr )
{
    int RetCode;
    if ( ECode == SQLITE_CONSTRAINT )
    {
        RetCode = get_error_msg ( Journal->Error, EStr );
    }
    else if ( ECode == SQLITE_BUSY )
    {
        RetCode = get_error_msg ( Journal->Error, EStr );
        if ( RetCode == ASKUE_SUCCESS ) RetCode = ASKUE_SUCCESS + 1;   
    }
    else if ( ECode != SQLITE_OK )
    {
        ( void ) get_error_msg ( Journal->Error, EStr );
        RetCode = ASKUE_ERROR;
    }
    else
    {
        RetCode = ASKUE_SUCCESS;
    }
    
    return RetCode;
}

// обработчик результатов запроса
typedef int ( *exec_callback_t ) ( void*, int, char**, char** );

// выполнить запрос
static 
int journal_sqlite3_exec ( a_journal_t *Journal, exec_callback_t callback, void *Ptr )
{
    // результат выполнения
    int Result;
    int Times = 5;
    // отслеживание занятости базы
    do {
        // описание ошибки
        char *EStr = NULL;
        // выполнение запроса
        int ECode = sqlite3_exec ( Journal->DB, Journal->SQL, callback, Ptr, &EStr );
        // ловля ошибок
        Result = catch_error ( Journal, ECode, EStr );
        // освобождение памяти
        sqlite3_free ( EStr );
        // следующий раз
        Times--;
    } while ( ( Result == ( ASKUE_SUCCESS + 1 ) ) && ( Times > 0 ) );
    // результат
    return Result;
}


// шаблон запроса
#define Tmp "INSERT INTO reg_tbl ( Device, BValue, EValue, Type, Date, Time ) VALUES ( %llu, %d, %d, '%s', '%s', '%s' );"
// вставка значения
int journal_insert ( a_journal_t *Journal, const a_record_t *Rcd )
{
    // оформить запрос
    if ( snprintf ( Journal->SQL, _ASKUE_SQLBUFLEN, Tmp, 
                    Rcd->Device, Rcd->Value.Base, Rcd->Value.Exp, Rcd->Type.Value, Rcd->Date.Value, Rcd->Time.Value ) <= 0 )
    {
        return ASKUE_ERROR;
    }
    else
    {
        return journal_sqlite3_exec ( Journal, NULL, NULL );
    }
}
#undef Tmp

static
int check_callback ( void *Ptr, int Amount, char **vals, char **cols )
{
    #define Flag vals[ 0 ]
    
    char *Next;
    *( int* ) Ptr = ( int ) strtol ( Flag, &Next, 10 );
    return Next == Flag;
    
    #undef Flag
}

// шаблон запроса
#define Tmp "SELECT EXISTS( SELECT * FROM reg_tbl WHERE device=%llu AND type='%s' AND date='%s' AND time='%s' );"
// проверка наличия значения
int journal_check ( int *Verdict, a_journal_t *Journal, const a_key_t *Key )
{
    // оформить запрос
    if ( snprintf ( Journal->SQL, _ASKUE_SQLBUFLEN, Tmp, Key->Device, Key->Type.Value, Key->Date.Value, Key->Time.Value ) <= 0 )
    {
        return ASKUE_ERROR;
    }
    else
    {
        return journal_sqlite3_exec ( Journal, check_callback, Verdict );
    }
}
#undef Tmp












