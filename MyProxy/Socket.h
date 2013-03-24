// Socket.h: interface for the Socket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKET_H__3314CD0B_61CF_4CA2_80F6_B4D72403EE37__INCLUDED_)
#define AFX_SOCKET_H__3314CD0B_61CF_4CA2_80F6_B4D72403EE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Socket
{
	SOCKET s;
	char*	inbuf;
	char*	inbuf_ptr;
	char*	outbuf;
	bool	state;

public:
	Socket(SOCKET con) : s(con), state(true) {
		inbuf	= new char[inbufsz];	*inbuf=0;	inbuf_ptr=inbuf;
		outbuf	= new char[outbufsz];	*outbuf=0;
	};
	virtual ~Socket(){
		shutdown ( s, SD_BOTH ); closesocket(s); delete[] inbuf; delete[] outbuf;
	};
	void send(char* buf){ ::send( s, buf, strlen(buf), 0);};
	void __cdecl sendf(char* format,...);
	void getline(char* line, int size);
	operator!(){return !state;}
static const int inbufsz;
static const int outbufsz;
};

#endif // !defined(AFX_SOCKET_H__3314CD0B_61CF_4CA2_80F6_B4D72403EE37__INCLUDED_)
