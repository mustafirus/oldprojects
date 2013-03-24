#include "StdAfx.h"
#include "domain.h"
#include "query.h"
#include "rfield.h"

RField::RField() : _pQField(NULL), _pRec(NULL), _pDataBuffer(NULL)
{
};

RField::RField(QField* pqf, Record* pr, RField* prf/* = NULL*/) : _pQField(pqf), _pRec(pr), _pDataBuffer(NULL)
{
	Domain* pd = *_pQField;
	_pDataBuffer = pd->new_buffer();
};

RField::~RField(void)
{
}
