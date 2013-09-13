#include <stdint.h>
#include <string.h>

#include "my.h"
#include "uint8_array.h"

void uint8_array_init ( uint8_array_t *u8a, size_t size )
{
    if ( size > 0 )
    {
        u8a->Item = ( uint8_t* ) mymalloc ( sizeof ( uint8_t ) * size );
        u8a->Size = size;
        memset ( u8a->Item, 0, size );
    }
    else
    {
        u8a->Size = 0;
        u8a->Item = NULL;
    }
}

void uint8_array_destroy ( uint8_array_t* ptr )
{
	myfree ( ptr->Item );
    ptr->Size = 0;
    ptr->Item = NULL;
}

void uint8_array_resize ( uint8_array_t* ptr, size_t size )
{
    if ( ptr == NULL )
    {
        return;
    }
    else if ( size > 0 )
    {
        ptr->Item = myrealloc ( ptr->Item, size );
        ptr->Size = size;
    }
    else
    {
        myfree ( ptr->Item );
        ptr->Size = 0;
        ptr->Item = NULL;
    }
}

void uint8_array_append ( uint8_array_t* Dest, const uint8_t *Data, size_t DataSize )
{
    if ( Dest == NULL )
        return;
        
	size_t OldSize = Dest->Size;
	uint8_array_resize ( Dest, Dest->Size + DataSize );
    memcpy ( Dest->Item + OldSize, Data, DataSize );
}

void uint8_array_update ( uint8_array_t *Dest, const uint8_t *Data, size_t DataSize )
{
    if ( Dest == NULL )
        return;
        
	uint8_array_resize ( Dest, DataSize );
	memcpy ( Dest->Item, Data, DataSize );
}

void uint8_array_extract ( uint8_array_t *Dest, const uint8_array_t *Src, size_t Start, size_t Stop )
{
    if ( Stop <= Start )
        return;
    
    size_t DestSize = Stop - Start;
    uint8_array_init ( Dest, DestSize );
    uint8_array_update ( Dest, Src->Item + Start, DestSize );
}
