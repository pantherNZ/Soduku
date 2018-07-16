#pragma once

#ifndef _CGAME_H_
#define _CGAME_H_

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Backbuffer.h"
#include "Sprite.h"

// Class Declaration:
class CGame
{
	// class functions
	public:
		CGame();
		~CGame();

		bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
		void Process();
		void Render();
		void SetMousePos(int _iMouseX, int _iMouseY);
		void SetMouseStatus(bool _bLeftButton, bool _bMouseDown);

	protected:

	private:
		void LoadRandomSetup(bool _bLoadFirst);
		POINT FindSquareNumber(int _iX, int _iY);
		void FindScreenLocation(POINT _poSquare, int& _riX, int& _riY);

		void ProcessMouseClick();
		void ProcessShortcuts();
		void ProcessButtons();

		bool SolveSudoku();
		bool LuaFunction(int _iBoard1D[81]);

		void CheckCompletion(bool _bIncrement);
		bool ChangeSquare(POINT _pSquare, int _iVal);

		bool CheckValidPlacementRowColumn(int _iX, int _iY);
		bool CheckValidPlacementSquare(int _iX, int _iY);

		void FindNextNumber(POINT _poSquare);

	// class members
	public:

	protected:

	private:
		// Window data
		int m_iWidth;
		int m_iHeight;
		HWND m_hWnd;
		HDC m_hDC;
	
		// Mouse info
		int m_iMouseX;
		int m_iMouseY;
		bool m_bRightMouseDown;
		bool m_bLeftMouseDown;
		bool m_bHighlight;

		// Classes
		CBackBuffer* m_pBackbuffer;

		// Game objects
		CSprite* m_pBackground;
		CSprite* m_pHighlight;
		CSprite* m_pNumbers[9];

		int m_iSetupBoard[9][9];
		int m_iBoard[9][9];
		POINT m_poSquare;
		POINT m_poButtons[5];
		POINT m_poButtonCollisions[5];

		STATUS m_eStatus;
		LPCWSTR m_strStatusText[4];
		int m_iBoardCount;

		// Lua
		lua_State* m_pLuaState; 
};		



#endif // _CGAME_H_