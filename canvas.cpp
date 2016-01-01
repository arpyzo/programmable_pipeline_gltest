#include "canvas.h"
#include "debug.h"

/******************************** Canvas *********************************/
BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
    EVT_SIZE             (Canvas::Event_Resize)
    EVT_PAINT            (Canvas::Event_Paint)
    EVT_ERASE_BACKGROUND (Canvas::Event_Erase_Background)
    EVT_TIMER            (TIMER_ANIMATION, Canvas::Event_Animation_Timer)
END_EVENT_TABLE()

Canvas::Canvas(wxWindow *parent)
: wxGLCanvas(parent, -1, 0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    gl_context = new wxGLContext(this);
    SetCurrent(*gl_context);
    
    GLenum glew_init_result = glewInit();
    if (glew_init_result != GLEW_OK) {
        wxMessageBox("Failure to initialize GLEW!");
        // TODO: Confirmation and exit
        // parent->Close(???bool);
    }

    animation_timer = new wxTimer(this, TIMER_ANIMATION);
    Switch_Scene(Scene::POINT);
}

Canvas::~Canvas() {
    animation_timer->Stop();
    delete animation_timer;

    delete scene;
    delete gl_context;
}

/*****************/
/* Canvas Events */
/*****************/
void Canvas::Event_Paint(wxPaintEvent &WXUNUSED(event)) {
    wxPaintDC(this);

    if (scene) {
        scene->Render();
        SwapBuffers();
    }
}

void Canvas::Event_Resize(wxSizeEvent &WXUNUSED(event)) {
    if (scene) {
        int width, height;
        GetClientSize(&width, &height);
        scene->Set_Viewport(width, height);
    }

    // TODO: This needs further work
}

void Canvas::Event_Erase_Background(wxEraseEvent &WXUNUSED(event)) { }

void Canvas::Event_Animation_Timer(wxTimerEvent &WXUNUSED(event)) {
    scene->Increment_Frame();
    Refresh();
}

/*********************/
/* Scene Maintenance */
/*********************/
void Canvas::Switch_Scene(Scene::Scene_Type scene_type) {
    if (scene) {
        Cleanup_Scene();
    }

    Initialize_Scene(scene_type);
    Refresh();
    animation_timer->Start(25);
}

void Canvas::Initialize_Scene(Scene::Scene_Type scene_type) {
    scene = Scene::Create_Scene(scene_type);
}

void Canvas::Cleanup_Scene() {
    animation_timer->Stop();

    delete scene;
}

/********************/
/* Miscellaneous GL */
/********************/
void Canvas::Clear_Screen() {
    wxPaintDC dc(this);

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    SwapBuffers();
}

void Canvas::Display_GL_State() {
    wxString state_msg = "";
    int state_int;
    float state_float[4];

    state_msg += "GL_VENDOR --- " + wxString(glGetString(GL_VENDOR)) + "\n";
    state_msg += "GL_RENDERER --- " + wxString(glGetString(GL_RENDERER)) + "\n";
    state_msg += "GL_VERSION --- " + wxString(glGetString(GL_VERSION)) + "\n";
    state_msg += "GL_EXTENSIONS --- " + wxString(glGetString(GL_EXTENSIONS)) + "\n";
    state_msg += "\n";

    state_msg += "GL_BLEND --- " + GLBool_Str(glIsEnabled(GL_BLEND)) + "\n";
    state_msg += "GL_DEPTH_TEST --- " + GLBool_Str(glIsEnabled(GL_DEPTH_TEST)) + "\n";
    state_msg += "GL_LINE_STIPPLE --- " + GLBool_Str(glIsEnabled(GL_LINE_STIPPLE)) + "\n";
    state_msg += "GL_FOG --- " + GLBool_Str(glIsEnabled(GL_FOG)) + "\n";
    state_msg += "\n";

    glGetFloatv(GL_POINT_SIZE_RANGE, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_RANGE --- %.1f, %.1f\n", state_float[0], state_float[1]);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, state_float);
    state_msg += wxString::Format("GL_POINT_SIZE_GRANULARITY --- %.4f\n", state_float[0]);
    //glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_SMOOTH_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    //glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, state_float);
    //state_msg += wxString::Format("GL_ALIASED_POINT_SIZE_RANGE --- %f, %f\n", state_float[0], state_float[1]);
    
    state_msg += "GL_LIGHTING --- " + GLBool_Str(glIsEnabled(GL_LIGHTING)) + "\n";
    state_msg += "GL_LIGHT_MODEL_LOCAL_VIEWER --- " + GLBool_Str(glIsEnabled(GL_LIGHT_MODEL_LOCAL_VIEWER)) + "\n";
    state_msg += "GL_LIGHT_MODEL_TWO_SIDE --- " + GLBool_Str(glIsEnabled(GL_LIGHT_MODEL_TWO_SIDE)) + "\n";
    glGetFloatv(GL_LIGHT_MODEL_AMBIENT, state_float);
    state_msg += wxString::Format("GL_LIGHT_MODEL_AMBIENT --- %.1f, %.1f, %.1f, %.1f\n", state_float[0], state_float[1], state_float[2], state_float[3]);
    glGetIntegerv(GL_MAX_LIGHTS, &state_int);
    state_msg += wxString::Format("GL_MAX_LIGHTS --- %d\n", state_int);
    state_msg += "GL_LIGHT0 --- " + GLBool_Str(glIsEnabled(GL_LIGHT0)) + "\n";
    state_msg += "GL_LIGHT1 --- " + GLBool_Str(glIsEnabled(GL_LIGHT1)) + "\n";

    wxMessageBox(state_msg, "OpenGL State");
}

wxString Canvas::GLBool_Str(GLboolean gl_bool) {
    if (gl_bool) {
        return "True";
    }
    return "False";
}
