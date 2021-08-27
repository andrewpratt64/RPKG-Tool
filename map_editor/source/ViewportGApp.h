/**
  \file ViewportGApp.h

  Subclass of G3D::GApp for the 3d viewport
 */
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <G3D/G3D.h>
#include <string>
#include <string_view>
#include <filesystem>
#include <windows.h>
//#include <commctrl.h>
#include <shellapi.h>

#include "GLFW/glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#include "GLFW/glfw3native.h"

#include "MapEditorViewportWindow.h"


 /**
  * @brief Viewport app
  * @author Andrew Pratt
  */
class ViewportGApp : public GApp {
private:
    /** Handle to the main app window */
    HWND m_hAppWin;

    /** Thread for running the viewport's main loop */
    std::thread m_viewportThread;

    /** Raw pointer to the viewport g3d window */
    MapEditorViewportWindow* m_viewportWin;
    /** Raw pointer to the current scene for convenience */
    Scene* m_scene;

    /** Fully qualified path to the running exe (for the viewport process) */
    std::filesystem::path m_exePath;

protected:

    /** Called from onInit */
    void makeGUI();


    /** @brief Initializes an empty scene */
    void initScene();


    /** @brief Click handler for add cube button */
    void onClick_AddCube();

    /** @brief Handler for when window is resized */
    void onResizeWindow(const GEvent& event);


public:
    // Constructor
    ViewportGApp(HWND hAppWin, std::filesystem::path&& exePath, const GApp::Settings& settings = GApp::Settings());


    /** @brief Wait for the viewport thread to stop */
    void waitForViewportToStop();


    /**
     * @brief Get a unique version of a desired entity name, if it isn't already unique
     *
     * @param name Desired entity name
     * @return Desired name of the entity, which may be modified to ensure it's unique
     * @warning Assumes m_scene has been initialized and is valid
     *
     * @throw std::overflow_error If too many entities have the same desired name, probably will never happen unless you have
     *      over 18,446,744,073,709,551,615 entities with the same name
     */
    G3D::String getUniqueEntityName(const G3D::String& name) const;

    // Method overrides
#pragma region overrides
    // Basically overrides of non-virtual methods GApp::run() and GApp::onRun(), respectively
    int runViewport();
    void onRunViewport();

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(Array<shared_ptr<Surface> >& posed3D, Array<shared_ptr<Surface2D> >& posed2D) override;

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;

    //virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;
#pragma endregion
};
