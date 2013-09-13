#ifndef ASKUE_TIME_FUNCTIONS_H_
#define ASKUE_TIME_FUNCTIONS_H_

#include <time.h>
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
struct tm add_second ( const struct tm *Time, int N );

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
struct tm add_minute ( const struct tm *Time, int N );

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
struct tm add_hour ( const struct tm *Time, int N );

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
struct tm add_day ( const struct tm *Time, int N );

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
int add_wday ( int wday, int N );

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
struct tm add_month ( const struct tm *Time, int N );

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
struct tm add_year ( const struct tm *Time, int N );

/*
 * 
 * name: _count_mdays
 * @param
 *  int - месяц в формате 1 .. 12
 *  int - год
 * @return
 *  int - число дней
 */
int count_mday ( int Mon, int Year );

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
struct tm get_current_time ( void );

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
int is_leap_year ( int Year );

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
struct tm sub_second ( const struct tm *Time, int N );

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
struct tm sub_minute ( const struct tm *Time, int N );

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
struct tm sub_hour ( const struct tm *Time, int N );

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
struct tm sub_day ( const struct tm *Time, int N );

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
struct tm sub_month ( const struct tm *Time, int N );

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
struct tm sub_year ( const struct tm *Time, int N );

#endif /* ASKUE_TIME_FUNCTIONS_H_ */
