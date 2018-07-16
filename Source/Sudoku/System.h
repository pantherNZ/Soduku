#pragma once

#ifndef _CSYSTEMCLASS_H_
#define _CSYSTEMCLASS_H_

// Definitions

// Library Includes:

// Local Includes:
#include "Utility.h"
#include "Game.h"

// Class Declaration:
class CSystemClass
{
	// class functions
	public:	
		~CSystemClass();

		bool Initialise(HINSTANCE _hInstance);
		void Run();

		CGame* GetGameClass() const;
		LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

		// Singleton Methods
		static CSystemClass* GetInstance();
		static void DestroyInstance();

	protected:

	private:

		// private constructor (singleton)
		CSystemClass();

		// privately called functions
		void ExecuteFrame();
		void CreateRegisterWindow();

	// class members
	public:

	protected:

	private:
		// Singleton Instance
		static CSystemClass* s_pApplication;

		// system app data
		HWND m_hWnd;
		int m_iWidth;
		int m_iHeight;

		// other data
		HMENU hMenu;
		LPCWSTR m_applicationName;
		HINSTANCE m_hInstance;		

		// classes used
		CGame* m_pGame;

		// dialog data
		HWND m_hDlg;
};

// Windows callback definition
static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

#endif // _CSYSTEMCLASS_H_