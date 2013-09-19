#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my.h"

static
size_t get_words_amount ( const char *str )
{
    const char *Next = str;
    size_t Amount = 1;
    
    while ( ( Next = strchr ( Next, ' ' ) ) != NULL )
    {
        Amount++;
        Next++;
    }
    
    return Amount;
}

char** str2word ( const char *str )
{
    size_t amount = get_words_amount ( str );
    
    char **RetVal = ( char** ) malloc ( sizeof ( char* ) * ( amount + 1 ) );
    if ( RetVal == NULL ) return NULL;
    RetVal[ amount ] = NULL;
    
    const char *Next = str;
    const char *Start = str;
    const char *End = strlen ( str ) + str;
    size_t I = 0;
    
    while ( ( Next = strchr ( Next, ' ' ) ) != NULL )
    {
        RetVal[ I ] = mystrndup ( Start, Next - Start );
        I++;
        Next++;
        Start = Next;
    }
    
    RetVal[ I ] = mystrndup ( Start, End - Start );
    
    return RetVal;
}

