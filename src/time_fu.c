/*
 * time.c
 * 
 * Copyright 2013 axard <axard@axard-desktop>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#ifndef _POSIX_SOURCE
    #define _POSIX_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * 
 * name: _is_leap_year
 * @param
 *  int year - год для проверки
 * @return
 *  int ( 1/0 ) - булевский результат
 * 
 * Проверяет является ли год високосным
 */ 
int is_leap_year ( int x )
{
    return ( !( x % 4 ) && ( x % 100 ) ) || !( x % 400 );
}

/*                      Функции вычитания                             */
/*
 * 
 * name: _sub_year
 * @param
 *  const struct tm * - уменьшаемое ( год )
 *  int - вычитаемое
 * @return
 *  struct tm
 * 
 * Уменьшить год на N
 */
struct tm sub_year ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    Result.tm_year -= N;
    return Result;
}

/*
 * 
 * name: _count_mdays
 * @param
 *  int - месяц в формате 1 .. 12
 *  int - год
 * @return
 *  int - число дней
 */
int count_mday ( int Mon, int Year )
{
    return ( Mon == 1 ) ? 31 :
            ( Mon == 2 ) ? ( ( is_leap_year ( Year ) ) ? 29 : 28 ) :
            ( Mon == 3 ) ? 31 :
            ( Mon == 4 ) ? 30 :
            ( Mon == 5 ) ? 31 :
            ( Mon == 6 ) ? 30 :
            ( Mon == 7 ) ? 31 :
            ( Mon == 8 ) ? 31 :
            ( Mon == 9 ) ? 30 :
            ( Mon == 10 ) ? 31 :
            ( Mon == 11 ) ? 30 :
            ( Mon == 12 ) ? 31 : 0;
}



/*
 * 
 * name: _sub_month
 * @param
 *  const struct tm * - уменьшаемое ( месяц )
 *  int - вычитаемое
 * @return
 *  struct tm
 * 
 * Уменьшить месяц на N. Если значение месяца стало < 0,
 * то уменьшить год
 */
struct tm sub_month ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 12 );
    if ( Div.quot )
    {
        Result = sub_year ( &Result, Div.quot );
    }
    
    if ( Result.tm_mon <= Div.rem )
    {
        Result = sub_year ( &Result, 1 );
        Result.tm_mon = 12 - ( Div.rem - Result.tm_mon );
    }
    else
    {
        Result.tm_mon = Result.tm_mon - Div.rem;
    }
    return Result;
}

/*
 * 
 * name: _count_wday_prev
 * @param
 *  int - день недели
 *  int - число дней назад
 * @return
 *  int - день недели
 * 
 * посчитать номер дня недели по смещению назад
 *  1 ... 7, где
 *    1 - Пн
 *    7 - Вс 
 */
int sub_wday ( int wday, int N )
{
    div_t D = div ( N, 7 );
    
    int Result = wday;
    
    if ( Result > 0 ) // если будни или суббота
    {
        Result -= D.rem;
    }
    else // если воскресенье
    {
        Result = 7 - D.rem;
    }
    
    return Result;
}


/*
 * 
 * name: _sub_day
 * @param
 *  const struct tm * - уменьшаемое ( месяц )
 *  int - вычитаемое
 * @return
 *  struct tm
 * 
 * Уменьшить день на N. Если значение дней стало < 0,
 * то уменьшить вышестояшие домены времени
 */
 
static
struct tm __sub_day ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    if ( Result.tm_mday <= N )
    {
        N -= Result.tm_mday;
        Result.tm_yday -= Result.tm_mday;
        Result = sub_month ( &Result, 1 );
        Result.tm_mday = count_mday ( Result.tm_mon + 1, Result.tm_year );
        Result = __sub_day ( &Result, N );
    }
    else
    {
        Result.tm_mday -= N;
        Result.tm_yday -= N;
    }
    
    return Result;
}
 
struct tm sub_day ( const struct tm *Time, int N )
{
    struct tm Result = __sub_day ( Time, N );
    Result.tm_wday = ( sub_wday ( Time->tm_wday, N ) ) ?: 0;
    return Result;
}

/*
 * 
 * name: sub_hour
 * @param
 *  const struct tm * - уменьшаемое
 *  int - вычитаемое
 * @return
 *  struct tm
 * Уменьшить часы на N
 */
struct tm sub_hour ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 24 );
    if ( Div.quot )
    {
        Result = sub_day ( &Result, Div.quot );
    }
    
    if ( Result.tm_hour < Div.rem )
    {
        Result = sub_day ( &Result, 1 );
        Result.tm_hour = 24 - ( Div.rem - Result.tm_hour );
    }
    else
    {
        Result.tm_hour = Result.tm_hour - Div.rem;
    }
    
    return Result;
}

/*
 * 
 * name: _sub_minute
 * @param
 *  const struct tm * - уменьшаемое
 *  int - вычитаемое
 * @return
 *  struct tm
 * Уменьшить минуты на N
 */
struct tm sub_minute ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 60 );
    if ( Div.quot )
    {
        Result = sub_hour ( &Result, Div.quot );
    }
    
    if ( Result.tm_min < Div.rem )
    {
        Result = sub_hour ( &Result, 1 );
        Result.tm_min = 60 - ( Div.rem - Result.tm_min );
    }
    else
    {
        Result.tm_min = Result.tm_min - Div.rem;
    }
    
    return Result;
}

/*
 * 
 * name: _sub_second
 * @param
 *  const struct tm * - уменьшаемое
 *  int - вычитаемое
 * @return
 *  struct tm
 * Уменьшить секунды на N
 */
struct tm sub_second ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 60 );
    if ( Div.quot )
    {
        Result = sub_minute ( &Result, Div.quot );
    }
    
    if ( Result.tm_sec < Div.rem )
    {
        Result = sub_minute ( &Result, 1 );
        Result.tm_sec = 60 - ( Div.rem - Result.tm_sec );
    }
    else
    {
        Result.tm_sec = Result.tm_min - Div.rem;
    }
    
    return Result;
}

/*                      Функции сложения                              */
/*
 * 
 * name: _add_year
 * @param
 *  const struct tm * - первое слагаемое ( год )
 *  int - второе слагаемое
 * @return
 *  struct tm
 * 
 * Увеличить год на N
 */
struct tm add_year ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    Result.tm_year += N;
    return Result;
}

/*
 * 
 * name: _add_month
 * @param
 *  const struct tm * - первое слагаемое ( месяц )
 *  int - второе слагаемое
 * @return
 *  struct tm
 * 
 * Увеличить месяц на N. Если значение месяца стало > 12,
 * то увеличить год
 */
struct tm add_month ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 12 );
    if ( Div.quot )
    {
        Result = add_year ( &Result, Div.quot );
    }
    
    if ( ( 11 - Result.tm_mon ) < Div.rem )
    {
        Result = add_year ( &Result, 1 );
        Result.tm_mon = Div.rem - ( 12 - Result.tm_mon );
    }
    else
    {
        Result.tm_mon = Result.tm_mon + Div.rem;
    }
    return Result;
}

/*
 * 
 * name: _add_wday
 * @param
 *  int - день недели
 *  int - плюсовое смещение по дням
 * @return
 *  int - номер дня недели
 * 
 * посчитать номер дня недели по смещению назад
 *  1 ... 7, где
 *    1 - Пн
 *    7 - Вс 
 */
int add_wday ( int wday, int N )
{
    div_t D = div ( N, 7 );
    
    int Result = wday + D.rem;
    
    Result = ( Result > 7 ) ? Result - 7 : Result;
    
    return Result;
}

/*
 * 
 * name: _add_day
 * @param
 *  const struct tm * - первое слагаемое ( месяц )
 *  int - второе слагаемое
 * @return
 *  struct tm
 * 
 * Увеличить день на N
 */
 
static
struct tm __add_day ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    if ( ( count_mday ( Result.tm_mon + 1, Result.tm_year ) - Result.tm_mday ) < N )
    {
        N -= ( count_mday ( Result.tm_mon + 1, Result.tm_year ) - Result.tm_mday );
        Result.tm_yday += count_mday ( Result.tm_mon + 1, Result.tm_year ) - Result.tm_mday;
        Result.tm_mday = 0;
        Result = add_month ( &Result, 1 );
        Result = __add_day ( &Result, N );
    }
    else
    {
        Result.tm_mday += N;
        Result.tm_yday += N;
    }
    
    return Result;
}
 
struct tm add_day ( const struct tm *Time, int N )
{
    struct tm Result = __add_day ( Time, N );
    Result.tm_wday = ( add_wday ( Time->tm_wday, N ) ) ?: 0;
    return Result;
}



/*
 * 
 * name: _add_hour
 * @param
 *  const struct tm * - первое слагаемое
 *  int - второе слагаемое
 * @return
 *  struct tm
 * Увеличить часы на N
 */
struct tm add_hour ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 24 );
    if ( Div.quot )
    {
        Result = add_day ( &Result, Div.quot );
    }
    
    if ( ( 24 - Result.tm_hour ) < Div.rem )
    {
        Result = add_day ( &Result, 1 );
        Result.tm_hour = Div.rem - ( 24 - Result.tm_hour );
    }
    else
    {
        Result.tm_hour = Result.tm_hour + Div.rem;
    }
    
    return Result;
}


/*
 * 
 * name: _add_minute
 * @param
 *  const struct tm * - первое слагаемое
 *  int - второе слагаемое
 * @return
 *  struct tm
 * Увеличить минуты на N
 */
struct tm add_minute ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 60 );
    if ( Div.quot )
    {
        Result = add_hour ( &Result, Div.quot );
    }
    
    if ( ( 60 - Result.tm_min ) < Div.rem )
    {
        Result = add_hour ( &Result, 1 );
        Result.tm_min = Div.rem - ( 60 - Result.tm_min );
    }
    else
    {
        Result.tm_min = Result.tm_min + Div.rem;
    }
    
    return Result;
}

/*
 * 
 * name: _add_second
 * @param
 *  const struct tm * - первое слагаемое
 *  int - второе слагаемое
 * @return
 *  struct tm
 * Увеличить секунды на N
 */
struct tm add_second ( const struct tm *Time, int N )
{
    struct tm Result = ( struct tm ) ( *Time );
    
    // если вычитаемые месяцы больше года
    div_t Div = div ( N, 60 );
    if ( Div.quot )
    {
        Result = add_minute ( &Result, Div.quot );
    }
    
    if ( ( 60 - Result.tm_sec ) < Div.rem )
    {
        Result = add_minute ( &Result, 1 );
        Result.tm_sec = Div.rem - ( 60 - Result.tm_sec );
    }
    else
    {
        Result.tm_sec = Result.tm_sec + Div.rem;
    }
    
    return Result;
}

/*
 * 
 * name: get_current_time
 * @param
 *  не принимает параметров
 * @return
 *  struct tm - текущее время
 * 
 * Возвращает текущее время в удобоваримом виде
 */
struct tm get_current_time ( void )
{
    time_t t = time ( NULL );
    struct tm Time;
    localtime_r ( &t, &Time );
    return Time;
}










