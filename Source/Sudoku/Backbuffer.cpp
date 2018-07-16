#include "Backbuffer.h"

// constructor
CBackBuffer::CBackBuffer(): m_hWnd(0), m_hDC(0), m_hSurface(0), m_hOldObject(0), m_iWidth(0), m_iHeight(0)
{

}

// destructor
CBackBuffer::~CBackBuffer()
{
	SelectObject(m_hDC, m_hOldObject);

	DeleteObject(m_hSurface);
	DeleteObject(m_hDC);
}

// Initalise the BB with handle to the window and dimensions
bool CBackBuffer::Initialise(HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hWnd = _hWnd;

	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	HDC hWindowDC = ::GetDC(m_hWnd);

	m_hDC = CreateCompatibleDC(hWindowDC);

	m_hSurface = CreateCompatibleBitmap(hWindowDC, m_iWidth, m_iHeight);

	ReleaseDC(m_hWnd, hWindowDC);

	m_hOldObject = static_cast<HBITMAP>(SelectObject(m_hDC, m_hSurface));

	HBRUSH brushWhite = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	m_hBrush = CreateSolidBrush(RGB(0,0,0));

	SelectObject(m_hDC, GetStockObject(DC_BRUSH));
	SelectObject(m_hDC, m_hBrush);

	return (true);
}

// clears the BB
void CBackBuffer::Clear(COLORREF _crColour)
{
	SetDCBrushColor(m_hDC, _crColour);
	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, GetStockObject(DC_BRUSH)));	

	Rectangle(GetBFDC(), -10, -10, GetWidth(), GetHeight());

	SelectObject(GetBFDC(), hOldBrush);
}

// returns the BFDC
HDC CBackBuffer::GetBFDC() const
{
	return (m_hDC);
}

// return width of the backbuffer
int CBackBuffer::GetWidth() const
{
	return (m_iWidth);
}

// return height of the backbuffer
int CBackBuffer::GetHeight() const
{
	return (m_iHeight);
}

// present the BB to the screen
void CBackBuffer::Present()
{
	HDC hWndDC = ::GetDC(m_hWnd);

	BitBlt(hWndDC, 0, 0, m_iWidth, m_iHeight, m_hDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hWndDC);
}