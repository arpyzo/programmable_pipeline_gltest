#include "main.h"
#include "debug.h"

IMPLEMENT_APP_CONSOLE(App)

/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame, wxFrame)
    EVT_MENU    (MENU_POINT,            Frame::Menu_Item)
    EVT_MENU    (MENU_TRIANGLE,         Frame::Menu_Item)
    EVT_MENU    (MENU_WIREFRAME,        Frame::Menu_Item)

    EVT_MENU    (MENU_GL_STATE,         Frame::Menu_Item)
    EVT_MENU    (MENU_ABOUT,            Frame::Menu_Item)
    EVT_MENU    (MENU_QUIT,             Frame::Menu_Item)
END_EVENT_TABLE()
        
Frame::Frame() 
: wxFrame((wxFrame *)NULL, -1, "GLTest", wxPoint(50, 50), wxSize(858, 704)) {
    SetIcon(wxIcon("GLTEST_ICON"));
    Setup_Menu_To_Scene_Array();
    Setup_Menu();

    wxSplitterWindow *split_win = new wxSplitterWindow(this);
    
    gl_canvas = new Canvas(split_win);
    
    wxTextCtrl *log_text_ctrl = new wxTextCtrl(split_win, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    wxLog::SetActiveTarget(new wxLogTextCtrl(log_text_ctrl));

    split_win->SplitHorizontally(gl_canvas, log_text_ctrl, 500);
}

/****************/
/* Menu Related */
/****************/
void Frame::Setup_Menu_To_Scene_Array() {
    menu_to_scene[MENU_POINT] = Scene::POINT;
    menu_to_scene[MENU_TRIANGLE] = Scene::TRIANGLE;
    menu_to_scene[MENU_WIREFRAME] = Scene::WIREFRAME;
}

void Frame::Setup_Menu() {
    wxMenuBar *menu_bar = new wxMenuBar();
    wxMenu *file_menu = new wxMenu();
    wxMenu *util_menu = new wxMenu();
    wxMenu *basic_2d_menu = new wxMenu();
    wxMenu *help_menu = new wxMenu();
      
    file_menu->Append(MENU_QUIT, "E&xit");
    menu_bar->Append(file_menu, "&File");

    util_menu->Append(MENU_GL_STATE, "GL &State");
    menu_bar->Append(util_menu, "&Utility");

    basic_2d_menu->Append(MENU_POINT, "Point");
    basic_2d_menu->Append(MENU_TRIANGLE, "Triangle");
    basic_2d_menu->Append(MENU_WIREFRAME, "Wireframe");
    menu_bar->Append(basic_2d_menu, "Basic &2D");

    help_menu->Append(MENU_ABOUT, "&About..");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);
}

void Frame::Menu_Item(wxCommandEvent &event) {
    switch(event.GetId()) {
        case MENU_GL_STATE:
            gl_canvas->Display_GL_State();
            return;
        case MENU_ABOUT:
            wxMessageBox("GLTest\nOpenGL Programmable Pipeline\nDemonstration Program\nRobert E. Pyzalski 2016", "About..", wxICON_INFORMATION);
            return;
        case MENU_QUIT:
            Close(TRUE);
        default:
            gl_canvas->Switch_Scene(menu_to_scene[event.GetId()]);
    }
}

/****************************** App *******************************/
bool App::OnInit() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Frame *main_frame = new Frame();
    main_frame->Show(TRUE);

    return TRUE;
}
