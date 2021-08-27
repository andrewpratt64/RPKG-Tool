/** \file main.cpp */
// Andrew Pratt 2021
// Map Editor for Hitman 3

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <windows.h>

#include "MapEditorApp.h"


// Entry point for application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, PWSTR cmdline, int cmdShow)
{
	// Create and run the editor app, then return it's exit code
	return static_cast<int>(MapEditor::MapEditorApp{ hInstance }.run());
}