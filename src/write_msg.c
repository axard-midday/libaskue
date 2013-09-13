#include <stdio.h>
#include <time.h>

#define TIME_STRBUF 19

void write_msg ( FILE *output, const char *Hdr, const char *St, const char *Msg )
{
    time_t t = time ( NULL );
	char asctime_str[ TIME_STRBUF + 1 ];
	int len = strftime ( asctime_str, TIME_STRBUF + 1, "%Y-%m-%d %H:%M:%S", localtime ( &t ) );

	if ( len != TIME_STRBUF )
	{
		return;
	}
	
    if ( Msg )
    {
        fputs ( "[ " ,output );
        fputs ( asctime_str ,output );
        fputs ( " | ", output );
        fputs ( Hdr, output );
        fputs ( " | ", output );
        fputs ( St, output );
        fputs ( " ]: " ,output );
        fputs ( Msg, output );
        fputs ( "\n" ,output );
    }
    else
    {
        fputs ( "[ " ,output );
        fputs ( asctime_str ,output );
        fputs ( " | ", output );
        fputs ( Hdr, output );
        fputs ( " | ", output );
        fputs ( St, output );
        fputs ( " ]\n" ,output );
    }
    
    fflush ( output );
}

#undef TIME_STRBUF
