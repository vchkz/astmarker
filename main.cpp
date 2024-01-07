#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include "image.h"
#include "imageEval.h"
#define M_PI 3.14159265358979323846

enum screenState {
    mainScreen = 0,
    DistortionEvaluationScreen
} currentState;


struct App {
    sf::RenderWindow window;

    sf::Vector2f translate;
    sf::Vector2f mouseCoord;

    Image firstIm;
    Image secondIm;
    Image thirdIm;

    const float CircleRadius = 10;

    ImageEval evaluationImg;

    bool showEvaluationSettingsWindow = false;
    bool pointMapping = false;
//    bool isMarkingAvailable = false;

    float zoom;

    enum distortImage {
        firstImage = 0,
        secondImage
    } currentDistortImage = firstImage;

    App() : window(sf::VideoMode(600, 600), "SFML Image Viewer"), zoom(1.0f) {

        firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
        secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
        thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
        evaluationImg.internalTexture.create(window.getSize().x, window.getSize().y);

    }

    void handleInputCastom() {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseWheelScrolled) { // Масштабирование колёсиком мыши
                auto oldZoom = zoom;

                if (event.mouseWheelScroll.delta > 0) zoom *= 1.1f;
                else zoom /= 1.1f;

                auto centerX = firstIm.internalTexture.getSize().x / 2;
                auto centerY = firstIm.internalTexture.getSize().y / 2;
                sf::Vector2f temp(((centerX) * (oldZoom - zoom) / 2), ((centerY) * (oldZoom - zoom) / 2));
                translate += temp;
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    translate += newCoord - mouseCoord;
                }
                mouseCoord = newCoord;

            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                evaluationImg.internalTexture.create(window.getSize().x, window.getSize().y);
            }
        }
    };

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                evaluationImg.internalTexture.create(window.getSize().x, window.getSize().y);
            } else if (event.type == sf::Event::MouseWheelScrolled) { // Масштабирование колёсиком мыши
                auto oldZoom = zoom;

                if (event.mouseWheelScroll.delta > 0) zoom *= 1.1f;
                else zoom /= 1.1f;

                auto centerX = firstIm.internalTexture.getSize().x / 2;
                auto centerY = firstIm.internalTexture.getSize().y / 2;
                sf::Vector2f temp(((centerX) * (oldZoom - zoom) / 2), ((centerY) * (oldZoom - zoom) / 2));
                translate += temp;
            } else if (event.type == sf::Event::MouseMoved) {

                // Движение мыши
                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    translate += newCoord - mouseCoord;
                } else {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    sf::Vector2f mousePosFloat4Second(mousePosFloat.x - static_cast<float>(window.getSize().x) / 3.0f,
                                                      mousePosFloat.y);
                    sf::Vector2f mousePosFloat4SecondTemp(mouseCoord.x - static_cast<float>(window.getSize().x) / 3.0f,
                                                          mouseCoord.y);

                    // Обработка hover для первой трети окна
                    if ((mousePos.x < window.getSize().x / 3) and firstIm.is_opened and secondIm.is_opened) {
                        bool loopCompleted = true;
                        for (int i = 0; i < firstIm.coordPoints.size(); ++i) {
                            sf::Vector2f circleCenter(
                                    firstIm.sprite.getTransform().transformPoint(firstIm.coordPoints[i]));

                            if (std::pow(mousePosFloat.x - circleCenter.x, 2) +
                                std::pow(mousePosFloat.y - circleCenter.y, 2) <= std::pow(CircleRadius, 2)) {
                                firstIm.hoveredCircleIndex = i;
                                loopCompleted = false;
                                break;
                            }
                        }
                        if (loopCompleted) { firstIm.hoveredCircleIndex = -1; }
                    }
                    // Обработка hover для второй трети окна
                    if (mousePos.x > window.getSize().x / 3 and mousePos.x < window.getSize().x / 3 * 2 and
                        secondIm.is_opened) {
                        bool loopCompleted = true;
                        for (int i = 0; i < secondIm.coordPoints.size(); ++i) {
                            sf::Vector2f circleCenter(
                                    secondIm.sprite.getTransform().transformPoint(secondIm.coordPoints[i]));
                            if (std::pow(mousePosFloat4Second.x - circleCenter.x, 2) +
                                std::pow(mousePosFloat4Second.y - circleCenter.y, 2) <= std::pow(CircleRadius, 2)) {
                                secondIm.hoveredCircleIndex = i;
                                loopCompleted = false;
                                break;
                            }
                        }
                        if (loopCompleted) { secondIm.hoveredCircleIndex = -1; }
                    }


                    if (firstIm.selectedCircleIndex != -1) {
                        firstIm.coordPoints[firstIm.selectedCircleIndex] +=
                                firstIm.sprite.getInverseTransform().transformPoint(mousePosFloat) -
                                firstIm.sprite.getInverseTransform().transformPoint(mouseCoord);
                    }
                    if (secondIm.selectedCircleIndex != -1) {
                        secondIm.coordPoints[secondIm.selectedCircleIndex] +=
                                secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat) -
                                secondIm.sprite.getInverseTransform().transformPoint(mouseCoord);
                    }
                }


                mouseCoord = newCoord;

            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (pointMapping) pointMapping = false;

                else {
                    if ((mousePos.x < window.getSize().x / 3) and (mousePos.y > 30) and firstIm.is_opened and
                        secondIm.is_opened) {
                        firstIm.selectedCircleIndex = firstIm.hoveredCircleIndex;

                        if (firstIm.selectedCircleIndex == -1 and
                            firstIm.sprite.getGlobalBounds().contains(mousePosFloat)) {

                            sf::Vector2f point = firstIm.sprite.getInverseTransform().transformPoint(mousePosFloat);

                            secondIm.coordPoints.push_back(secondIm.predictPoint(point, firstIm));
                            firstIm.coordPoints.push_back(point);
                            secondIm.selectedCircleIndex = secondIm.coordPoints.size() - 1;
                            pointMapping = true;
                        }
                    } else if (mousePos.x > window.getSize().x / 3 and mousePos.x < window.getSize().x / 3 * 2 and
                               (mousePos.y > 30) and secondIm.is_opened) {
                        mousePosFloat.x += -static_cast<float>(window.getSize().x) / 3.0f;
                        secondIm.selectedCircleIndex = secondIm.hoveredCircleIndex;


                        if (secondIm.selectedCircleIndex == -1 and
                            secondIm.sprite.getGlobalBounds().contains(mousePosFloat)) {
                            sf::Vector2f point = secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat);

                            firstIm.coordPoints.push_back(firstIm.predictPoint(point, secondIm));
                            secondIm.coordPoints.push_back(point);

                            firstIm.selectedCircleIndex = firstIm.coordPoints.size() - 1;
                            pointMapping = true;
                        }
                    }
                }

            } else if (event.type == sf::Event::MouseButtonReleased and !pointMapping) {
                if (firstIm.selectedCircleIndex != -1 or secondIm.selectedCircleIndex != -1) {
                    firstIm.selectedCircleIndex = -1;
                    secondIm.selectedCircleIndex = -1;
                    thirdIm.genWarpImg(firstIm, secondIm);
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Delete) {
                    if (firstIm.hoveredCircleIndex != -1 or secondIm.hoveredCircleIndex != -1) {
                        int index;
                        if (firstIm.hoveredCircleIndex != -1) { index = firstIm.hoveredCircleIndex; }
                        else if (secondIm.hoveredCircleIndex != -1) { index = secondIm.hoveredCircleIndex; }
                        firstIm.coordPoints.erase(firstIm.coordPoints.begin() + index);
                        secondIm.coordPoints.erase(secondIm.coordPoints.begin() + index);
                        thirdIm.genWarpImg(firstIm, secondIm);
                    }
                }
            }
        }
    }


    void update() {
        ImGui::SFML::Update(window, sf::seconds(1.0f / 60.0f));


        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Image 1")) {
                    firstIm.openImage();
                    if (secondIm.is_opened && (secondIm.texture.getSize().y > firstIm.texture.getSize().y)) {
                        firstIm.scaleFactor = static_cast<float>(secondIm.texture.getSize().y) /
                                              static_cast<float>(firstIm.texture.getSize().y);
                    }
                }
                if (ImGui::MenuItem("Open Image 2")) {
                    secondIm.openImage();
                    if (firstIm.is_opened && (firstIm.texture.getSize().y > secondIm.texture.getSize().y)) {
                        secondIm.scaleFactor = static_cast<float>(firstIm.texture.getSize().y) /
                                               static_cast<float>(secondIm.texture.getSize().y);
                    }
                }
                if (ImGui::MenuItem("Import a set of points", "CTRL+O", false, true));
                ImGui::Separator();
                if (ImGui::MenuItem("Save", "CTRL+S", false, true));
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) {
                    window.close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Warp evaluation")) {
                if (ImGui::MenuItem("Warp image", nullptr, false, firstIm.coordPoints.size()>3)){
                    currentState = DistortionEvaluationScreen;
                    evaluationImg.genEvalImg(firstIm, secondIm);
                    showEvaluationSettingsWindow = true;
                    zoom = 1;
                    translate = sf::Vector2f(0, 0);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", nullptr, false, currentState == DistortionEvaluationScreen)){
                    currentState = mainScreen;
                    showEvaluationSettingsWindow = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (showEvaluationSettingsWindow) {
            ImGui::Begin("Model Settings", &showEvaluationSettingsWindow);
            ImGui::Separator();
            ImGui::Text("Type of transformation:");
            if (ImGui::RadioButton("Affine", (int *) &evaluationImg.currentModelType, evaluationImg.affine)) {
                if (currentDistortImage == firstImage)evaluationImg.genEvalImg(firstIm, secondIm);
                if (currentDistortImage == secondImage)evaluationImg.genEvalImg(secondIm, firstIm);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Perspective", (int *) &evaluationImg.currentModelType, evaluationImg.perspective)) {
                if (currentDistortImage == firstImage)evaluationImg.genEvalImg(firstIm, secondIm);
                if (currentDistortImage == secondImage)evaluationImg.genEvalImg(secondIm, firstIm);
            }
            ImGui::Separator();
            ImGui::Text("Which image should be warped:");
            if (ImGui::RadioButton("First", (int *) &currentDistortImage, firstImage)) {
                evaluationImg.genEvalImg(firstIm, secondIm);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Second", (int *) &currentDistortImage, secondImage)) {
                evaluationImg.genEvalImg(secondIm, firstIm);
            }
            ImGui::Separator();
            ImGui::SliderFloat("Transparency", &evaluationImg.transparency, 0.01f, 255.0f);

            ImGui::Separator();
            ImGui::Checkbox("Show points", &evaluationImg.showPoints);
            ImGui::SliderFloat("Point size", &evaluationImg.pointSize, 1.0f, 20.0f);
            ImGui::SliderFloat("Line wight", &evaluationImg.lineWidth, 1.0f, 20.0f);

            ImGui::End();
        }


        ImGui::Begin("statusbar", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::SetWindowPos(ImVec2(0.0f, window.getSize().y - 30.0f));
            ImGui::SetWindowSize(ImVec2(window.getSize().x, 30.0f));

            ImGui::Columns(8);
            ImGui::Text("win size: %d %d", window.getSize().x, window.getSize().y);
            ImGui::NextColumn();
            ImGui::Text("translate: %f %f", translate.x, translate.y);
            ImGui::NextColumn();
            ImGui::Text("zoom: %f", zoom);
            ImGui::NextColumn();
            ImGui::Text("mouse: %d %d", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            ImGui::NextColumn();
            ImGui::Text("1 hoverID: %d", firstIm.hoveredCircleIndex);
            ImGui::NextColumn();
            ImGui::Text("2 hoverID: %d", secondIm.hoveredCircleIndex);
            ImGui::NextColumn();
            ImGui::Text("1 selectID: %d", firstIm.selectedCircleIndex);
            ImGui::NextColumn();
            ImGui::Text("2 selectID: %d", secondIm.selectedCircleIndex);
        }
        ImGui::End();
    }

    void render() {

        window.clear();
        switch (currentState) {
            case mainScreen:
                if (firstIm.is_opened) {
                    firstIm.internalTexture.clear();
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(firstIm.internalTexture.getTexture());
                    firstIm.sprite.setPosition(translate);
                    firstIm.sprite.setScale(zoom, zoom);
                    firstIm.internalTexture.draw(firstIm.sprite);

                    for (std::size_t i = 0; i < firstIm.coordPoints.size(); ++i) {
                        sf::CircleShape tempCircle(CircleRadius);
                        tempCircle.setPosition(firstIm.sprite.getTransform().transformPoint(firstIm.coordPoints[i]));
                        tempCircle.move(-CircleRadius, -CircleRadius);
                        if (firstIm.hoveredCircleIndex == i or secondIm.hoveredCircleIndex == i)
                            tempCircle.setFillColor(sf::Color::Blue);
                        firstIm.internalTexture.draw(tempCircle);
                    }
                    firstIm.internalTexture.display();
                    window.draw(internalSprite);
                }
                if (secondIm.is_opened) {
                    secondIm.internalTexture.clear();
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(secondIm.internalTexture.getTexture());
                    internalSprite.move(window.getSize().x / 3, 0);
                    secondIm.sprite.setPosition(translate);
                    secondIm.sprite.setScale(zoom, zoom);

                    secondIm.internalTexture.draw(secondIm.sprite);

                    for (std::size_t i = 0; i < secondIm.coordPoints.size(); ++i) {
                        sf::CircleShape tempCircle(CircleRadius);
                        tempCircle.setPosition(secondIm.sprite.getTransform().transformPoint(secondIm.coordPoints[i]));
                        tempCircle.move(-CircleRadius, -CircleRadius);
                        if (firstIm.hoveredCircleIndex == i or secondIm.hoveredCircleIndex == i)
                            tempCircle.setFillColor(sf::Color::Blue);
                        secondIm.internalTexture.draw(tempCircle);
                    }
                    secondIm.internalTexture.display();
                    window.draw(internalSprite);
                }
                if (thirdIm.is_opened) {
                    thirdIm.internalTexture.clear();
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(thirdIm.internalTexture.getTexture());
                    internalSprite.move(window.getSize().x / 3 * 2, 0);
                    thirdIm.sprite.setPosition(translate);
                    thirdIm.sprite.setScale(zoom, zoom);
                    thirdIm.internalTexture.draw(thirdIm.sprite);
                    thirdIm.internalTexture.display();
                    window.draw(internalSprite);
                }
                break;
            case DistortionEvaluationScreen:
                if (currentDistortImage == firstImage) {
                    evaluationImg.internalTexture.clear();
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(evaluationImg.internalTexture.getTexture());

                    secondIm.sprite.setPosition(translate);
                    secondIm.sprite.setScale(zoom, zoom);
                    evaluationImg.internalTexture.draw(secondIm.sprite);
                    evaluationImg.sprite.setPosition(translate);
                    evaluationImg.sprite.setScale(zoom, zoom);
                    evaluationImg.sprite.setColor(sf::Color(255, 255, 255, evaluationImg.transparency));
                    evaluationImg.internalTexture.draw(evaluationImg.sprite);
                    if (evaluationImg.showPoints) {
                        for (std::size_t i = 0; i < firstIm.coordPoints.size(); ++i) {
                            sf::Vector2f srcPoint(secondIm.sprite.getTransform().transformPoint(secondIm.coordPoints[i]));
                            sf::Vector2f dstPoint(evaluationImg.sprite.getTransform().transformPoint(evaluationImg.coordPoints[i]));

                            //lines
                            sf::Vector2f delta = dstPoint - srcPoint;
                            sf::RectangleShape lineShape1;
                            sf::RectangleShape lineShape2;
                            lineShape1.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y), evaluationImg.lineWidth));
                            lineShape2.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y), -evaluationImg.lineWidth));
                            lineShape1.setPosition(srcPoint);
                            lineShape2.setPosition(srcPoint);
                            float angle = std::atan2(delta.y, delta.x) * (180.f / static_cast<float>(M_PI));
                            lineShape1.setRotation(angle);
                            lineShape2.setRotation(angle);
                            lineShape1.setFillColor(sf::Color::Red);
                            lineShape2.setFillColor(sf::Color::Red);
                            evaluationImg.internalTexture.draw(lineShape1);
                            evaluationImg.internalTexture.draw(lineShape2);

                            //points
                            sf::CircleShape srcCircle(evaluationImg.pointSize);
                            srcCircle.setPosition(srcPoint);
                            srcCircle.move(-evaluationImg.pointSize, -evaluationImg.pointSize);
                            evaluationImg.internalTexture.draw(srcCircle);

                            sf::CircleShape dstCircle(evaluationImg.pointSize);
                            dstCircle.setPosition(dstPoint);
                            dstCircle.move(-evaluationImg.pointSize, -evaluationImg.pointSize);
                            dstCircle.setFillColor(sf::Color::Red);
                            evaluationImg.internalTexture.draw(dstCircle);

                        }
                    }


                    evaluationImg.internalTexture.display();
                    window.draw(internalSprite);
                } else if (currentDistortImage == secondImage) {
                    evaluationImg.internalTexture.clear();
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(evaluationImg.internalTexture.getTexture());

                    firstIm.sprite.setPosition(translate);
                    firstIm.sprite.setScale(zoom, zoom);
                    evaluationImg.internalTexture.draw(firstIm.sprite);

                    evaluationImg.sprite.setPosition(translate);
                    evaluationImg.sprite.setScale(zoom, zoom);
                    evaluationImg.sprite.setColor(sf::Color(255, 255, 255, evaluationImg.transparency));
                    evaluationImg.internalTexture.draw(evaluationImg.sprite);

                    if (evaluationImg.showPoints) {
                        for (std::size_t i = 0; i < secondIm.coordPoints.size(); ++i) {
                            sf::Vector2f srcPoint(firstIm.sprite.getTransform().transformPoint(firstIm.coordPoints[i]));
                            sf::Vector2f dstPoint(evaluationImg.sprite.getTransform().transformPoint(evaluationImg.coordPoints[i]));

                            //lines
                            sf::Vector2f delta = dstPoint - srcPoint;
                            sf::RectangleShape lineShape1;
                            sf::RectangleShape lineShape2;
                            lineShape1.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y), evaluationImg.lineWidth));
                            lineShape2.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y), -evaluationImg.lineWidth));
                            lineShape1.setPosition(srcPoint);
                            lineShape2.setPosition(srcPoint);
                            float angle = std::atan2(delta.y, delta.x) * (180.f / static_cast<float>(M_PI));
                            lineShape1.setRotation(angle);
                            lineShape2.setRotation(angle);
                            lineShape1.setFillColor(sf::Color::Red);
                            lineShape2.setFillColor(sf::Color::Red);
                            evaluationImg.internalTexture.draw(lineShape1);
                            evaluationImg.internalTexture.draw(lineShape2);

                            //points
                            sf::CircleShape srcCircle(evaluationImg.pointSize);
                            srcCircle.setPosition(srcPoint);
                            srcCircle.move(-evaluationImg.pointSize, -evaluationImg.pointSize);
                            evaluationImg.internalTexture.draw(srcCircle);

                            sf::CircleShape dstCircle(evaluationImg.pointSize);
                            dstCircle.setPosition(dstPoint);
                            dstCircle.move(-evaluationImg.pointSize, -evaluationImg.pointSize);
                            dstCircle.setFillColor(sf::Color::Red);
                            evaluationImg.internalTexture.draw(dstCircle);
                        }
                    }

                    evaluationImg.internalTexture.display();
                    window.draw(internalSprite);
                }
                break;
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    void run() {
        ImGui::SFML::Init(window);
        while (window.isOpen()) {

            switch (currentState) {
                case mainScreen:
                    handleInput();

                case DistortionEvaluationScreen:
                    handleInputCastom();
            }
            update();
            render();

        }
        ImGui::SFML::Shutdown();
    }
};

int main() {
    App app;
    app.run();

    return 0;
}
