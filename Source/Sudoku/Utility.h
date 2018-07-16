#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

#define WIN32_LEAN_AND_MEAN

// Includes
#include <Windows.h>
#include <WindowsX.h>
#include <time.h>
#include <cstdlib>
#include <vld.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// Lua Includes
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

// Enumerations
enum STATUS
{
	PUZZLE_SETUP,
	PLAYER_SOLVING,
	COMPUTER_SOLVING,
	PUZZLE_SOLVED
};

// Structs

// Defined Functions
#define ReleaseCOM(x) { if(x) { x->Release(); x = nullptr; } }
#define DeleteOBJ(x) {if(x) { delete x; x = nullptr; } }
#define RandomFloat() (rand() / (RAND_MAX + 1.0f))

// Defined Values
#define SQUARESIZE 46
#define HALFSQUARESIZE SQUARESIZE / 2
#define GRIDOFFSETX 104
#define GRIDDOFSETY 94

#endif // UTILITY_H