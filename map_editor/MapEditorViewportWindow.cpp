/** \file MapEditorWindow.cpp */
#include "MapEditorViewportWindow.h"

#include "GLFW/glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#include "GLFW/glfw3native.h"


MapEditorViewportWindow::MapEditorViewportWindow(const OSWindow::Settings& settings)
{
	// Initialize GLFW
	debugAssert(glfwInit());

	// Set GLFW window hints
	glfwWindowHint(GLFW_RESIZABLE,				GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED,				GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED,				GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING,				GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,			GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API,				GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API,	GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,	GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_NO_ERROR,		GLFW_FALSE);

	// Create GLFW window
	m_glfwWin = std::make_unique<GLFWwindow>(glfwCreateWindow(800, 600, "Map Editor Viewport", NULL, NULL));
	debugAssert(m_glfwWin);
	
}


MapEditorViewportWindow::~MapEditorViewportWindow()
{
}