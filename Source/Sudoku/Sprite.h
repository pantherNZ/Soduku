#pragma once

#ifndef __SPRITE_H__
#define __SPRITE_H__

// Library Includes:

// Local Includes:
#include "Utility.h"

// Class Declaration:
class CSprite
{
	// class functions
	public:
		CSprite();
		~CSprite();

		bool Initialise(HINSTANCE _hInstance, int _iResourceID, int _iMarkResourceID, HDC _hDC);
		void Render();

		int GetWidth() const;
		int GetHeight() const;

		int GetX() const;
		int GetY() const;

		void TranslateRelative(int _iX, int _iY);
		void TranslateAbsolute(int _iX, int _iY);

	protected:

	private:

	// class members
	public:

	protected:
		HDC m_hDC;
		int m_iX;
		int m_iY;

		HBITMAP m_hSprite;
		HBITMAP m_hMask;

		BITMAP m_bitmapSprite;
		BITMAP m_bitmapMask;

		static HDC s_hSharedSpriteDC;
		static int s_iRefCount;

	private:

};

#endif