#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "program.h"

using namespace std;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (glfwInit() == GLFW_FALSE)
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(1024, 768, "Mask editor", NULL, NULL);
    if (window == 0)
    {
        glfwTerminate();
        return -1;
    }

    Program app(window);

    glfwSetKeyCallback(window, Program::KeyActionCallback);
    glfwSetCursorPosCallback(window, Program::CursorPosCallback);
    glfwSetScrollCallback(window, Program::ScrollCallback);
    glfwSetMouseButtonCallback(window, Program::MouseButtonCallback);

    glfwSetWindowSizeCallback(window, Program::ResizeCallback);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
//    style.Colors[ImGuiCol_Button] = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    Program::ResizeCallback(window, 1024, 768);

    if (app.SetUp())
    {
        while (glfwWindowShouldClose(window) == 0)
        {
//            glfwWaitEvents();
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);

            app.Render();

            glfwSwapBuffers(window);
        }
        app.CleanUp();
    }

    glfwTerminate();

    return 0;
}

