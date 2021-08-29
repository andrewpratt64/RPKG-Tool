/** \file ViewportGApp.cpp */
#include "ViewportGApp.h"

#include <string>
#include <sstream>
#include <windows.h>
//#include <commctrl.h>

#include "MapEditorViewportWindow.h"


ViewportGApp::ViewportGApp(HWND hAppWin, std::filesystem::path&& exePath, const ViewportGApp::Settings& settings) :
    GApp(settings, (m_viewportWin = MapEditorViewportWindow::create(settings.window)), new RenderDevice{}, true),
    m_hAppWin{ hAppWin },
    m_scene{ nullptr },
    m_exePath{ exePath }
{
    // Catch exceptions
    catchCommonExceptions = true;
    // Let G3D take care of viewport input
    manageUserInput = true;

    // Get the win32 handle to the viewport window
    HWND hViewportWin{ m_viewportWin->getWin32Handle() };

    // Parent the viewport window to the main window
    SetParent(hViewportWin, m_hAppWin);

    // Configure the viewport window's style
    SetWindowLong(hViewportWin, GWL_STYLE, WS_CHILD | (~(WS_TILEDWINDOW | WS_POPUP | WS_CAPTION)));

    // Configure the viewport window's position
    SetWindowPos(
        // Window to position
        hViewportWin,
        // Ignored argument,
        NULL,
        // X, y, width, height
        // TODO: Don't hardcode
        0, 0, 100, 100,
        // Flags
        SWP_NOZORDER
    );

    // Update the viewport window
    assert(UpdateWindow(m_viewportWin->getWin32Handle()));

    // Make sure the viewport window is the one g3d renders to
    renderDevice->setWindow(m_viewportWin);

    // Make sure the viewport can capture input
    m_viewportWin->setInputCapture(true);

    // Make the viewport visible
    m_viewportWin->show();
}


void ViewportGApp::waitForViewportToStop()
{
    // Wait for the viewport thread to return, if it's still running and valid
    if (m_viewportThread.joinable())
        m_viewportThread.join();
}


G3D::String ViewportGApp::getUniqueEntityName(const G3D::String& name) const
{
    // Get a list of all entity names
    Array<String> names;
    m_scene->getEntityNames(names);

    // If the desired name isn't already taken, return it
    if (names.findIndex(name) == -1)
        return name;

    // Declare a variable to hold the number that should be
    // appended to the end of the desired name
    size_t nameSuffix{ 0 };

    // Keep trying numbers until a unique name is found
    G3D::String newName;
    do
    {
        newName = name + G3D::String(std::to_string(nameSuffix++));
    } while (names.findIndex(newName) != -1);

    // Return the new name
    return newName;
}


int ViewportGApp::runViewport()
{
    // Copied from GApp::run()
    int ret = 0;
    if (catchCommonExceptions) {
        try {
            onRunViewport();
            ret = m_exitCode;
        }
        catch (const char* e) {
            alwaysAssertM(false, e);
            ret = -1;
        }
        catch (const Image::Error& e) {
            alwaysAssertM(false, e.reason + "\n" + e.filename);
            ret = -1;
        }
        catch (const String& s) {
            alwaysAssertM(false, s);
            ret = -1;
        }
        catch (const TextInput::WrongTokenType& t) {
            alwaysAssertM(false, t.message);
            ret = -1;
        }
        catch (const TextInput::WrongSymbol& t) {
            alwaysAssertM(false, t.message);
            ret = -1;
        }
        catch (const LightweightConduit::PacketSizeException& e) {
            alwaysAssertM(false, e.message);
            ret = -1;
        }
        catch (const ParseError& e) {
            alwaysAssertM(false, e.formatFileInfo() + e.message);
            ret = -1;
        }
        catch (const FileNotFound& e) {
            alwaysAssertM(false, e.message);
            ret = -1;
        }
        catch (const std::exception& e) {
            alwaysAssertM(false, e.what());
            ret = -1;
        }
    }
    else {
        onRunViewport();
        ret = m_exitCode;
    }

    return ret;
}


void ViewportGApp::onRunViewport()
{
    // Call pre-run code
    beginRun();
    debugAssertGLOk();

    do
    {
        oneFrame();
    } while (!m_endProgram);

    // Start running the viewport in a seperate thread
    /*m_viewportThread = std::thread{[this]() {
        do
        {
            oneFrame();
        } while (!m_endProgram);
    } };*/

    // Call post-run code
    endRun();
}


// Called before the application loop begins. Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void ViewportGApp::onInit()
{
    GApp::onInit();

    //setFrameDuration(1.0f / 240.0f);

    // Show/don't show rendering stats
    showRenderingStats = false;
    
    // Don't simulate 3d environment
    setSimulationTimeScale(0.0f);
    msgBox("Yup");
    // Setup scene
    initScene();
    msgBox("The");
    // Setup models
    ArticulatedModel::Specification spec;
    spec.filename = "models/primitive/cube.obj";
    //spec.stripMaterials = true;
    m_scene->createModel(spec.toAny(), "cube");

    // Make the GUI after the scene is loaded because loading/rendering/simulation initialize
    // some variables that advanced GUIs may wish to reference with pointers.
    makeGUI();
}


void ViewportGApp::makeGUI()
{
    // Load icons
    // Using the Tango Icon Library: https://en.wikipedia.org/wiki/Tango_Desktop_Project
    //shared_ptr<IconSet> icons = IconSet::fromFile(System::findDataFile("tango.icn"));
}


void ViewportGApp::initScene()
{
    // Load an empty scene from a file
    //loadScene(G3D::String{ (m_exePath / "data-files/scenes/empty.Scene.Any").string() });
    loadScene("_map_editor_EmptyEditorScene");
    // Get a pointer to the loaded scene
    m_scene = scene().get();

    // Allow the scene to be edited
    m_scene->setEditing(true);
}


void ViewportGApp::onClick_AddCube()
{
    // Create a new entity
    shared_ptr<Entity> newEnt{ VisibleEntity::create(
        // Entity name
        getUniqueEntityName("cube"),
        // Scene entity is in
        m_scene,
        // Model to use
        m_scene->modelTable()["cube"].resolve(),
        // Transform
        CFrame::fromXYZYPRDegrees(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
        ) };

    // Insert the new entity into the scene hierarchy
    m_scene->insert(newEnt);
}


void ViewportGApp::onAI()
{
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void ViewportGApp::onNetwork()
{
    GApp::onNetwork();
    // Poll net messages here
}


void ViewportGApp::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    //debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool ViewportGApp::onEvent(const GEvent& event)
{
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    switch (event.type)
    {
    case GEventType::VIDEO_RESIZE:
        onResizeWindow(event);
        break;
    }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    return false;
}


void ViewportGApp::onResizeWindow(const GEvent& event)
{
    return;
}


void ViewportGApp::onUserInput(UserInput* ui)
{
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void ViewportGApp::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D)
{
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void ViewportGApp::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D)
{
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void ViewportGApp::onCleanup()
{
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}