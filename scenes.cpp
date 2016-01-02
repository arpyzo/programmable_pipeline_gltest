#include "scenes.h"
#include "debug.h"

/***************************** Scene ******************************/
Scene *Scene::Create_Scene(Scene_Type scene_type) {
    switch(scene_type) {
        case POINT: return new Point_Scene();
        case TRIANGLE: return new Triangle_Scene();
        default: 
            printf("ERROR: invalid scene type!");
    }
    // TODO: Create blank scene for this
    return new Point_Scene();
}

void Scene::Set_Viewport(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void Scene::Increment_Frame() {
    animation_frame++;
}

/***************************** Point_Scene ******************************/
Point_Scene::Point_Scene() {
    rendering_program = Compile_Point_Shaders();

    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glPointSize(40);
}

Point_Scene::~Point_Scene() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

void Point_Scene::Render() {
    const GLfloat gray[] = { 0.5, 0.5, 0.5, 1.0 };

    glClearBufferfv(GL_COLOR, 0, gray);

    glPointSize(abs(sinf(animation_frame / 50) * 100));

    glUseProgram(rendering_program);
    glDrawArrays(GL_POINTS, 0, 1);
    glFlush();
}

GLuint Point_Scene::Compile_Point_Shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "void main() {                              \n"
        "   gl_Position = vec4(0.0, 0.0, 1.0, 1.0); \n"
        "}                                          \n"
    };

    static const GLchar *fragment_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "out vec4 color;                            \n"
        "                                           \n"
        "void main() {                              \n"
        "   color = vec4(0.3, 0.3, 1.0, 1.0);       \n"
        "}                                          \n"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

/***************************** Triangle_Scene ******************************/
Triangle_Scene::Triangle_Scene() {
    rendering_program = Compile_Triangle_Shaders();

    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
}

Triangle_Scene::~Triangle_Scene() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

void Triangle_Scene::Render() {
    const GLfloat blue[] = { 0.3, 0.3, 1.0, 1.0 };
    glClearBufferfv(GL_COLOR, 0, blue);

    GLfloat offset[] = { sinf(animation_frame / 30) * 0.7, cosf(animation_frame / 30) * 0.7, 0, 0 };
    glVertexAttrib4fv(0, offset);

    const GLfloat green[] = { 0.2, 1.0, 0.2, 1.0 };
    glVertexAttrib4fv(1, green);

    glUseProgram(rendering_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

GLuint Triangle_Scene::Compile_Triangle_Shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] = {
        "#version 450 core                                  \n"
        "                                                   \n"
        "                                                   \n"
        "layout (location = 0) in vec4 offset;              \n"
        "layout (location = 1) in vec4 color;               \n"
        "                                                   \n"
        "out VERTEX_SHADER_OUT {                            \n"
        "   vec4 color;                                     \n"
        "} vertex_shader_out;                               \n"
        "                                                   \n"
        "void main() {                                      \n"
        "const vec4 vertices[3] = vec4[3](                  \
                vec4( 0.25, -0.25, 0.5, 1.0),               \
                vec4(-0.25, -0.25, 0.5, 1.0),               \
                vec4( 0.25,  0.25, 0.5, 1.0)                \
         );                                                 \n"
        "   gl_Position = vertices[gl_VertexID] + offset;   \n"
        "                                                   \n"
        "   vertex_shader_out.color = color;                \n"
        "}                                                  \n"
    };

    static const GLchar *fragment_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "in VERTEX_SHADER_OUT {                     \n"
        "   vec4 color;                             \n"
        "} fragment_shader_in;                      \n"
        "out vec4 color;                            \n"
        "                                           \n"
        "void main() {                              \n"
        "   color = fragment_shader_in.color;       \n"
        "}                                          \n"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}