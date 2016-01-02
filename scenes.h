#ifndef __SCENES_H
#define __SCENES_H

#pragma warning (disable : 4305)

#include "GLEW/glew.h"

/***************************** Scene ******************************/
class Scene {
    public:
        enum Scene_Type {
            POINT,
            TRIANGLE
        };

        static Scene *Create_Scene(Scene_Type type);
        virtual ~Scene() {}

        virtual void Set_Viewport(int width, int height);
        virtual void Render() {}
        void Increment_Frame();

    protected:
        virtual void Generate_Polygons() {}

        float animation_frame = 0;
};

/***************************** Point_Scene ******************************/
class Point_Scene : public Scene {
    public:
        Point_Scene();
        ~Point_Scene();

        void Render();

    protected:
        GLuint Compile_Point_Shaders();

        GLuint rendering_program;
        GLuint vertex_array_object;
};

/***************************** Triangle_Scene ******************************/
class Triangle_Scene : public Scene {
public:
    Triangle_Scene();
    ~Triangle_Scene();

    void Render();

protected:
    GLuint Compile_Triangle_Shaders();

    GLuint rendering_program;
    GLuint vertex_array_object;

};

#endif // __SCENES_H
