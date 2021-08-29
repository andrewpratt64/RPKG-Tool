#pragma once
/** \file MapEditorViewportWindowUserData.h */
// Damn these file names are getting long :/

class MapEditorViewportWindow;


/** Structure of GLFW user data */
// See for more info: https://www.glfw.org/docs/latest/window_guide.html#window_userptr
struct MapEditorViewportWindowUserData
{
	/** Pointer to the owning MapEditorViewportWindow object */
	MapEditorViewportWindow* m_owner;
};