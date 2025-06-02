#include <print>
#include <iostream>
#include <fstream>
#include <array>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>


constexpr int width  = 1600;
constexpr int height = 900;


struct Vertex {
    glm::vec3 m_pos;

    Vertex(glm::vec3 pos)
    : m_pos(pos)
    { }

};

[[nodiscard]] static std::string read_entire_file(const char *filename) {
    std::ifstream file(filename);
    return std::string(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );
}

static void process_inputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main() {

    std::array vertices {
        Vertex({ -0.5f, -0.5f, 0.0f }),
        Vertex({  0.5f, -0.5f, 0.0f }),
        Vertex({  0.0f,  0.5f, 0.0f })
    };


    glfwSetErrorCallback([](int _error_code, const char* description) {
        (void) _error_code;
        std::println(stderr, "GLFW ERROR: {}", description);
    });

    glfwInit();
    GLFWwindow *window = glfwCreateWindow(width, height, "gl", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL(glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow* _win, int w, int h) {
            (void) _win;
            glViewport(0, 0, w, h);
        }
    );

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    std::string vert_src = read_entire_file("shader.vert");
    auto vert_cstr = vert_src.c_str();
    glShaderSource(vert, 1, &vert_cstr, NULL);
    glCompileShader(vert);


    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    std::string frag_src = read_entire_file("shader.frag");
    auto frag_cstr = frag_src.c_str();
    glShaderSource(frag, 1, &frag_cstr, NULL);
    glCompileShader(frag);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    GLuint a_pos = glGetAttribLocation(program, "a_pos");
    glVertexAttribPointer(
        a_pos,
        3,
        GL_FLOAT,
        false,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, m_pos))
    );
    glEnableVertexAttribArray(a_pos);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        process_inputs(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
