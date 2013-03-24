// Socket.cpp: implementation of the Socket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Socket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int Socket::inbufsz = 1024;
const int Socket::outbufsz = 1024;
void Socket::sendf(char* format,...)
{
	int cnt;
	va_list arg_ptr;
	va_start( arg_ptr , format);
	cnt = _vsnprintf( outbuf, outbufsz, format, arg_ptr );
	va_end( arg_ptr );
	if(cnt == -1){ cnt = outbufsz; outbuf[cnt] = 0; }
	if(cnt < 0){
		send("Socket Internal error\n");
		return;
	}
	::send( s, outbuf, strlen(outbuf), 0);
}
void Socket::getline(char* line, int size)
{
	int remainingchars = size - 1;
	char  *s2, *snext;
	bool gotline = false;
	while(state){
		if(!*inbuf_ptr) {
			int len = recv( s, inbuf, inbufsz-1, 0);
			if(len == SOCKET_ERROR) {
				len = 0;
				state = false;
			}
			inbuf[len] = 0;
			inbuf_ptr = inbuf;
		}
		s2 = strchr(inbuf_ptr, '\n');
		if(s2) {
			snext = s2 + 1;
			*s2-- = 0;
			if(*s2 == '\r'){ *s2-- = 0;}
			if(*s2 == '\\'){ *s2 = 0; } else { gotline = true;}

		}else {
			snext = inbuf_ptr + strlen(inbuf_ptr);
		}
		strncat(line, inbuf_ptr, remainingchars);
		remainingchars = size - strlen(line) -1;
		inbuf_ptr = snext;
		
		if(gotline)
			return;
	}
}
