// library includes:

// Local includes:
#include "Game.h"
#include "Backbuffer.h"

// self include:
#include "Sprite.h"

// Statics
HDC CSprite::s_hSharedSpriteDC = 0;
int CSprite::s_iRefCount = 0;

// constructor
CSprite::CSprite() : m_iX(0), m_iY(0)
{
	++s_iRefCount;
}

// destructor
CSprite::~CSprite()
{
	DeleteObject(m_hSprite);
	DeleteObject(m_hMask);

	--s_iRefCount;

	if(s_iRefCount == 0)
	{
		DeleteDC(s_hSharedSpriteDC);
		s_hSharedSpriteDC = 0;
	}
}

// create / initalise the sprite using 2 resources (sprite ID + mask ID)
bool CSprite::Initialise(HINSTANCE _hInstance, int _iSpriteResourceID, int _iMaskResourceID, HDC _hDC)
{
	m_hDC = _hDC;

	if(!s_hSharedSpriteDC)
	{
		s_hSharedSpriteDC = CreateCompatibleDC(NULL);
	}

	m_hSprite = LoadBitmap(_hInstance, MAKEINTRESOURCE(_iSpriteResourceID));
	m_hMask = LoadBitmap(_hInstance, MAKEINTRESOURCE(_iMaskResourceID));

	if(m_hMask == NULL || m_hSprite == NULL)
	{
		return(false);
	}

	GetObject(m_hSprite, sizeof(BITMAP), &m_bitmapSprite);
	GetObject(m_hMask, sizeof(BITMAP), &m_bitmapMask);

	return(true);
}

// Draw the sprite
void CSprite::Render()
{
	int iW = GetWidth();
	int iH = GetHeight();

	int iX = int(m_iX) - (iW / 2);
	int iY = int(m_iY) - (iH / 2);

	HGDIOBJ hOldObj = SelectObject(s_hSharedSpriteDC, m_hMask);

	BitBlt(m_hDC, iX, iY, iW, iH, s_hSharedSpriteDC, 0, 0, SRCAND);

	SelectObject(s_hSharedSpriteDC, m_hSprite);

	BitBlt(m_hDC, iX, iY, iW, iH, s_hSharedSpriteDC, 0, 0, SRCPAINT);

	SelectObject(s_hSharedSpriteDC, hOldObj);
}

// get sprite width
int CSprite::GetWidth() const
{
	return(m_bitmapSprite.bmWidth);
}

// get sprite Height
int CSprite::GetHeight() const
{
	return(m_bitmapSprite.bmHeight);
}

// get sprite X pos
int CSprite::GetX() const
{
	return(m_iX);
}

// get sprite Y pos
int CSprite::GetY() const
{
	return(m_iY);
}
	
// translate the sprite in a direction by X & Y
void CSprite::TranslateRelative(int _iX,  int _iY)
{
	m_iX += _iX;
	m_iY += _iY;
}

// translate the sprite to a specific spot
void CSprite::TranslateAbsolute(int _iX, int _iY)
{
	m_iX = _iX;
	m_iY = _iY;
}
