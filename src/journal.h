#ifndef LIBASKUE_JOURNAL_H_
#define LIBASKUE_JOURNAL_H_

#include "types.h"

// вставка значения
int journal_insert ( a_journal_t *Journal, const a_record_t *Rcd );

// проверка наличия значения
int journal_check ( int *Verdict, a_journal_t *Journal, const a_key_t *Key );

#endif /* LIBASKUE_JOURNAL_H_ */
