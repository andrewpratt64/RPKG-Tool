/** \file MapEditorWindow.cpp */
#include "MapEditorViewportWindow.h"


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
	glfwWindowHint(GLFW_VISIBLE,				GLFW_FALSE);

	// Create GLFW window
	//m_glfwWin = std::make_shared<GLFWwindow>(glfwCreateWindow(800, 600, "Map Editor Viewport", NULL, NULL));
	m_glfwWin = glfwCreateWindow(settings.width, settings.height, "Map Editor Viewport", NULL, NULL);
	debugAssert(m_glfwWin);
	// Get the win32 handle of the window
	m_hWin = glfwGetWin32Window(m_glfwWin);
	debugAssert(m_hWin != NULL);

	// Configure context
	setAsCurrentGraphicsContext();

	// Set vsync interval
	// Should probably be zero if using async rendering
	glfwSwapInterval(0);

	GLCaps::init(settings.api);
}


MapEditorViewportWindow::~MapEditorViewportWindow()
{
	// Stop GLFW
	glfwTerminate();
}


HWND MapEditorViewportWindow::getWin32Handle() const
{
	return m_hWin;
}


void MapEditorViewportWindow::getMouseButtonState(uint8& mouseButtons) const {
	mouseButtons = 0;
	int shift = 0;
	for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_8; ++i) {
		if (glfwGetMouseButton(m_glfwWin, i) == GLFW_PRESS) {
			// Center and right(buttons 2 and 3) have swapped order in G3D
			if (i == GLFW_MOUSE_BUTTON_2) ++shift;
			if (i == GLFW_MOUSE_BUTTON_3) --shift;
			mouseButtons |= (1 << shift);
			if (i == GLFW_MOUSE_BUTTON_2) --shift;
			if (i == GLFW_MOUSE_BUTTON_3) ++shift;
		}
		++shift;
	}
}


MapEditorViewportWindow* MapEditorViewportWindow::create(const Settings& s)
{
	MapEditorViewportWindow* newWin{ new MapEditorViewportWindow(s) };
	return newWin;
}


void MapEditorViewportWindow::show() const
{
	glfwShowWindow(m_glfwWin);
}


void MapEditorViewportWindow::setInputCapture(bool c)
{
	if (c)
		// If c is true, hide the cursor and lock it to the center of the window
		glfwSetInputMode(m_glfwWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// If c is false, have the cursor behave normally
		glfwSetInputMode(m_glfwWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void MapEditorViewportWindow::setMouseVisible(bool b)
{
	if (b)
		// If b is true, have the cursor behave normally
		glfwSetInputMode(m_glfwWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		// If b is false, hide the cursor
		glfwSetInputMode(m_glfwWin, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}


void MapEditorViewportWindow::setAsCurrentGraphicsContext() const
{
	// Configure OpenGL context
	glfwMakeContextCurrent(m_glfwWin);
}


void MapEditorViewportWindow::getSettings(OSWindow::Settings& settings) const
{
	settings = m_settings;
}


int MapEditorViewportWindow::width() const
{
	int v;
	glfwGetFramebufferSize(m_glfwWin, &v, NULL);
	return v;
}


int MapEditorViewportWindow::height() const
{
	int v;
	glfwGetFramebufferSize(m_glfwWin, NULL, &v);
	return v;
}


Rect2D MapEditorViewportWindow::fullRect() const
{
	RECT v;
	GetWindowRect(m_hWin, &v);
	return Rect2D::xyxy(
		static_cast<float>(v.left),
		static_cast<float>(v.top),
		static_cast<float>(v.right),
		static_cast<float>(v.bottom)
	);
}


Rect2D MapEditorViewportWindow::clientRect() const
{
	RECT v;
	GetClientRect(m_hWin, &v);
	return Rect2D::xyxy(
		static_cast<float>(v.left),
		static_cast<float>(v.top),
		static_cast<float>(v.right),
		static_cast<float>(v.bottom)
	);
}


void MapEditorViewportWindow::setFullRect(const Rect2D& dims)
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::setClientRect(const Rect2D& dims)
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::getDroppedFilenames(Array<String>& files)
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::setFullPosition(int x, int y)
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::setClientPosition(int x, int y)
{
	// Unimplemented
	return;
}


bool MapEditorViewportWindow::hasFocus() const
{
	return GetFocus() == m_hWin;
}


String MapEditorViewportWindow::getAPIVersion() const
{
	return String{ glfwGetVersionString() };
}


String MapEditorViewportWindow::getAPIName() const
{
	return "GLFW";
}


String MapEditorViewportWindow::className() const
{
	return String{"GLFWWindow"};
}


void MapEditorViewportWindow::setGammaRamp(const Array<uint16>& gammaRamp)
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::setCaption(const String& caption)
{
	// Unimplemented
	return;
}


int MapEditorViewportWindow::numJoysticks() const
{
	// Unimplemented
	return 0;
}

String MapEditorViewportWindow::joystickName(unsigned int sticknum) const
{
	// Unimplemented
	return "UNIMPLEMENTED";
}


String MapEditorViewportWindow::caption()
{
	return String{""};
}

void MapEditorViewportWindow::swapGLBuffers()
{
	glfwSwapBuffers(m_glfwWin);
}


void MapEditorViewportWindow::setRelativeMousePosition(double x, double y)
{
	glfwSetCursorPos(m_glfwWin, x, y);
}


void MapEditorViewportWindow::setRelativeMousePosition(const Vector2& p)
{
	glfwSetCursorPos(m_glfwWin, p.x, p.y);
}


void MapEditorViewportWindow::getRelativeMouseState(Vector2& position, uint8& mouseButtons) const
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::getRelativeMouseState(int& x, int& y, uint8& mouseButtons) const
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::getRelativeMouseState(double& x, double& y, uint8& mouseButtons) const
{
	// Unimplemented
	return;
}


void MapEditorViewportWindow::getJoystickState(unsigned int stickNum, Array<float>& axis, Array<bool>& button) const
{
	// Unimplemented
	return;
}


String MapEditorViewportWindow::_clipboardText() const
{
	// Unimplemented
	return String{""};
}


void MapEditorViewportWindow::_setClipboardText(const String& text) const
{
	// Unimplemented
	return;
}