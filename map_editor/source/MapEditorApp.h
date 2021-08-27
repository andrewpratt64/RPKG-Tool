/**
  \file MapEditorApp.h

  Main Win32 app for the map editor
 */
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <string>
#include <string_view>
#include <filesystem>
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>

#include "str_maybe_unicode.h"
#include "ViewportGApp.h"

namespace MapEditor
{
    /**
     * @brief Main app for the map editor. Contains main win32 window, and the g3d viewport window
     * @author Andrew Pratt
     */
    class MapEditorApp
    {
    public:
        // Window property name for the MapEditorApp instance a window is associated with
        static constexpr auto WIN_PROP_NAME_INSTANCE_PTR{ StrMaybeUnicode("PROP_MAP_EDITOR_APP_INSTANCE_PTR") };
        // Environment variable name for the parent window's class name
        static constexpr auto WIN_ENVIRO_VAR_NAME_PARENT_HWND{ StrMaybeUnicode("MAP_EDITOR_PARENT_HWND") };
        // Environment variable name for the fully qualified path to the running exe's parent directory
        static constexpr auto WIN_ENVIRO_VAR_NAME_EXE_DIR{ StrMaybeUnicode("MAP_EDITOR_EXE_DIR") };
        // Environment variable name for the length of the value of WIN_ENVIRO_VAR_NAME_EXE_DIR in characters
        static constexpr auto WIN_ENVIRO_VAR_NAME_EXE_DIR_LEN{ StrMaybeUnicode("MAP_EDITOR_EXE_DIR_LEN") };
        // Name of the main window class
        static constexpr auto MAIN_CLASS_NAME{ L"MapEditorMainWin" };
        // Name of the exe file for the viewport process
        static constexpr auto EXE_NAME_VIEWPORT{ StrMaybeUnicode("map_editor_viewport.exe") };


    private:

        /** Handle to the running instance */
        HINSTANCE m_hInstance{ NULL };
        /** Handle to the main app window */
        HWND m_hAppWin{ NULL };
        /** Handle to the icon for the main app window */
        HICON m_hAppWinIcon{ NULL };
        /** Handle to the window header */
        HWND m_hHeader{ NULL };

        /** Fully qualified path to the running exe */
        std::filesystem::path m_exePath;


        /** G3D viewport child process info */
        PROCESS_INFORMATION m_viewportAppProc;
        /** Return value from running the viewport sub-application */
        int m_viewportReturnVal;


    public:
        MapEditorApp(HINSTANCE hInstance);
        virtual ~MapEditorApp();


        /**
         * @brief Get the instance of an app from a window
         *
         * @param hAppWin Handle to the main window of a map editor app
         * @return Pointer to the MapEditorApp instance the window belongs to, or nullptr if no app was found
         */
        static MapEditorApp* getAppInstance(const HWND hAppWin);


        /**
         * @brief Run the application and wait for it to finish
         *
         * @return Exit code
         */
        WPARAM run();

        /**
         * @brief Displays an error message for the calling thread's last error
         * 
         * @return True if error message was displayed
         */
        bool showLastErrorMsg();


    protected:
        /** @brief Method called right before the window is destroyed */
        void onCleanup();

        /** @brief Window procedure function for the main window */
        static LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        /**
         * @brief Start the message loop for the main window and wait for it to quit
         *
         * @return Exit code
         */
        WPARAM startMainLoop();


        /** @brief Asynchronously creates and starts the editor viewport */
        void startViewport();

        /**
         * @brief Waits for the editor viewport to stop running
         *
         * @return The return value of the G3D application
         */
        int waitForViewportToStop();


#pragma region Win32Gui

    protected:

        /**
         * @brief Setup all win32-related gui
         *
         * @return True if everything was setup correctly, false otherwise
        */
        bool setupWin32Gui();

        /**
         * @brief Create the app's main window
         *
         * @return HWND to the window
        */
        HWND createWin32MainWindow();

        /**
         * @brief Create a header control for the window
         *
         * @param hParent Handle to the parent of the header
         * @return HWND to the header
        */
        HWND createWin32Header(HWND hParent);

        /**
         * @brief Insert an item into the header control
         *
         * @param hHeader Handle to the header control
         * @param insertAfter Index of the item to insert the new item after
         * @param w Width of the new item
         * @param Address
         * @return Index of the inserted item
        */
        int win32HeaderInsertItem(HWND hHeader, const int insertAfter, const int w, LPTSTR lpsz);

#pragma endregion
    };
}