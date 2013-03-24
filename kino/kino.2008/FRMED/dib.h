//dib.h.
//header file for the CDIBitmap class

#ifndef __DIB__
#define __DIB__

class CDIBitmap : public CObject
{
    DECLARE_SERIAL(CDIBitmap)

public:
    CDIBitmap();
    ~CDIBitmap();
protected:
    BITMAPINFO* m_pBMI;      //Pointer to the BITMAPINFO struct.
	int iBISize;
    BYTE* m_pBits;           //Pointer to the bitmap bits.
	int iBitsSize;
public:
    BITMAPINFO* GetBMHdrPtr() {return m_pBMI;} // Pointer to bitmap header.
    BYTE* GetBMBitsPtr() {return m_pBits;}     // Pointer to bitmap bits.
    RGBQUAD* GetBMColorTabPtr();           // Pointer to color table.
    int GetNumClrEntries();                // Number of color table entries.
    int GetBMWidth();
    int GetBMHeight();
    virtual BOOL LoadDIB(CFile* fp);  
    virtual void Serialize(CArchive& ar);
    virtual void DrawBitmap(CDC* pDC, int x, int y, int scale = 100);
	void DrawBitmapHiMetric(CDC* pDC, int scale);
};

#endif // __DIB__
