#include "MapEditorApp.h"
#include <cassert>



MapEditorApp::MapEditorApp(HINSTANCE hInstance) :
    // Init handle to instance
    m_hInstance{hInstance}
{
    // Make sure handle to instance is valid
    assert(m_hInstance);

    // Create main window
    createWin32MainWindow();
    // Make sure main window is valid (so that intellisense will stop yelling at
    //  me when I use SetProp)
    assert(m_hAppWin != 0);

    // Create custom window properties
    SetProp(m_hAppWin, MapEditorApp::WIN_PROP_NAME_INSTANCE_PTR, this);
}


MapEditorApp::~MapEditorApp()
{
}


MapEditorApp* MapEditorApp::getAppInstance(const HWND hAppWin)
{
    // Attempt to get a handle to the MapEditorAppInstance
    HANDLE hAppInstance{ GetProp(hAppWin, MapEditorApp::WIN_PROP_NAME_INSTANCE_PTR) };

    // If the handle is null, return a null pointer
    if (hAppInstance == NULL)
        return nullptr;

    // If the handle is NOT null, cast it to a MapEditorApp and return it
    return reinterpret_cast<MapEditorApp*>(hAppInstance);
}


void MapEditorApp::onCleanup()
{
    // Delete window properties
    RemoveProp(m_hAppWin, MapEditorApp::WIN_PROP_NAME_INSTANCE_PTR);

    // Unload resources
    DestroyIcon(m_hAppWinIcon);
}


LRESULT MapEditorApp::MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // Window is going to be destroyed
    case WM_DESTROY:
    {
        // Perform cleanup, if this is a valid map editor app
        auto appInstance{ MapEditorApp::getAppInstance(hwnd) };
        if (appInstance)
            appInstance->onCleanup();
        // Indicate we are quiting the current thread
        PostQuitMessage(0);
    }


    // Window is activated
    case WM_ACTIVATE:
        // Focus the window
        SetFocus(hwnd);
        return 0;

    // Window's client area needs to be repainted
    case WM_PAINT:
    {
        // Begin painting
        PAINTSTRUCT ps;
        HDC hdc{ BeginPaint(hwnd, &ps) };

        // Fill with a solid color
        FillRect(hdc, &ps.rcPaint, static_cast<HBRUSH>(GetStockObject(GetSysColor(COLOR_WINDOW))));

        // Finish painting
        EndPaint(hwnd, &ps);

        return 0;
    }

    // Default
    default:
        // Run the default window procedure
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


WPARAM MapEditorApp::run()
{
    WPARAM exitCode;
    std::thread foo{ [this, &exitCode]() {exitCode = startMainLoop(); } };
    startViewport();
    foo.join();

    // Create and start the 3d viewport
    //startViewport();

    // Enter main message loop and get it's exit code when it's finished
    //WPARAM exitCode{ startMainLoop() };

    // Wait for the viewport to stop
    waitForViewportToStop();

    // Return exit code
    return exitCode;
}


WPARAM MapEditorApp::startMainLoop()
{
    MSG msg;
    BOOL bReturn;

    // Get the next message and loop while the app is still running
    while ((bReturn = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        // Handle errors
        if (bReturn == -1)
        {
            throw "ERROR";
        }
        // Handle messages
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Return the exit code
    return msg.wParam;
}


void MapEditorApp::startViewport()
{
    //  == App Settings ==
    ViewportGApp::Settings settings(NULL, NULL);


    // Window caption
    settings.window.caption = "Map Editor";


    // Set enable to catch more OpenGL errors
    settings.window.debugContext = true;


    // Window size
    if (settings.window.fullScreen)
    {
        settings.window.width = 1920;
        settings.window.height = 1080;
    }
    else
    {
        settings.window.height = int(OSWindow::primaryDisplayWindowSize().y * 0.95f);
        // Constrain ultra widescreen aspect ratios
        settings.window.width = min(settings.window.height * 1920 / 1080, int(OSWindow::primaryDisplayWindowSize().x * 0.95f));

        // Make even
        settings.window.width -= settings.window.width & 1;
        settings.window.height -= settings.window.height & 1;
    }
    settings.window.resizable = !settings.window.fullScreen;
    settings.window.framed = settings.window.resizable;


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


    // Directory containing map editor-specific data
    settings.dataDir = FilePath::concat(FileSystem::currentDirectory(), "data-files");

    // Window icon
    settings.window.defaultIconFilename = "data-files/icons/icon.png";


    // Setup screen capture
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
    settings.window.framed = true;

    settings.window.hardware = true;
    settings.window.sharedContext = false;
    settings.window.stencilBits = 0;
    settings.window.stereo = false;

    settings.useDeveloperTools = true;

    // Create and run the viewport app
    m_viewportApp = std::make_unique<ViewportGApp>(m_hAppWin, settings);
    m_viewportApp->runViewport();
}


int MapEditorApp::waitForViewportToStop()
{
    // Wait for the G3D app to finish
    m_viewportApp->waitForViewportToStop();
    // Return the app's return value
    //return m_viewportReturnVal;
    return EXIT_SUCCESS;
}


bool MapEditorApp::setupWin32Gui()
{
    // Make sure the calling thread is a GUI thread
    assert(IsGUIThread(true) != ERROR_NOT_ENOUGH_MEMORY);

    // Setup common controls from Comctl32.dll
    InitCommonControlsExStruct icex;
    icex.dwSize = sizeof(InitCommonControlsExStruct);
    icex.dwICC = ICC_WIN95_CLASSES;
    return InitCommonControlsEx(&icex);;
}


HWND MapEditorApp::createWin32MainWindow()
{
    // Load the window icon from a file
    m_hAppWinIcon = ExtractIcon(nullptr, L"data-files/icons/map_editor.ico", 0);

    // Declare main window class
    WNDCLASSEX wcxAppMain;
    // Class size
    wcxAppMain.cbSize = sizeof(wcxAppMain);
    // Class style
    wcxAppMain.style = CS_HREDRAW | CS_VREDRAW /* | CS_PARENTDC*/;
    // Function pointer to winproc
    wcxAppMain.lpfnWndProc = MainWinProc;
    // Extra class size
    wcxAppMain.cbClsExtra = 0;
    // Extra instance size
    wcxAppMain.cbWndExtra = 0;
    // Handle to instance containing winproc
    wcxAppMain.hInstance = m_hInstance;
    // Window icon
    wcxAppMain.hIcon = m_hAppWinIcon;
    // Cursor
    wcxAppMain.hCursor = LoadCursor(NULL, IDC_ARROW);
    // Window background
    wcxAppMain.hbrBackground = static_cast<HBRUSH>(GetStockObject(GetSysColor(COLOR_WINDOW)));
    // Menu resource name
    wcxAppMain.lpszMenuName = L"MapEditorMainWinMenu";
    // Window class name
    wcxAppMain.lpszClassName = L"MapEditorMainWin";
    // Small window icon
    // TODO: Don't use the same icon as the large window icon
    wcxAppMain.hIconSm = m_hAppWinIcon;

    // Register main window class
    RegisterClassEx(&wcxAppMain);

    // Create an instance of the main window class
    m_hAppWin = CreateWindowEx(
        // Extended window style
        0,
        // Window class
        L"MapEditorMainWin",
        // Window name
        L"Map Editor",
        // Window style
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE,
        // X, Y, Width, Height
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        // Window parent
        NULL,
        // Menu
        NULL,
        // Module
        GetModuleHandle(NULL),
        // Win32 lpParam
        NULL
    );

    // Finish setting up and showing window
    ShowWindow(m_hAppWin, SWP_SHOWWINDOW);
    UpdateWindow(m_hAppWin);

    // Return the main window
    return m_hAppWin;
}


HWND MapEditorApp::createWin32Header(HWND hParent)
{
    // Declare the child window identifier for the header window
    long long ID_HEADER{ -1 };

    // Create the header window
    HWND hHeader{ CreateWindowEx(
        // Extended window style
        0,
        // Window class
        WC_HEADER,
        // Window name
        LPCTSTR{NULL},
        // Window style
        WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
        // X, Y, Width, Height
        0, 0, 0, 0,
        // Window parent
        hParent,
        // Child window identifier
        reinterpret_cast<HMENU>(ID_HEADER),
        // Module
        GetModuleHandle(NULL),
        // Win32 lpParam
        NULL
    ) };

    // Return null if the header window wasn't created
    if (hHeader == NULL)
        return HWND{ NULL };

    // Get the correct header layout
    HDLAYOUT layout{};
    WINDOWPOS headerPos{};
    layout.pwpos = &headerPos;
    RECT rcParent{};
    GetClientRect(hParent, &rcParent);
    layout.prc = &rcParent;
    Header_Layout(hHeader, &layout);

    // Set the header position/scale/etc.
    // Return null if it couldn't be set
    if (SetWindowPos(
        hHeader,
        layout.pwpos->hwndInsertAfter,
        layout.pwpos->x,
        layout.pwpos->y,
        layout.pwpos->cx,
        layout.pwpos->cy,
        (layout.pwpos->flags & ~SWP_DRAWFRAME) | SWP_SHOWWINDOW
    ) == NULL)
        return HWND{ NULL };

    // Return a HWND to the header
    return hHeader;
}


int MapEditorApp::win32HeaderInsertItem(HWND hHeader, const int insertAfter, const int w, LPTSTR lpsz)
{
    // Declare the new header item
    HDITEM newItem;

    // Setup item
    newItem.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
    newItem.cxy = w;
    newItem.pszText = lpsz;
    newItem.cchTextMax = sizeof(newItem.pszText) / sizeof(newItem.pszText[0]);
    newItem.fmt = HDF_CENTER | HDF_STRING;

    // Insert item
    return static_cast<int>(SendMessage(hHeader, HDM_INSERTITEM, static_cast<WPARAM>(insertAfter), reinterpret_cast<LPARAM>(&newItem)));
}