#include <print>
#include <array>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

namespace {

struct Vertex {
    glm::vec3 m_pos;
    Vertex(glm::vec3 pos) : m_pos(pos) { }
};

constexpr char shader_vertex[] = {
    #embed "shader.vert" suffix(,)
    '\0'
};

constexpr char shader_fragment[] = {
    #embed "shader.frag" suffix(,)
    '\0'
};

[[nodiscard]] GLuint compile_shader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512] = {0};
        glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader compilation failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

[[nodiscard]] GLuint create_shader_program(const char* vertex_src, const char* fragment_src) {

    GLuint vert = compile_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glDeleteShader(vert);
    glDeleteShader(frag);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512] = {0};
        glGetProgramInfoLog(program, sizeof info_log, nullptr, info_log);
        std::println(stderr, "shader program linkage failed: {}", info_log);
        exit(EXIT_FAILURE);
    }

    return program;
}

[[nodiscard]] GLFWwindow* setup_glfw(int width, int height, const char* window_title) {
    glfwSetErrorCallback([]([[maybe_unused]] int error_code, char const* desc) {
        std::println(stderr, "glfw error: {}", desc);
    });

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow* window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL(glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(
        window, []([[maybe_unused]] GLFWwindow* win, int w, int h) {
            glViewport(0, 0, w, h);
        }
    );

    return window;
}

void process_inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

} // namespace

int main() {

    std::array vertices {
        Vertex({ -0.5f, -0.5f, 0.0f }), // bottom-left
        Vertex({  0.5f, -0.5f, 0.0f }), // bottom-right
        Vertex({  0.0f,  0.5f, 0.0f }), // top
    };

    GLFWwindow* window = setup_glfw(1600, 900, "opengl template");

    glDebugMessageCallback([](
        [[maybe_unused]] GLenum src,
        [[maybe_unused]] GLenum type,
        [[maybe_unused]] GLuint id,
        [[maybe_unused]] GLenum severity,
        [[maybe_unused]] GLsizei len,
        const char* msg,
        [[maybe_unused]] const void* args
    ) { std::println(stderr, "opengl error: {}", msg); }, nullptr);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    GLuint program = create_shader_program(shader_vertex, shader_fragment);

    GLuint a_pos = glGetAttribLocation(program, "a_pos");
    glVertexAttribPointer(a_pos, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_pos)));
    glEnableVertexAttribArray(a_pos);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    double frame_time = 0.0;
    double last_frame = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        frame_time = time - last_frame;
        last_frame = time;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::println("FPS: {}", 1.0 / frame_time);

        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        process_inputs(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glfwDestroyWindow(window);
    glfwTerminate();

}
