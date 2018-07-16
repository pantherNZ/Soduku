// Self Include:
#include "System.h"

// Local Includes:

// Library Includes:

// Implementation:
CSystemClass::CSystemClass() : m_pGame(nullptr), m_iWidth(800), m_iHeight(600)
{												 

}

CSystemClass::~CSystemClass()
{
	// Remove the window.
	DestroyWindow(m_hWnd);
	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);

	// delete used classes
	DeleteOBJ(m_pGame);
}

bool CSystemClass::Initialise(HINSTANCE _hInstance)
{
	// Basic program settings
	m_applicationName = L"GD2S03 - Sudoku Lua / C++ - Alex Denford";
	m_hInstance = _hInstance;

	// Initialise the windows api.
	CreateRegisterWindow();

	// Create game class
	m_pGame = new CGame;

	// Initialise game class
	return(m_pGame->Initialise(m_hInstance, m_hWnd, m_iWidth, m_iHeight));
}

void CSystemClass::CreateRegisterWindow()
{
	WNDCLASSEX wc;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, L"Registering Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}

	// Determine the resolution of the clients desktop screen.
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	m_iWidth = 800;
	m_iHeight = 625;

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(NULL, m_applicationName, m_applicationName, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
						(iScreenWidth / 2) - (m_iWidth / 2), (iScreenHeight / 2) - (m_iHeight / 2),
						m_iWidth, m_iHeight, NULL, NULL, m_hInstance, NULL);

	// check if creating the window failed
	if(!m_hWnd)
	{
		MessageBox(0, L"Creating Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}
}

void CSystemClass::Run()
{
	MSG msg;

	// Initialise the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ExecuteFrame();
		}
	}
}

void CSystemClass::ExecuteFrame()
{
	// render scene
	m_pGame->Process();

	// render scene
	m_pGame->Render();
}

CSystemClass* CSystemClass::GetInstance()
{
	if (s_pApplication == nullptr)
	{
		s_pApplication = new CSystemClass;
	}

	return (s_pApplication);
}

void CSystemClass::DestroyInstance()
{
	DeleteOBJ(s_pApplication);
}

LRESULT CALLBACK CSystemClass::MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{	
	switch(_uiMsg)
	{
		// Mouse move
		case WM_MOUSEMOVE:
		{
			int iMouseX = LOWORD(_lParam);
			int iMouseY = HIWORD(_lParam);

			// update the game stored values for X & Y
			m_pGame->SetMousePos(iMouseX, iMouseY);
		}
		break;

		// Mouse down
		case WM_LBUTTONDOWN:
		{
			// if mouse clicked, update game
			m_pGame->SetMouseStatus(true, true);
		}
		break;

		// Mouse up
		case WM_LBUTTONUP:
		{
			// if mouse clicked, update game
			m_pGame->SetMouseStatus(true, false);
		}
		break;

		// Mouse down
		case WM_RBUTTONDOWN:
		{
			// if mouse clicked, update game
			m_pGame->SetMouseStatus(false, true);
		}
		break;

		// Mouse up
		case WM_RBUTTONUP:
		{
			// if mouse clicked, update game
			m_pGame->SetMouseStatus(false, false);
		}
		break;

		default: break;
	}
	
	return(DefWindowProc(_hwnd, _uiMsg, _wParam, _lParam));
}

CGame* CSystemClass::GetGameClass() const
{
	return(m_pGame);
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uiMsg)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return(CSystemClass::GetInstance()->MessageHandler(_hwnd, _uiMsg, _wParam, _lParam));
		}
	}
}