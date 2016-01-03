#include "scenes.h"
#include "debug.h"

/***************************** Scene ******************************/
Scene *Scene::Create_Scene(Scene_Type scene_type) {
    switch(scene_type) {
        case POINT: return new Point_Scene();
        case TRIANGLE: return new Triangle_Scene();
        case WIREFRAME: return new Wireframe_Scene();
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
    rendering_program = Compile_Shaders();

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

GLuint Point_Scene::Compile_Shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "void main(void)   {                        \n"
        "   gl_Position = vec4(0.0, 0.0, 1.0, 1.0); \n"
        "}                                          \n"
    };

    static const GLchar *fragment_shader_source[] = {
        "#version 450 core                      \n"
        "                                       \n"
        "out vec4 color;                        \n"
        "                                       \n"
        "void main(void) {                      \n"
        "   color = vec4(0.3, 0.3, 1.0, 1.0);   \n"
        "}                                      \n"
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
    rendering_program = Compile_Shaders();

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

GLuint Triangle_Scene::Compile_Shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar *vertex_shader_source[] = {
        "#version 450 core                                  \n"
        "                                                   \n"
        "layout (location = 0) in vec4 offset;              \n"
        "layout (location = 1) in vec4 color;               \n"
        "                                                   \n"
        "out VERTEX_SHADER_OUT {                            \n"
        "   vec4 color;                                     \n"
        "} vertex_shader_out;                               \n"
        "                                                   \n"
        "void main(void) {                                  \n"
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
        "#version 450 core                      \n"
        "                                       \n"
        "in VERTEX_SHADER_OUT {                 \n"
        "   vec4 color;                         \n"
        "} fragment_shader_in;                  \n"
        "out vec4 color;                        \n"
        "                                       \n"
        "void main(void) {                      \n"
        "   color = fragment_shader_in.color;   \n"
        "}                                      \n"
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

/***************************** Wireframe_Scene ******************************/
Wireframe_Scene::Wireframe_Scene() {
    rendering_program = Compile_Shaders();

    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Wireframe_Scene::~Wireframe_Scene() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Wireframe_Scene::Render() {
    const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    glClearBufferfv(GL_COLOR, 0, black);

    glUseProgram(rendering_program);
    glDrawArrays(GL_PATCHES, 0, 3);
}

GLuint Wireframe_Scene::Compile_Shaders() {
    static const GLchar *vertex_shader_source[] = {
        "#version 450 core                                  \n"
        "                                                   \n"
        "void main(void) {                                  \n"
        "    const vec4 vertices[3] = vec4[3](              \
                 vec4( 0.25, -0.25, 0.5, 1.0),              \
                 vec4(-0.25, -0.25, 0.5, 1.0),              \
                 vec4( 0.25,  0.25, 0.5, 1.0)               \
             );                                             \n"
        "    gl_Position = vertices[gl_VertexID];           \n"
        "}                                                  \n"
    };

    static const GLchar *tessellation_control_shader_source[] = {
        "#version 450 core                                                              \n"
        "                                                                               \n"
        "layout (vertices = 3) out;                                                     \n"
        "                                                                               \n"
        "void main(void) {                                                              \n"
        "   if (gl_InvocationID == 0) {                                                 \n"
        "       gl_TessLevelInner[0] = 5.0;                                             \n"
        "       gl_TessLevelOuter[0] = 5.0;                                             \n"
        "       gl_TessLevelOuter[1] = 5.0;                                             \n"
        "       gl_TessLevelOuter[2] = 5.0;                                             \n"
        "   }                                                                           \n"
        "                                                                               \n"
        "   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;   \n"
        "}                                                                              \n"
    };

    static const GLchar *tessellation_evaluation_shader_source[] = {
        "#version 450 core                                              \n"
        "                                                               \n"
        "layout (triangles, equal_spacing, cw) in;                      \n"
        "                                                               \n"
        "void main(void) {                                              \n"
        "   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +     \n"
        "                 (gl_TessCoord.y * gl_in[1].gl_Position) +     \n"
        "                 (gl_TessCoord.z * gl_in[2].gl_Position);      \n"
        "}                                                              \n" 
    };

    static const GLchar *fragment_shader_source[] = {
        "#version 450 core                      \n"
        "                                       \n"
        "out vec4 color;                        \n"
        "                                       \n"
        "void main(void) {                      \n"
        "   color = vec4(1.0, 1.0, 1.0, 1.0);   \n"
        "}                                      \n"
    };

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint tessellation_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessellation_control_shader, 1, tessellation_control_shader_source, NULL);
    glCompileShader(tessellation_control_shader);

    GLuint tessellation_evaluation_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessellation_evaluation_shader, 1, tessellation_evaluation_shader_source, NULL);
    glCompileShader(tessellation_evaluation_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, tessellation_control_shader);
    glAttachShader(program, tessellation_evaluation_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(tessellation_control_shader);
    glDeleteShader(tessellation_evaluation_shader);
    glDeleteShader(fragment_shader);

    return program;
}