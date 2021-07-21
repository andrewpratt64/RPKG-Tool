/**
  \file MapEditorWindow.h

  Window for the map editor 3d viewport, utilizing both GLFW and Win32.
 */
#pragma once
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include "GLFW/glfw3native.h"
#include <G3D/G3D.h>
#include <memory>


 /**
  * @brief Window for the map editor 3d viewport
  * @author Andrew Pratt
  */
class MapEditorViewportWindow : public OSWindow
{
protected:
	/** Pointer to the GLFW window */
	std::unique_ptr<GLFWwindow> m_glfwWin;
	/** Handle to the window */
	HWND m_hWin;

public:
	
	// NOTE: At least for now, the settings argument is completely ignored
	MapEditorViewportWindow::MapEditorViewportWindow(const OSWindow::Settings& settings);
	MapEditorViewportWindow::~MapEditorViewportWindow();
};

