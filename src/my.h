#ifndef ASKUE_MY_H_
#define ASKUE_MY_H_

#include <stdlib.h>

// обёртка free()
void myfree ( void *ptr );

// обёртка malloc()
void* mymalloc ( size_t size );

// обёртка strdup ()
char* mystrdup ( const char *src );

// обёртка strndup()
char* mystrndup ( const char *src, size_t n );

// обёртка realloc()
void* myrealloc ( void *ptr, size_t size );

#endif /* ASKUE_MY_H_ */
