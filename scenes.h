#ifndef __SCENES_H
#define __SCENES_H

#pragma warning (disable : 4305)

#include <fstream>
#include "GLEW/glew.h"

using namespace std;

/***************************** Scene ******************************/
class Scene {
    public:
        enum Scene_Type {
            BLANK,
            POINT,
            TRIANGLE,
            WIREFRAME
        };

        static Scene *Create_Scene(Scene_Type type);
        virtual ~Scene() {}

        void Set_Viewport(int width, int height);
        virtual void Render() {}
        void Increment_Frame();

    protected:
        void Initialize_Pipeline();
        void Cleanup_Pipeline();

        virtual GLuint Compile_Shaders() { return 0; }
        string Load_Shader(string shader_name);
        void Add_Shader(GLuint program, string shader_name, GLenum shader_type);

        GLuint rendering_program;
        GLuint vertex_array_object;

        float animation_frame = 0;
};

/***************************** Blank_Scene ******************************/
class Blank_Scene : public Scene {
public:
    Blank_Scene() {}
    ~Blank_Scene() {}

    void Render();
};

/***************************** Point_Scene ******************************/
class Point_Scene : public Scene {
    public:
        Point_Scene();
        ~Point_Scene();

        void Render();

    protected:
        GLuint Compile_Shaders();
};

/***************************** Triangle_Scene ******************************/
class Triangle_Scene : public Scene {
    public:
        Triangle_Scene();
        ~Triangle_Scene();

        void Render();

    protected:
        GLuint Compile_Shaders();
};

/***************************** Wireframe_Scene ******************************/
class Wireframe_Scene : public Scene {
    public:
        Wireframe_Scene();
        ~Wireframe_Scene();

        void Render();

    protected:
        GLuint Compile_Shaders();
};
#endif // __SCENES_H
