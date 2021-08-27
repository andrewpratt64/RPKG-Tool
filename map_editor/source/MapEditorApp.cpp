#include "MapEditorApp.h"
#include <sstream>
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <cassert>
#include <strsafe.h>

// TODO: throw "ERROR";    <- this is lazy

using namespace MapEditor;



MapEditorApp::MapEditorApp(HINSTANCE hInstance) :
    // Init handle to instance
    m_hInstance{hInstance}
{
    // Make sure handle to instance is valid
    assert(m_hInstance);

    // Get the path to the running exe
    TCHAR moduleFileNameStr[MAX_PATH];
    ZeroMemory(moduleFileNameStr, MAX_PATH);
    auto moduleFileNameStrLen{ GetModuleFileName(NULL, moduleFileNameStr, MAX_PATH) };
    if (moduleFileNameStrLen == 0)
    {
        showLastErrorMsg();
        throw "ERROR";
    }
    m_exePath = moduleFileNameStr;

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
    // Create and start the 3d viewport
    startViewport();

    // Enter main message loop and get it's exit code when it's finished
    WPARAM exitCode{ startMainLoop() };

    // Wait for the viewport to stop
    waitForViewportToStop();

    // Return exit code
    return exitCode;
}


bool MapEditor::MapEditorApp::showLastErrorMsg()
{
    // Get the last error code
    DWORD errCode{ GetLastError() };

    // Allocate a string buffer to hold the error message
    LPTSTR lpErrMsgBuffer{ nullptr };

    // Get the formatted error message
    if (FormatMessage(
        // Formatting flags
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        // Ignored argument
        NULL,
        // Error code to format
        errCode,
        // Language to use
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        // Buffer to store formatted string in
        reinterpret_cast<LPTSTR>(&lpErrMsgBuffer),
        // Min number of chars to allocate
        0,
        // Ignored argument
        NULL
    ) == 0)
    {
        // Display a less-helpful error if formatting the error message failed
        std::wostringstream errMsgStream;
        errMsgStream << L"Error code " << errCode << L". Failed to get formatted error message, raised error " << GetLastError() << L" while formatting";
        //std::wstring errMsg;
        //errMsgStream.str(errMsg);
        MessageBox(NULL, static_cast<LPCWSTR>(errMsgStream.str().c_str()), L"ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    }
    else
        // Display error message
        MessageBox(NULL, static_cast<LPCWSTR>(lpErrMsgBuffer), L"ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);

    

    return true;
}


WPARAM MapEditorApp::startMainLoop()
{
    setupWin32Gui();

    MSG msg;
    BOOL bReturn;

    // Get the next message and loop while the app is still running
    while ((bReturn = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        // Handle errors
        if (bReturn == -1)
        {
            showLastErrorMsg();
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
    // Set an environment variable so the viewport knows the class name of the main parent window
    //SetEnvironmentVariable(WIN_ENVIRO_VAR_NAME_PARENT_HWND, WIN_PROP_NAME_INSTANCE_PTR);

    // Set environment variables so the viewport knows the parent directory of the exe
    auto exePath{ m_exePath.parent_path() };
    if (SetEnvironmentVariable(WIN_ENVIRO_VAR_NAME_EXE_DIR, static_cast<LPCWSTR>(exePath.c_str())) == 0)
    {
        showLastErrorMsg();
        throw "ERROR";
    }

    // Setup an instance of the startup info structure
    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    // Size of startup info in memory
    startupInfo.cb = sizeof(startupInfo);

    // Setup an instance of the proccess info structure
    ZeroMemory(&m_viewportAppProc, sizeof(m_viewportAppProc));

    // Start the child process
    if (!CreateProcess(
        // Process module name
        (m_exePath.parent_path() / EXE_NAME_VIEWPORT).wstring().c_str(),
        // Command line
        NULL,
        // Process handle inheritance
        NULL,
        // Thread handle inheritance
        NULL,
        // Inherit all possible handles?
        true,
        // Creation flags
        0,
        // Environment block (NULL uses parent's)
        NULL,
        // Starting directory (NULL uses parent's)
        NULL,
        // Startup info
        &startupInfo,
        // Process info
        &m_viewportAppProc
    ))
    {
        // Bail if process failed to start
        showLastErrorMsg();
        throw "ERROR";
    }


}


int MapEditorApp::waitForViewportToStop()
{
    // Wait for the viewport process to stop
    WaitForSingleObject(m_viewportAppProc.hProcess, INFINITE);

    // Close the process handles
    CloseHandle(m_viewportAppProc.hProcess);
    CloseHandle(m_viewportAppProc.hThread);
    
    // Return the app's return value
    //return m_viewportReturnVal;
    return EXIT_SUCCESS;
}


bool MapEditorApp::setupWin32Gui()
{
    // Make sure the calling thread is a GUI thread
    assert(IsGUIThread(true) != ERROR_NOT_ENOUGH_MEMORY);

    // Setup common controls from Comctl32.dll
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
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
    wcxAppMain.lpszClassName = MAIN_CLASS_NAME;
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
        MAIN_CLASS_NAME,
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