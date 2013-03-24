// StMdfr.h: interface for the StMdfr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STMDFR_H__F32B1332_15CE_11D2_A6AA_204C4F4F5020__INCLUDED_)
#define AFX_STMDFR_H__F32B1332_15CE_11D2_A6AA_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class RField;
struct FStyle;

class StMdfr  
{
	RField* pRField;
	const char* data;
	int		color1;
	int		color2;
	bool	autodel;
public:
	bool	focused;
	StMdfr(RField* prf, int c1, int c2) : pRField(prf), color1(c1), color2(c2), data(NULL), autodel(false){};
	virtual ~StMdfr(){ if(autodel)delete (char*)data;};

	RField* GetRField(){return pRField;};
	void SetData(const char* str){data = str;};
	void SetData(int s)
	{
		if(!data)
		{
			data = new char[4];
			autodel = true;
		}
		itoa(s, (char*)data, 10);
	};
	void ModifyStyle(FStyle* pst);
};

#endif // !defined(AFX_STMDFR_H__F32B1332_15CE_11D2_A6AA_204C4F4F5020__INCLUDED_)
