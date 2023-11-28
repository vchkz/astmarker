#include "program.h"
#include <glad/glad.h>

#include <imgui.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>



#include "fstream"


#include<opencv2//opencv.hpp>


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "image.h"



Image firstImage;
Image secondImage;
Image overlayImage;

static struct {
    int width = 200;
    int height = 300;
    int mousex = 0;
    int mousey = 0;
    int mouseImagex = 0;
    int mouseImagey = 0;
    int zoom = 100;
    int translatex = 0;
    int translatey = 0;
    bool shiftPressed = false;
    bool ctrlPressed = false;
    bool spacePressed = false;
    float scale = 1.0f;
    ImVec2 offset = ImVec2(0.0f, 0.2f);
} state;

Program::Program(GLFWwindow *window)
        : _window(window) {
    glfwSetWindowUserPointer(this->_window, static_cast<void *>(this));
}

Program::~Program() {
    glfwSetWindowUserPointer(this->_window, nullptr);
}



bool Program::SetUp() {
    ImGuiIO &io = ImGui::GetIO();
    return true;
}


ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);




void openImage(Image& img) {

    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
    if (result == NFD_OKAY) {
        img.fromFile(outPath);
    }
    else {
        std::cout << "nfd error" << std::endl;
    }
}

// Функция для обработки нажатия кнопки "save"
void savePoints() {
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("txt", NULL, &outPath);

    if (result == NFD_OKAY) {
        // TODO: тут будет сохранение точек
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
        free(outPath);
    }
    else {
        printf("Error: %s\n", NFD_GetError());
        free(outPath);
    }
}

void importPoints(){
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

    if (result == NFD_OKAY) {
        //TODO: откырытие точек
    }
    else {
        std::cout << "Error: %s\n" << NFD_GetError() << std::endl;
    }
}

void Program::Render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Image 1")) openImage(firstImage);
            if (ImGui::MenuItem("Open Image 2")) openImage(secondImage);
            if (ImGui::MenuItem("Import a set of points", "CTRL+O", false, true)) importPoints();
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "CTRL+S", false, true)) savePoints();
            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) {
                //TODO: Фунуция для выхода из программы
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    {
        // Получение размеров экрана
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        // Вычисление новых размеров и позиций окон
        ImVec2 windowSize = ImVec2(screenSize.x / 3, screenSize.y);
        ImVec2 windowPos1 = ImVec2(0, 20);
        ImVec2 windowPos2 = ImVec2(screenSize.x / 3, 20);
        ImVec2 windowPos3 = ImVec2(2 * screenSize.x / 3, 20);


        // Рисование первого окна
        ImGui::SetNextWindowPos(windowPos1);
        ImGui::SetNextWindowSize(windowSize);
        ImGui::Begin("FIRST WINDOW", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);

        if (firstImage.isOpened) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 imageSize = ImVec2(firstImage._data.cols, firstImage._data.rows);
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = ImVec2(1.0f, 1.0f);

            // Рассчитываем координаты вершин с учетом масштаба и смещения
            ImVec2 a = ImVec2(state.translatex, state.translatey);
            ImVec2 b = ImVec2(a.x + imageSize.x * (state.zoom / 100.0), a.y + imageSize.y * (state.zoom / 100.0));
            drawList->AddImage((void*)(intptr_t)firstImage._glindex, a, b, uv0, uv1);
        }

        ImGui::End();


        // Рисование второго окна
        ImGui::SetNextWindowPos(windowPos2);
        ImGui::SetNextWindowSize(windowSize);
        ImGui::Begin("SECOND WINDOW", nullptr, ImGuiWindowFlags_NoCollapse);
        if (secondImage.isOpened) {

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 imageSize = ImVec2(secondImage._data.cols, secondImage._data.rows);
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = ImVec2(1.0f, 1.0f);

            // Рассчитываем координаты вершин с учетом масштаба и смещения
            ImVec2 a = ImVec2(state.translatex + windowPos2.x, state.translatey);
            ImVec2 b = ImVec2(a.x + imageSize.x * (state.zoom / 100.0), a.y + imageSize.y * (state.zoom / 100.0));
            drawList->AddImage((void*)(intptr_t)secondImage._glindex, a, b, uv0, uv1);

        }
        ImGui::End();

        // Рисование третьего окна
        ImGui::SetNextWindowPos(windowPos3);
        ImGui::SetNextWindowSize(windowSize);
        ImGui::Begin("OVERLAY WINDOW", nullptr, ImGuiWindowFlags_NoCollapse);
        if (overlayImage.isOpened) {

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 imageSize = ImVec2(overlayImage._data.cols, overlayImage._data.rows);
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = ImVec2(1.0f, 1.0f);

            // Рассчитываем координаты вершин с учетом масштаба и смещения
            ImVec2 a = ImVec2(state.translatex + windowPos3.x, state.translatey);
            ImVec2 b = ImVec2(a.x + imageSize.x * (state.zoom / 100.0), a.y + imageSize.y * (state.zoom / 100.0));
            drawList->AddImage((void*)(intptr_t)overlayImage._glindex, a, b, uv0, uv1);

        }
        ImGui::End();
    }


    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x* clear_color.w, clear_color.y* clear_color.w, clear_color.z* clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::onKeyAction(
        int key,
        int scancode,
        int action,
        int mods) {
    (void) key;
    (void) scancode;
    (void) action;

    state.shiftPressed = (mods & GLFW_MOD_SHIFT);
    state.ctrlPressed = (mods & GLFW_MOD_CONTROL);


    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            state.spacePressed = true;
        } else if (action == GLFW_RELEASE) {
            state.spacePressed = false;
        }

        return;
    }

}

//bool isMouseInContent() {
//    if (state.mousex < state.contentPosition.x) return false;
//    if (state.mousey < state.contentPosition.y) return false;
//    if (state.mousex > (state.contentPosition.x + state.contentSize.x)) return false;
//    if (state.mousey > (state.contentPosition.y + state.contentSize.y)) return false;
//
//    return true;
//}

void Program::onMouseMove(
        int x,
        int y) {

    if (state.ctrlPressed){
        state.translatex += ((x - state.mousex) );
        state.translatey += ((y - state.mousey) );
    }
    state.mousex = x;
    state.mousey = y;
}

void Program::onMouseButton(
        int button,
        int action,
        int mods) {
}

void Program::onScroll(int x, int y) {
    auto oldZoom = state.zoom;

    state.zoom += (y * 10);
    if (state.zoom < 10) state.zoom = 10;

    auto centerX = state.width / 2;
    auto centerY = state.height / 2;

    state.translatex -= ((centerX - state.mousex) * ((oldZoom - state.zoom) / 200.0f));
    state.translatey -= ((centerY - state.mousey) * ((oldZoom - state.zoom) / 200.0f));

}


void Program::onResize(
        int width,
        int height) {
}

void Program::CleanUp() {}

void Program::KeyActionCallback(
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onKeyAction(key, scancode, action, mods);
}

void Program::CursorPosCallback(
        GLFWwindow *window,
        double x,
        double y) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onMouseMove(int(x), int(y));
}

void Program::ScrollCallback(
        GLFWwindow *window,
        double x,
        double y) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onScroll(int(x), int(y));
}

void Program::MouseButtonCallback(
        GLFWwindow *window,
        int button,
        int action,
        int mods) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onMouseButton(button, action, mods);
}

void Program::ResizeCallback(
        GLFWwindow *window,
        int width,
        int height) {
    auto app = static_cast<Program *>(glfwGetWindowUserPointer(window));

    if (app != nullptr) app->onResize(width, height);
}
