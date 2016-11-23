#include "scenes.h"
#include "debug.h"

/***************************** Scene ******************************/
Scene *Scene::Create_Scene(Scene_Type scene_type) {
    switch(scene_type) {
        case BLANK: return new Blank_Scene();
        case POINT: return new Point_Scene();
        case TRIANGLE: return new Triangle_Scene();
        case WIREFRAME: return new Wireframe_Scene();
        default: 
            printf("ERROR: invalid scene type!");
    }
    return new Blank_Scene();
}

void Scene::Initialize_Pipeline() {
    rendering_program = Compile_Shaders();

    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
}

void Scene::Cleanup_Pipeline() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

void Scene::Set_Viewport(int width, int height) {
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void Scene::Increment_Frame() {
    animation_frame++;
}

string Scene::Load_Shader(string shader_name) {
    string shader_source = "";
    ifstream shader_file("shaders/" + shader_name + ".sdr", ios::in);

    if (shader_file) {
        shader_source.assign(istreambuf_iterator<char>(shader_file), istreambuf_iterator<char>());
        shader_file.close();
        printf("Loaded shader \"%s\"", shader_name);
    } else {
        printf("Failed to load shader \"%s\"", shader_name);
    }

    return shader_source;
}

void Scene::Add_Shader(GLuint program, string shader_name, GLenum shader_type) {
    string shader_string = Load_Shader(shader_name.c_str());
    const GLchar *shader_source = shader_string.c_str();


    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        printf("Shader compilation failed!");

        GLchar compile_log[1024];
        glGetShaderInfoLog(shader, sizeof(compile_log), NULL, compile_log);
        printf("%s", compile_log);
        return;
    }

    glAttachShader(program, shader);
    glDeleteShader(shader);
}

/***************************** Blank_Scene ******************************/
void Blank_Scene::Render() {
    const GLfloat black[] = { 0.0, 0.0, 0.0, 0.0 };

    glClearBufferfv(GL_COLOR, 0, black);

    glFlush();
}

/***************************** Point_Scene ******************************/
Point_Scene::Point_Scene() {
    Initialize_Pipeline();
}

Point_Scene::~Point_Scene() {
    Cleanup_Pipeline();
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
    GLuint program = glCreateProgram();

    Add_Shader(program, "point_vertex", GL_VERTEX_SHADER);
    Add_Shader(program, "point_fragment", GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    return program;
}

/***************************** Triangle_Scene ******************************/
Triangle_Scene::Triangle_Scene() {
    Initialize_Pipeline();
}

Triangle_Scene::~Triangle_Scene() {
    Cleanup_Pipeline();
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
    GLuint program = glCreateProgram();

    Add_Shader(program, "triangle_vertex", GL_VERTEX_SHADER);
    Add_Shader(program, "triangle_fragment", GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    return program;
}

/***************************** Wireframe_Scene ******************************/
Wireframe_Scene::Wireframe_Scene() {
    Initialize_Pipeline();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Wireframe_Scene::~Wireframe_Scene() {
    Cleanup_Pipeline();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Wireframe_Scene::Render() {
    const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    glClearBufferfv(GL_COLOR, 0, black);

    glUseProgram(rendering_program);
    glDrawArrays(GL_PATCHES, 0, 3);
}

GLuint Wireframe_Scene::Compile_Shaders() {
    GLuint program = glCreateProgram();

    Add_Shader(program, "wireframe_vertex", GL_VERTEX_SHADER);
    Add_Shader(program, "wireframe_tessellation_control", GL_TESS_CONTROL_SHADER);
    Add_Shader(program, "wireframe_tessellation_evaluation", GL_TESS_EVALUATION_SHADER);
    Add_Shader(program, "wireframe_fragment", GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    return program;
}