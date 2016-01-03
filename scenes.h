#ifndef __SCENES_H
#define __SCENES_H

#pragma warning (disable : 4305)

#include "GLEW/glew.h"

/***************************** Scene ******************************/
class Scene {
    public:
        enum Scene_Type {
            POINT,
            TRIANGLE,
            WIREFRAME
        };

        static Scene *Create_Scene(Scene_Type type);
        virtual ~Scene() {}

        virtual void Set_Viewport(int width, int height);
        virtual void Render() {}
        void Increment_Frame();

    protected:
        void Initialize_Pipeline();
        void Cleanup_Pipeline();
        virtual GLuint Compile_Shaders() { return 0; }
        void Add_Shader(GLuint program, const GLchar **shader_source, GLenum shader_type);

        GLuint rendering_program;
        GLuint vertex_array_object;

        float animation_frame = 0;
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
