#pragma once

#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

// Local Includes
#include "Utility.h"

class CBackBuffer
{
	// Member Functions
	public:
		CBackBuffer();
		~CBackBuffer();

		bool Initialise(HWND _hWnd, int _iWidth, int _iHeight);

		HDC GetBFDC() const;
		int GetHeight() const;
		int GetWidth() const;

		void Clear(COLORREF _crColour);
		void Present();

	protected:

	private:


	// Member Variables
	public:

	protected:
		int m_iWidth;
		int m_iHeight;
		HWND m_hWnd;
		HDC m_hDC;

		HBITMAP m_hSurface;
		HBITMAP m_hOldObject;
		HBRUSH m_hBrush;

	private:
};
#endif // __BACKBUFFER_H__