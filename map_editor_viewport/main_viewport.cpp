/** \file main_viewport.cpp */
// Andrew Pratt 2021
// Map Editor for Hitman 3 - Viewport child process

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <string>
#include <locale>
#include <codecvt>
#include <stdlib.h>
#include <windows.h>

//#include "MapEditorApp.h"
#include "ViewportGApp.h"


// Entry point for child process
//int APIENTRY WinMainViewport(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR cmdline, int cmdShow)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, PWSTR cmdline, int cmdShow)
{
    msgBox("jsdbvoudv");
    //  == App Settings ==
    ViewportGApp::Settings settings(NULL, NULL);


    // Window caption
    settings.window.caption = "Map Editor";


    // Set enable to catch more OpenGL errors
    settings.window.debugContext = true;


    // Window size
    settings.window.fullScreen = false;
    // TODO: Don't hardcode dimensions
    settings.window.width = 1920 / 10;
    settings.window.height = 1080 / 10;
    settings.window.resizable = false;


    // Set to true for a significant performance boost if your app can't
    // render at the display frequency, or if you *want* to render faster
    // than the display.
    settings.window.asynchronous = true;


    // Render slightly larger than the screen so that screen-space refraction, bloom,
    // screen-space AO, and screen-space reflection to look good at screen edges. Set to zero for
    // maximum performance and free memory. Increase the second argument to improve AO without affecting
    // color. The third argument is the resolution. Set to 0.5f to render at half-res and upscale,
    // 2.0f to supersample.
    settings.hdrFramebuffer.setGuardBandsAndSampleRate(0, 0, 1.0f);
    settings.renderer.deferredShading = false;
    settings.renderer.orderIndependentTransparency = true;

    // Get the path of the running exe for the viewport
    // TODO: Better error handling(?)
    // TODO: Delete buffers after use(?)
    TCHAR exePathWBuffer[MAX_PATH]{};
    ZeroMemory(exePathWBuffer, MAX_PATH);
    GetEnvironmentVariable(L"MAP_EDITOR_EXE_DIR", exePathWBuffer, MAX_PATH);
    char exePathBuffer[MAX_PATH];
    debugAssert(wcstombs(exePathBuffer, exePathWBuffer, sizeof(exePathWBuffer)) != 0);
    std::filesystem::path exePath{ exePathBuffer };
    settings.dataDir = G3D::String{ (exePath / "data-files").string() };
    //settings.dataDir = FilePath::concat(FileSystem::currentDirectory(), "data-files");

    // Window icon
    settings.window.defaultIconFilename = "data-files/icons/icon.png";


    // Setup screen capture
    // TODO: Use correct directory, i.e. NOT FileSystem::currentDirectory()
    settings.screenCapture.outputDirectory = FilePath::concat(FileSystem::currentDirectory(), "../journal");
    if (!FileSystem::exists(settings.screenCapture.outputDirectory))
    {
        settings.screenCapture.outputDirectory = "";
    }
    settings.screenCapture.includeAppRevision = true;
    settings.screenCapture.includeG3DRevision = false;
    settings.screenCapture.filenamePrefix = "G2MapMaker_";


    // Window should not be visible at first
    settings.window.visible = false;

    // Window frame
    settings.window.framed = false;

    settings.window.hardware = true;
    settings.window.sharedContext = false;
    settings.window.stencilBits = 0;
    settings.window.stereo = false;

    settings.useDeveloperTools = true;
    
    // Attempt to get the main window of the parent process
    //HWND hParent{ FindWindow(MapEditor::MapEditorApp::MAIN_CLASS_NAME, NULL) };
    HWND hParent{ FindWindow(L"MapEditorMainWin", NULL)};

    // Bail if we couldn't get the parent window
    if (hParent == NULL)
        return EXIT_FAILURE;

    // Create and run the viewport app
    return ViewportGApp(hParent, std::move(exePath), settings).runViewport();
    //ViewportGApp gApp{ hParent, std::move(exePath), settings };
    //std::thread{ [&gApp]() { gApp.runViewport(); } }.join();

    return EXIT_SUCCESS;
}