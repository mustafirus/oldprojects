//dib.cpp
//implementation file for CDIBitmap class

#include "stdafx.h"
#include "malloc.h"
#include "dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBitmap

IMPLEMENT_SERIAL(CDIBitmap, CObject, 0 )

CDIBitmap::CDIBitmap() : m_pBMI(NULL), iBISize(0), m_pBits(NULL), iBitsSize(0)
{
}

CDIBitmap::~CDIBitmap()
{
    // Free memory.
    if (m_pBMI != NULL)
    	free(m_pBMI);
    if (m_pBits != NULL)
    	free(m_pBits);
}

/////////////////////////////////////////////////////////////////////////////

//Serialization not supported. Use the ASSERT() statement
//to prevent accidental overwriting.
void CDIBitmap::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << iBISize;
		ar.Write(m_pBMI, iBISize);
		ar << iBitsSize;
		ar.Write(m_pBits, iBitsSize);
	}else
	{
		ar >> iBISize;
		if(m_pBMI)
			free(m_pBMI);
		m_pBMI =(BITMAPINFO*) malloc(iBISize);
		ar.Read(m_pBMI, iBISize);
		ar >> iBitsSize;
		if(m_pBits)
			free(m_pBits);
		if(iBitsSize)
		{
			m_pBits = (BYTE*) malloc(iBitsSize);
			ar.Read(m_pBits, iBitsSize);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Private functions

static BOOL IsPMDIB(BITMAPINFOHEADER* pBIH)
{
//Returns TRUE if the bitmap information header pointed to by 
//pBIH is a Presentation Manager style. 
    ASSERT(pBIH);
    if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER))
        return TRUE;
    
    return FALSE;
}

static int GetColorEntriesInDIB(BITMAPINFO* pBmpInfo) 
{
    BITMAPINFOHEADER* pBIH;
    BITMAPCOREHEADER* pBCH;
    int iColors = 0, iBitsPerPixel;

    ASSERT(pBmpInfo);

    pBIH = &(pBmpInfo->bmiHeader);
    pBCH = (BITMAPCOREHEADER*) pBIH;

	//In Windows DIB's, the color table length is specified
	//by the biClrUsed field, but only if that field is non-zero.
    if ((!IsPMDIB(pBIH)) && (pBIH->biClrUsed != 0))
    	{
        iColors = pBIH->biClrUsed;
		return iColors;
    	}

    // Otherwise we can assume that the color table size depends
    // the number of bits per pixel.
    if (IsPMDIB(pBIH))
    	iBitsPerPixel = pBCH->bcBitCount;
    else 
        iBitsPerPixel = pBIH->biBitCount;

	if (iBitsPerPixel == 1)
		iColors = 2;

	if (iBitsPerPixel == 4)
		iColors = 16;

	if (iBitsPerPixel == 8)
		iColors = 256;

	return iColors;
}


/////////////////////////////////////////////////////////////////////////////
// CDIBitmap commands

BOOL CDIBitmap::LoadDIB(CFile* fp)
{
//Load a DIB from a file. The file will already have been
//opened by the framework's serialization code.

    BOOL bIsPM = FALSE;
    BITMAPINFO* pBmpInfo = NULL;
    BYTE* pBits = NULL;
	int iColors, iColorTableSize;

    // Get the current file position.
    DWORD dwFileStart = fp->GetPosition();

    //Read the file header. This will tell us the file size and
    //where the data bits start in the file.
    BITMAPFILEHEADER BmpFileHdr;
    int iBytes;
    iBytes = fp->Read(&BmpFileHdr, sizeof(BmpFileHdr));
    if (iBytes != sizeof(BmpFileHdr))
    	{
        AfxMessageBox("Error reading file header");
        goto error;
    	}

    //Do we have "BM" at the start indicating a bitmap file?.
    if (BmpFileHdr.bfType != 0x4D42)
    	{
        AfxMessageBox("Not a bitmap file");
        goto error;
	    }

    //Assume for now that the file is a Windows DIB.
    //Read the BITMAPINFOHEADER. If the file is a PM DIB
    //file we'll convert it later.
    BITMAPINFOHEADER BmpInfoHdr;
    iBytes = fp->Read(&BmpInfoHdr, sizeof(BmpInfoHdr)); 
    if (iBytes != sizeof(BmpInfoHdr))
    	{
        AfxMessageBox("Error reading header information");
        goto error;
    	}

    //Was it a real Windows DIB file?
    if (BmpInfoHdr.biSize != sizeof(BITMAPINFOHEADER))
		{
    	//Not a Windows DIB. Set a flag indicating it's
    	//a PM format DIB.

        bIsPM = TRUE;
    
		//Is it really a PM DIB?
		if (BmpInfoHdr.biSize != sizeof(BITMAPCOREHEADER))
       		{
			//No!
       		AfxMessageBox("Unknown DIB format");
       		goto error;
       		}

	    //We reach here only if it's a PM format DIB.
	    //Rewind the file pointer to read the BITMAPCOREHEADER
    	//data, then create the BITMAPINFOHEADER from it.
        fp->Seek(dwFileStart + sizeof(BITMAPFILEHEADER), CFile::begin);
        BITMAPCOREHEADER BmpCoreHdr;
        iBytes = fp->Read(&BmpCoreHdr, sizeof(BmpCoreHdr)); 
        if (iBytes != sizeof(BmpCoreHdr))
        	{
           	AfxMessageBox("Error reading header information");
           	goto error;
        	}

        BmpInfoHdr.biSize = sizeof(BITMAPINFOHEADER);
        BmpInfoHdr.biWidth = (int) BmpCoreHdr.bcWidth;
        BmpInfoHdr.biHeight = (int) BmpCoreHdr.bcHeight;
        BmpInfoHdr.biPlanes = BmpCoreHdr.bcPlanes;
        BmpInfoHdr.biBitCount = BmpCoreHdr.bcBitCount;
        BmpInfoHdr.biCompression = BI_RGB;
        BmpInfoHdr.biSizeImage = 0;
        BmpInfoHdr.biXPelsPerMeter = 0;
        BmpInfoHdr.biYPelsPerMeter = 0;
        BmpInfoHdr.biClrUsed = 0;
        BmpInfoHdr.biClrImportant = 0;
    }
	    
	//Determine memory needed.
    iColors = GetColorEntriesInDIB((LPBITMAPINFO) &BmpInfoHdr);
    iColorTableSize = iColors * sizeof(RGBQUAD);
    
    //Allocate memory for 256 entries.
    iBISize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
    iBitsSize = BmpFileHdr.bfSize - BmpFileHdr.bfOffBits;

    //Allocate memory for header.
    pBmpInfo = (LPBITMAPINFO) malloc(iBISize);
    if (!pBmpInfo)
    	{
        AfxMessageBox("Insufficient memory for DIB header");
        goto error;
    	}

    //Copy header to newly allocated memory.
    memcpy(pBmpInfo, &BmpInfoHdr, sizeof(BITMAPINFOHEADER));

    //Read the color table data from the file.
    if (bIsPM == FALSE)
    	{
        iBytes = fp->Read(((LPBYTE) pBmpInfo) + sizeof(BITMAPINFOHEADER),
                             iColorTableSize);
        if (iBytes != iColorTableSize)
        	{
            AfxMessageBox("Error reading color table");
            goto error;
        	}
		}
	else
		{
        //Read each PM color table entry and convert it to Windows
        //DIB format.
        LPRGBQUAD lpRGB;
        lpRGB = (LPRGBQUAD) ((LPBYTE) pBmpInfo + sizeof(BITMAPINFOHEADER));
        RGBTRIPLE rgbt;
        for (int i = 0; i < iColors; i++)
        	{
            iBytes = fp->Read(&rgbt, sizeof(RGBTRIPLE));
            if (iBytes != sizeof(RGBTRIPLE))
            	{
                AfxMessageBox("Error reading color table data.");
                goto error;
            	}
            lpRGB->rgbBlue = rgbt.rgbtBlue;
            lpRGB->rgbGreen = rgbt.rgbtGreen;
            lpRGB->rgbRed = rgbt.rgbtRed;
            lpRGB->rgbReserved = 0;
            lpRGB++;
        	}
    	}

    // Allocate memory for the bitmap bits.
    pBits = (BYTE*) malloc(iBitsSize);
    if (pBits == NULL)
    	{
        AfxMessageBox("Insufficient memory to read DIB bits");
        goto error;
    	}

    //Move file pointer to file location of bits.
    fp->Seek(dwFileStart + BmpFileHdr.bfOffBits, CFile::begin);

    // Read data.
    iBytes = fp->Read(pBits, iBitsSize);
    if (iBytes != iBitsSize)
    	{
        AfxMessageBox("Error reading bitmap data");
        goto error;
    	}

    //We reach here only if no errors occurred.
    if (m_pBMI != NULL)
    	free(m_pBMI);
    m_pBMI = pBmpInfo; 
    if (m_pBits != NULL)
    	free(m_pBits);
    m_pBits = pBits;
    return TRUE;
                
//Oops!
error: 
    if (pBmpInfo)
    	free(pBmpInfo);
    if (pBits)
    	free (pBits);
    return FALSE;    
}

RGBQUAD* CDIBitmap::GetBMColorTabPtr()
{
//Returns a pointer to the bitmap's color table data.
	return (LPRGBQUAD)(((BYTE*)(m_pBMI)) + sizeof(BITMAPINFOHEADER));
}
   
void CDIBitmap::DrawBitmap(CDC* pDC, int x, int y, int scale)
{
//Draw the DIB to the specified device context at position (x,y).

	if (m_pBMI == NULL)
		return;

    ::StretchDIBits(pDC->GetSafeHdc(),
                  x,                        // Destination x
                  y,                        // Destination y
                  m_pBMI->bmiHeader.biWidth * scale / 100,  // Destination width
                  m_pBMI->bmiHeader.biHeight * scale / 100, // Destination height
                  0,                        // Source x
                  0,                        // Source y
                  m_pBMI->bmiHeader.biWidth,   // Source width
                  m_pBMI->bmiHeader.biHeight,  // Source height
                  GetBMBitsPtr(),           // Pointer to bits
                  GetBMHdrPtr(),		    // ptr to BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  SRCCOPY);		            // Raster operation code (ROP)
}

void CDIBitmap::DrawBitmapHiMetric(CDC* pDC, int scale)
{
//Draw the DIB to the specified device context at position (x,y).

	if (m_pBMI == NULL)
		return;
	int width = m_pBMI->bmiHeader.biWidth * 100000 /
				  m_pBMI->bmiHeader.biXPelsPerMeter * scale / 100;
	int height = m_pBMI->bmiHeader.biHeight * 100000 /
				  m_pBMI->bmiHeader.biYPelsPerMeter * scale / 100;

    ::StretchDIBits(pDC->GetSafeHdc(),
                  0,                    // Destination x
                  0,                   // Destination y
                  width,						// Destination width
                  -height,						// Destination height
                  0,                        // Source x
                  0,                        // Source y
                  m_pBMI->bmiHeader.biWidth,   // Source width
                  m_pBMI->bmiHeader.biHeight,  // Source height
                  GetBMBitsPtr(),           // Pointer to bits
                  GetBMHdrPtr(),		    // ptr to BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  SRCCOPY);		            // Raster operation code (ROP)
}

// Get the number of color table entries.
int CDIBitmap::GetNumClrEntries()
{
    return GetColorEntriesInDIB(m_pBMI);
}

int CDIBitmap::GetBMWidth()
{
//Return bitmap width.
	return m_pBMI->bmiHeader.biWidth;
} 

int CDIBitmap::GetBMHeight()
{
//Return bitmap height.
	return m_pBMI->bmiHeader.biHeight;
}
