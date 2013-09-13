#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int set_byte ( void *mem, int memsize, int id, uint8_t data )
{
    if ( memsize < 0 ) return -1;
    uint8_t *_mem = ( uint8_t* ) mem;
    if ( id >= memsize ) return -1;
    if ( id >= 0 )
        _mem[ id ] = data;
    else 
        _mem[ memsize + id ] = data;
    return 0;
}

int get_byte ( void *mem, int memsize, int id, uint8_t *data )
{
    if ( memsize < 0 ) return -1;
    uint8_t *_mem = ( uint8_t* ) mem;
    if ( id >= memsize ) return -1;
    if ( id >= 0 )
        ( *data ) = _mem[ id ];
    else 
        ( *data ) = _mem[ memsize + id ];
    return 0;
}

