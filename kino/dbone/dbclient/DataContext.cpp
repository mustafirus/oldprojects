// DataContext.cpp: implementation of the DataContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "my_vector.h"
#include "DataContext.h"

#include "EditControl.h"
#include "FieldFrame.h"

static Frame* createFrame(short int type, DataContext* pdc){
	switch(type){
		case 0:
			ASSERT( pdc->getWnd() );
			return new EditControl( pdc->getWnd(), 0, NULL );
		case 1: //V
			return new VecFrame(false);
		case 2: // H
			return new VecFrame(true);
		case 3: //F
			return new FieldFrame();
		case 4: //L
//			return 'L';
		default:
			return NULL;
	}

}

static Frame* createFrame(FrameData* buf, short int num, DataContext* pdc){
	Frame* pf = createFrame(buf[num].type, pdc);
	for( int n = buf[num].first_child; n != 0; n = buf[n].next_brother ){
		pf->addchild( createFrame(buf, n, pdc) );
	}
	return pf;
}

Frame* TestDC::getFrame(){
	FrameData buf[1024];
	FILE* framein = fopen("P:\\dbone\\test\\testdata.bin", "r");
	size_t num = fread( buf, sizeof(FrameData), 1024, framein );
	fclose(framein);

	return createFrame(buf, 0, this);

}

