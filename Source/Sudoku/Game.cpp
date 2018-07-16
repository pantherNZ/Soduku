// Self Include:
#include "Game.h"

// Local Includes:
#include "resource.h"

// Library Includes:

// Implementation:
CGame::CGame() : m_hWnd(NULL), m_iWidth(0), m_iHeight(0), m_pBackbuffer(nullptr), m_pBackground(nullptr), m_iMouseX(0), m_iMouseY(0), 
				m_bLeftMouseDown(false), m_bRightMouseDown(false), m_bHighlight(false), m_eStatus(PUZZLE_SETUP), m_iBoardCount(0)
{
	m_poSquare.x = -1;
	m_poSquare.y = -1;
}

CGame::~CGame()
{
	DeleteOBJ(m_pBackbuffer);
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pHighlight);

	for(int i = 0; i < 9; ++i)
	{
		DeleteOBJ(m_pNumbers[i]);
	}
}

bool CGame::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;
	m_hWnd = _hWnd;

	// Create Backbuffer
	m_pBackbuffer = new CBackBuffer;

	// Initialise Backbuffer
	if(!m_pBackbuffer->Initialise(_hWnd, _iWidth, _iHeight))
	{
		MessageBox(NULL, L"Initialising Backbuffer Failed", L"Error", MB_OK);	
		return(false);
	}

	m_hDC = m_pBackbuffer->GetBFDC();

	// Create Game Sprites
	m_pBackground = new CSprite;

	if(!m_pBackground->Initialise(_hInstance, IDB_BACKGROUND, IDB_BACKGROUND, m_pBackbuffer->GetBFDC()))
	{
		MessageBox(NULL, L"Initialising Background Failed", L"Error", MB_OK);	
		return(false);
	}

	m_pBackground->TranslateAbsolute(_iWidth / 2, (_iHeight - 25) / 2);

	// Highlight sprite
	m_pHighlight = new CSprite;

	if(!m_pHighlight->Initialise(_hInstance, IDB_HIGHLIGHT, IDB_HIGHLIGHT, m_pBackbuffer->GetBFDC()))
	{
		MessageBox(NULL, L"Initialising Highlight Sprite Failed", L"Error", MB_OK);	
		return(false);
	}

	// Create number sprites
	for(int i = 0; i < 9; ++i)
	{
		m_pNumbers[i] = new CSprite;

		// Initialise
		if(!m_pNumbers[i]->Initialise(_hInstance, IDB_NUMBER1 + i * 2, IDB_NUMBER1_MASK + i * 2, m_pBackbuffer->GetBFDC()))
		{
			MessageBox(NULL, L"Initialising Number Sprites Failed", L"Error", MB_OK);	
			return(false);
		}
	}

	// Initialise board
	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			m_iBoard[i][j] = 0;
		}
	}

	for(int i = 0; i < 5; ++i)
	{
		m_poButtons[i].x = 665;
		m_poButtons[i].y = 232 + i * 68;

		m_poButtonCollisions[i].x = (i == 0 ? 100 : (i == 1 ? 87 : 70));
		m_poButtonCollisions[i].y = 28;
	}	

	// Text for puzzle status
	m_strStatusText[PUZZLE_SETUP] = L"Status: Puzzle Setup";
	m_strStatusText[PLAYER_SOLVING] = L"Status: Player Solving";
	m_strStatusText[COMPUTER_SOLVING] = L"Status: Computer Solving";
	m_strStatusText[PUZZLE_SOLVED] = L"Status: Solved";

	// Setup text font
	SetBkMode(m_hDC, TRANSPARENT);
	HFONT hFont = CreateFont(30, 12, 0, 0, 500, false, false, false, DEFAULT_CHARSET, 0, 0, 0, 0, L"Imprint MT Shadow");
	SelectFont(m_hDC, hFont);
	SetTextColor(m_hDC, RGB(0, 0, 255));

	LoadRandomSetup(true);

	return(true);
}

void CGame::Process()
{
	// Game processes split into smaller functions
	ProcessMouseClick();
	ProcessShortcuts();
	ProcessButtons();
}

void CGame::ProcessMouseClick()
{
	if(m_bLeftMouseDown)
	{
		if(m_poSquare.x != -1 && m_poSquare.y != -1)
		{
			bool bValid = (m_iBoard[m_poSquare.x][m_poSquare.y] == 0);

			FindNextNumber(m_poSquare);

			m_bLeftMouseDown = false;

			if(m_iBoard[m_poSquare.x][m_poSquare.y] == 0)
			{
				--m_iBoardCount;
			}
			else if(bValid)
			{
				++m_iBoardCount;
			}

			CheckCompletion(false);
		}
	}
}

void CGame::ProcessShortcuts()
{
	if(m_bHighlight)
	{
		// find if they are over the grid
		if(m_poSquare.x != -1 && m_poSquare.y != -1)
		{
			bool bValid = (m_iBoard[m_poSquare.x][m_poSquare.y] == 0);

			// shortcuts
			if(GetAsyncKeyState(VK_DELETE) & 0x8000 || GetAsyncKeyState(0x30) & 0x8000)
			{
				if(!bValid) --m_iBoardCount;
				ChangeSquare(m_poSquare, 0);			
				CheckCompletion(false);
			}
			else if(GetAsyncKeyState(0x31) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 1);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x32) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 2);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x33) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 3);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x34) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 4);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x35) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 5);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x36) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 6);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x37) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 7);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x38) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 8);
				CheckCompletion(bValid && bSuccess);
			}
			else if(GetAsyncKeyState(0x39) & 0x8000) 
			{
				bool bSuccess = ChangeSquare(m_poSquare, 9);
				CheckCompletion(bValid && bSuccess);
			}
		}
	}

	// Solve (if 'S' pushed)
	if(GetAsyncKeyState(0x53) & 0x8000) 
	{
		if(m_eStatus == PUZZLE_SETUP)
		{
			MessageBox(NULL, L"The Sudoku can't be solved if it is still being set up (Push SPACE to begin)", L"Solving Unsuccessful", MB_OK);
		}
		else
		{
			if(SolveSudoku())
			{
				MessageBox(NULL, L"The Sudoku has been solved", L"Solving Successful", MB_OK);
			}
			else
			{
				MessageBox(NULL, L"No solution has been found for the Sudoku", L"Solving Unsuccessful", MB_OK);
			}	
		}
	}

	// Start game check (if ENTER pushed)
	if(GetAsyncKeyState(VK_SPACE) & 0x8000 && m_eStatus == PUZZLE_SETUP)
	{
		if(MessageBox(NULL, L"Start the game?", L"Setup Complete", MB_YESNO) == IDYES)
		{
			m_eStatus = PLAYER_SOLVING;

			// Store board setup
			for(int i = 0; i < 9; ++i)
			{
				for(int j = 0; j < 9; ++j)
				{
					m_iSetupBoard[i][j] = m_iBoard[i][j];
				}
			}
		}
	}

	// Reset the board (if 'C' pushed)
	if(GetAsyncKeyState(0x43) & 0x8000)
	{
		if(MessageBox(NULL, L"Clear the board?", L"Clear board", MB_YESNO) == IDYES)
		{
			for(int i = 0; i < 9; ++i)
			{
				for(int j = 0; j < 9; ++j)
				{
					m_iBoard[i][j] = 0;
				}
			}

			m_eStatus = PUZZLE_SETUP;
		}
	}
}

void CGame::ProcessButtons()
{
	if(m_bLeftMouseDown)
	{
		// Check button collisions
		for(int i = 0; i < 5; ++i)
		{
			if((m_iMouseX >= m_poButtons[i].x - m_poButtonCollisions[i].x && m_iMouseX <= m_poButtons[i].x + m_poButtonCollisions[i].x) &&
			   (m_iMouseY >= m_poButtons[i].y - m_poButtonCollisions[i].y && m_iMouseY <= m_poButtons[i].y + m_poButtonCollisions[i].y))
			{
				m_bLeftMouseDown = false;

				switch(i)
				{
					// Random
					case(0):
					{
						if(m_eStatus != PUZZLE_SETUP)
						{	
							if(MessageBox(NULL, L"Load a new random puzzle? (you will lose your current progress)", L"New Puzzle", MB_YESNO) == IDNO)
							{
								break;
							}
						}

						LoadRandomSetup(false);
						m_eStatus = PUZZLE_SETUP;
					}
					break;

					// Restart
					case(1):
					{
						if(m_eStatus != PUZZLE_SETUP)
						{
							if(MessageBox(NULL, L"Restart the puzzle? (you will lose your current progress)", L"Restart Puzzle", MB_YESNO) == IDNO)
							{
								break;
							}
							
							// Store board setup
							for(int i = 0; i < 9; ++i)
							{
								for(int j = 0; j < 9; ++j)
								{
									m_iBoard[i][j] = m_iSetupBoard[i][j];
								}
							}

							m_eStatus = PLAYER_SOLVING;
						}
					}
					break;

					// Solve
					case(2):
					{
						if(m_eStatus == PUZZLE_SETUP)
						{
							MessageBox(NULL, L"The Sudoku can't be solved if it is still being set up (Push SPACE to begin)", L"Solving Unsuccessful", MB_OK);
						}
						else if(m_eStatus == PUZZLE_SOLVED)
						{
							MessageBox(NULL, L"The Sudoku has already been solved", L"Solving Unsuccessful", MB_OK);
						}
						else
						{
							if(SolveSudoku())
							{
								MessageBox(NULL, L"The Sudoku has been solved", L"Solving Successful", MB_OK);
							}
							else
							{
								MessageBox(NULL, L"No solution has been found for the Sudoku", L"Solving Unsuccessful", MB_OK);
								m_eStatus = PLAYER_SOLVING;
							}		
						}
					}
					break;

					// Help
					case(3):
					{
						std::wstringstream ss;
						ss << L"2014-08-14 - GD2S03 - Summative_1_Lua - Alex Denford\n\n";
						ss << L"This program allows the user to import random Sudoku's from a file, modify them or create their own. The program has a built in solver which uses LUA.";
						ss << L"\n\nControls: \nMouse 1 - Place a number at the mouse location (through 0 - 9)"; 
						ss << L"\nNumber keys 0 - 9 - Place a number at the mouse location"; 
						ss << L"\nSpace - Tells the program you want to start solving"; 
						ss << L"\n'S' key - Asks the program to solve the puzzle"; 
						ss << L"\n'C' key - Clears the board"; 
						ss << L"\n\nHave fun! Created by Alex Denford"; 
						MessageBox(NULL, ss.str().c_str(), L"Help", MB_OK);
					}
					break;

					// Quit
					case(4):
					{
						if(MessageBox(NULL, L"Are you sure?", L"Quit Game", MB_YESNO) == IDYES)
						{
							PostQuitMessage(0);
						}											
					}
					break;
				}
			}
		}
	}
}

void CGame::SetMousePos(int _iX, int _iY)
{
	m_iMouseX = _iX;
	m_iMouseY = _iY;

	// find if they are on the grid
	m_poSquare = FindSquareNumber(m_iMouseX, m_iMouseY);

	if(m_poSquare.x != -1 && m_poSquare.y != -1)
	{
		int iX = 0;
		int iY = 0;

		FindScreenLocation(m_poSquare, iX, iY);
		m_pHighlight->TranslateAbsolute(iX, iY);

		if(!m_bHighlight)
		{
			m_bHighlight = true;
		}
	}
	else if(m_bHighlight)
	{
		m_bHighlight = false;
	}
}

void CGame::SetMouseStatus(bool _bLeftButton, bool _bMouseDown)
{
	if(_bLeftButton)
	{
		m_bLeftMouseDown = _bMouseDown;
	}
	else
	{
		m_bRightMouseDown = _bMouseDown;
	}
}

void CGame::Render()
{
	// Clear the backbuffer
	m_pBackbuffer->Clear(RGB(0,0,0));

	// Render background
	m_pBackground->Render();

	// Render highlight
	if(m_bHighlight)
	{
		m_pHighlight->Render();
	}

	// Render numbers
	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if(m_iBoard[i][j] != 0)
			{
				int iX = 0;
				int iY = 0;

				POINT p = {i, j};

				FindScreenLocation(p, iX, iY);

				m_pNumbers[m_iBoard[i][j] - 1]->TranslateAbsolute(iX, iY);
				m_pNumbers[m_iBoard[i][j] - 1]->Render();
			}
		}
	}

	// Render Text
	RECT r = {178, m_iHeight - 88, 0, 0};
	DrawText(m_hDC, m_strStatusText[m_eStatus], -1, &r, DT_NOCLIP);

	// Present backbuffer to the screen
	m_pBackbuffer->Present();
}

POINT CGame::FindSquareNumber(int _iX, int _iY)
{
	POINT p = {-1, -1};

	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			int iX = 0;
			int iY = 0;

			POINT pTemp = {i, j};
			FindScreenLocation(pTemp, iX, iY);

			if(_iX >= iX - HALFSQUARESIZE && _iX <= iX + HALFSQUARESIZE &&
			   _iY >= iY - HALFSQUARESIZE && _iY <= iY + HALFSQUARESIZE)
			{
				p.x = i;
				p.y = j;

				break;
			}
		}
	}

	return(p);
}

void CGame::FindScreenLocation(POINT _poSquare, int& _riX, int& _riY)
{
	_riX = GRIDOFFSETX + (SQUARESIZE + 3) * _poSquare.x + (7 * (_poSquare.x / 3));
	_riY = GRIDDOFSETY + (SQUARESIZE + 3) * _poSquare.y + (7 * (_poSquare.y / 3));
}

void CGame::LoadRandomSetup(bool _bLoadFirst)
{
	m_iBoardCount = 0;
	std::ifstream myfile("Data\\Setups.txt");
	std::string strLine;

	if(myfile.is_open())
	{
		// number of setups in file
		std::getline(myfile, strLine);

		int iSetupCount = atoi(strLine.c_str());
		int iRand = (iSetupCount <= 1 ? iSetupCount - 1: rand() % iSetupCount);

		if(_bLoadFirst)
		{
			iRand = 0;
		}

		// blank lines (get to the next setup
		for(int i = 0; i < iRand * 10 + 1; ++i)
		{
			std::getline(myfile, strLine);
		}

		for(int i = 0; i < 9; ++i)
		{
			std::getline(myfile, strLine);

			for(unsigned int j = 0; j < 9; ++j)
			{
				std::string str = strLine.substr(j, 1);
				m_iBoard[j][i] = atoi(str.c_str());

				if(m_iBoard[j][i] != 0)
				{
					m_iBoardCount++;
				}
			}
		}

		myfile.close();
	}
} 

bool CGame::SolveSudoku()
{
	m_eStatus = COMPUTER_SOLVING;

	Render();

	int iBoard1D[81];

	for(int i = 0; i < 9; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			iBoard1D[i * 9 + j] = m_iBoard[j][i];
		}
	}

	// Initialize Lua
	m_pLuaState = luaL_newstate();

	// load Lua base libraries
	luaL_openlibs(m_pLuaState); 
 
	// Load the script 
	luaL_dofile(m_pLuaState, "Data\\Sudoku Solver.lua"); 
 
	// Call the add function 
	bool bResult = LuaFunction(iBoard1D);

	// Cleanup Lua 
	lua_close(m_pLuaState); 

	m_eStatus = PUZZLE_SOLVED;

	m_iBoardCount = 81;

	return(bResult);
}

bool CGame::LuaFunction(int _iBoard1D[81])
{
	lua_getglobal(m_pLuaState, "SudokuSolver");

	std::ostringstream ss;

	for(int i = 0; i < 81; i++) 
	{
		ss << _iBoard1D[i];
	}

	std::string s = ss.str();

	lua_pushstring(m_pLuaState, s.c_str());
	lua_call(m_pLuaState, 1, 1);

	std::string strReturn = (const char*)lua_tostring(m_pLuaState, -1);

	lua_pop(m_pLuaState, 1);

	if(strReturn == "FAIL")
	{
		return(false);
	}

	for(int i = 0; i < 81; i++) 
	{
		int iX = i / 9;
		int iY = i % 9;
		m_iBoard[iX][iY] = stoi(strReturn.substr(i, 1));
	}

	return(true);
}

void CGame::CheckCompletion(bool _bIncrement)
{
	if(_bIncrement)
	{
		++m_iBoardCount;
	}

	if(m_iBoardCount >= 81)
	{
		for(int i = 0; i < 9; ++i)
		{
			if(!CheckValidPlacementRowColumn(i, i))
			{
				return;
			}
		}

		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				if(!CheckValidPlacementSquare(i * 3, j * 3))
				{
					return;
				}
			}
		}

		MessageBox(NULL, L"Well done you have solved the Sudoku!", L"Congratulations", MB_OK);
	}
}

bool CGame::CheckValidPlacementRowColumn(int _iX, int _iY)
{
	bool bValidA[10];
	bool bValidB[10];	

	for(int j = 0; j < 10; ++j)
	{
		bValidA[j] = true;
		bValidB[j] = true;
	}

	for(int j = 0; j < 9; ++j)
	{
		if(!bValidA[m_iBoard[j][_iY]] || !bValidB[m_iBoard[_iX][j]])
		{
			return(false);
		}

		if(m_iBoard[j][_iY] != 0)
		{
			bValidA[m_iBoard[j][_iY]] = false;
		}

		if(m_iBoard[_iX][j] != 0)
		{
			bValidB[m_iBoard[_iX][j]] = false;
		}
	}

	return(true);
}

bool CGame::CheckValidPlacementSquare(int _iX, int _iY)
{
	bool bValidC[10];

	for(int j = 0; j < 10; ++j)
	{
		bValidC[j] = true;
	}

	int iSmallX = _iX - (_iX % 3);
	int iSmallY = _iY - (_iY % 3);

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(!bValidC[m_iBoard[iSmallX + i][iSmallY + j]])
			{
				return(false);
			}

			if(m_iBoard[iSmallX + i][iSmallY + j] != 0)
			{
				bValidC[m_iBoard[iSmallX + i][iSmallY + j]] = false;
			}
		}
	}

	return(true);
}

bool CGame::ChangeSquare(POINT _pSquare, int _iVal)
{
	if(m_eStatus == PLAYER_SOLVING)
	{
		for(int i = 0; i < 9; ++i)
		{
			for(int j = 0; j < 9; ++j)
			{
				if(m_iSetupBoard[_pSquare.x][_pSquare.y] != 0)
				{
					return(false);
				}
			}
		}
	}
	
	//if(m_eStatus == PUZZLE_SETUP)
	//{
		int iCurrentBoardVal = m_iBoard[_pSquare.x][_pSquare.y];
		m_iBoard[_pSquare.x][_pSquare.y] = _iVal;

		if(!CheckValidPlacementRowColumn(_pSquare.x, _pSquare.y))
		{
			m_iBoard[_pSquare.x][_pSquare.y] = iCurrentBoardVal;
			return(false);
		}

		if(!CheckValidPlacementSquare(_pSquare.x, _pSquare.y))
		{
			m_iBoard[_pSquare.x][_pSquare.y] = iCurrentBoardVal;
			return(false);
		}
	//}

	m_iBoard[_pSquare.x][_pSquare.y] = _iVal;
	return(true);
}

void CGame::FindNextNumber(POINT _poSquare)
{
	int iVal = m_iBoard[_poSquare.x][_poSquare.y];

	for(int i = iVal + 1; i < 10; ++i)
	{
		m_iBoard[_poSquare.x][_poSquare.y] = i;

		if(!CheckValidPlacementRowColumn(_poSquare.x, _poSquare.y))
		{
			m_iBoard[_poSquare.x][_poSquare.y] = iVal;
			continue;
		}

		if(!CheckValidPlacementSquare(_poSquare.x, _poSquare.y))
		{
			m_iBoard[_poSquare.x][_poSquare.y] = iVal;
			continue;
		}

		return;
	}

	m_iBoard[_poSquare.x][_poSquare.y] = 0;
}