extern "C" {
#include "jpeglib.h"
}


class CDIB {
public:
	CDIB(int w, int h);
	CDIB();
	~CDIB();
	virtual bool ImageLoaded();	// Check if a picture is loaded
	virtual bool Load(CString fname); // Load from file
	virtual bool Save(CString fname); // Save to file
	virtual void Draw(CDC* pDC, int x, int y, int width, int height);
	virtual int DibWidth();
	virtual int DibHeight();
	void* GetPixelAddress(int x, int y);
	int StorageWidth();
	void brighten(int value);
	void negative();
	void grey();
	void blending(CDIB& b1, CDIB& b2, int p);
	void histogramm(float* h, float zoom);
	void contrast(float alpha);
	void rgb(char ch);
	void matrix(int* matrix, int matrixsize, int koeff, char offset = 0);
	void flip(char c);
	bool SaveJpeg(CString pszFileName, int quality);
	bool LoadJpeg(CString pszFileName);
protected:
	BITMAPFILEHEADER* m_pBMFH;	// Pointer to BITMAPFILEHEADER
	BITMAPINFO* m_pBMI;			// Pointer to BITMAPINFO struct
	BYTE* m_pBits;				// Pointer to Pixelbits
	DWORD m_dwLength;			// length of DIB (including headers)
};