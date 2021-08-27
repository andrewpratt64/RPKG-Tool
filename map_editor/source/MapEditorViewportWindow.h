/**
  \file MapEditorWindow.h

  Window for the map editor 3d viewport, utilizing both GLFW and Win32.
 */
#pragma once
#include <memory>
#include <G3D/G3D.h>
#include "GLFW/glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#include "GLFW/glfw3native.h"
#include <windows.h>


 /**
  * @brief Window for the map editor 3d viewport
  * @author Andrew Pratt
  */
class MapEditorViewportWindow : public OSWindow
{
protected:
	/** Pointer to the GLFW window */
	GLFWwindow* m_glfwWin;
	/** Handle to the window */
	HWND m_hWin;

public:

	// NOTE: At least for now, the settings argument is completely ignored
	MapEditorViewportWindow(const OSWindow::Settings& settings);
	~MapEditorViewportWindow();


	/**
	 * @brief Accessor for the viewport's window handle
	 *
	 * @return HWND to the window
	*/
	HWND getWin32Handle() const;


	// Copied from G3D::GLFWWindow::getMouseButtonState
	void getMouseButtonState(uint8& mouseButtons) const;


	// Inherited via OSWindow
	static MapEditorViewportWindow* create(const Settings& s = Settings());

	virtual void show() const override;
	virtual void setInputCapture(bool c) override;
	virtual void setMouseVisible(bool b) override;
	virtual void setAsCurrentGraphicsContext() const override;
	virtual void getSettings(OSWindow::Settings& settings) const override;
	virtual int width() const override;
	virtual int height() const override;
	virtual Rect2D fullRect() const override;
	virtual Rect2D clientRect() const override;
	virtual void setFullRect(const Rect2D& dims) override;
	virtual void setClientRect(const Rect2D& dims) override;
	virtual void getDroppedFilenames(Array<String>& files) override;
	virtual void setFullPosition(int x, int y) override;
	virtual void setClientPosition(int x, int y) override;
	virtual bool hasFocus() const override;
	virtual String getAPIVersion() const override;
	virtual String getAPIName() const override;
	virtual String className() const override;
	virtual void setGammaRamp(const Array<uint16>& gammaRamp) override;
	virtual void setCaption(const String& caption) override;
	virtual int numJoysticks() const override;
	virtual String joystickName(unsigned int sticknum) const override;
	virtual String caption() override;
	virtual void swapGLBuffers() override;
	virtual void setRelativeMousePosition(double x, double y) override;
	virtual void setRelativeMousePosition(const Vector2& p) override;
	virtual void getRelativeMouseState(Vector2& position, uint8& mouseButtons) const override;
	virtual void getRelativeMouseState(int& x, int& y, uint8& mouseButtons) const override;
	virtual void getRelativeMouseState(double& x, double& y, uint8& mouseButtons) const override;
	virtual void getJoystickState(unsigned int stickNum, Array<float>& axis, Array<bool>& button) const override;
	virtual String _clipboardText() const override;
	virtual void _setClipboardText(const String& text) const override;
};

