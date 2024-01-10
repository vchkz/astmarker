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

    float CircleRadius = 10;
    float transparency = 0.5f;

    ImageEval evaluationImg;

    bool isDirty = true;

    void setDirty() { isDirty = true; }

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

        firstIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
        secondIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
        thirdIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
        evaluationImg.pointsTexture.create(window.getSize().x, window.getSize().y);

    }

//    void handleInputCastom() {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            ImGui::SFML::ProcessEvent(event);
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            } else if (event.type == sf::Event::MouseWheelScrolled) { // Масштабирование колёсиком мыши
//                auto oldZoom = zoom;
//
//                if (event.mouseWheelScroll.delta > 0) zoom *= 1.1f;
//                else zoom /= 1.1f;
//
//
//
//            } else if (event.type == sf::Event::MouseMoved) {
//                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);
//
//                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
//                    translate += newCoord - mouseCoord;
//                }
//                mouseCoord = newCoord;
//
//            } else if (event.type == sf::Event::Resized) {
//                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
//                window.setView(sf::View(visibleArea));
//                firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
//                secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
//                thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
//                evaluationImg.internalTexture.create(window.getSize().x, window.getSize().y);
//
//            }
//        }
//    };

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
                firstIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
                secondIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
                thirdIm.pointsTexture.create(window.getSize().x / 3, window.getSize().y);
                evaluationImg.pointsTexture.create(window.getSize().x, window.getSize().y);
                evaluationImg.internalTexture.create(window.getSize().x, window.getSize().y);
                firstIm.drawPicture();
                secondIm.drawPicture();
                thirdIm.drawPicture(secondIm.sprite, transparency);
                firstIm.drawCircles(CircleRadius);
                secondIm.drawCircles(CircleRadius);
                thirdIm.drawCircles(CircleRadius);
            } else if (event.type == sf::Event::MouseWheelScrolled) { // Масштабирование колёсиком мыши

                if (event.mouseWheelScroll.delta > 0) zoom *= 1.1f;
                else zoom /= 1.1f;

                firstIm.sprite.setScale(zoom, zoom);
                secondIm.sprite.setScale(zoom, zoom);
                thirdIm.sprite.setScale(zoom, zoom);
                evaluationImg.sprite.setScale(zoom, zoom);
                firstIm.drawPicture();
                secondIm.drawPicture();
                thirdIm.drawPicture(secondIm.sprite, transparency);
                evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);
                firstIm.drawCircles(CircleRadius);
                secondIm.drawCircles(CircleRadius);
                thirdIm.drawCircles(CircleRadius);
                setDirty();
            } else if (event.type == sf::Event::MouseMoved and sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);
                translate += newCoord - mouseCoord;
                firstIm.sprite.setPosition(translate);
                secondIm.sprite.setPosition(translate);
                thirdIm.sprite.setPosition(translate);
                evaluationImg.sprite.setPosition(translate);
                firstIm.drawPicture();
                secondIm.drawPicture();
                thirdIm.drawPicture(secondIm.sprite, transparency);
                evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);
                firstIm.drawCircles(CircleRadius);
                secondIm.drawCircles(CircleRadius);
                thirdIm.drawCircles(CircleRadius);
                mouseCoord = newCoord;
            } else if (event.type == sf::Event::MouseMoved and
                       !sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { // Движение мыши без зажатого пробела

                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);


                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                sf::Vector2f mousePosFloat4Second(mousePosFloat.x - static_cast<float>(window.getSize().x) / 3.0f,
                                                  mousePosFloat.y);
                sf::Vector2f mousePosFloat4SecondTemp(mouseCoord.x - static_cast<float>(window.getSize().x) / 3.0f,
                                                      mouseCoord.y);

                // Обработка hover для первой трети окна
                if (!pointMapping) {
                    if ((mousePos.x < window.getSize().x / 3) and firstIm.is_opened and secondIm.is_opened) {
                        int oldHoveredCircleIndex = firstIm.hoveredCircleIndex;
                        bool loopCompleted = true;
                        for (int i = 0; i < firstIm.ast.getPts().size(); ++i) {
                            sf::Vector2f circleCenter(
                                    firstIm.sprite.getTransform().transformPoint(firstIm.ast.getPts()[i].x,
                                                                                 firstIm.ast.getPts()[i].y));

                            if (std::pow(mousePosFloat.x - circleCenter.x, 2) +
                                std::pow(mousePosFloat.y - circleCenter.y, 2) <= std::pow(CircleRadius, 2)) {
                                firstIm.hoveredCircleIndex = i;
                                secondIm.hoveredCircleIndex = i;
                                loopCompleted = false;
                                break;
                            }
                        }
                        if (loopCompleted) {
                            firstIm.hoveredCircleIndex = -1;
                            secondIm.hoveredCircleIndex = -1;
                        }

                        if (oldHoveredCircleIndex != firstIm.hoveredCircleIndex) {
                            firstIm.drawCircles(CircleRadius);
                            secondIm.drawCircles(CircleRadius);
                        }
                    }
                    // Обработка hover для второй трети окна
                    if (mousePos.x > window.getSize().x / 3 and mousePos.x < window.getSize().x / 3 * 2 and
                        secondIm.is_opened) {
                        int oldHoveredCircleIndex = secondIm.hoveredCircleIndex;
                        bool loopCompleted = true;
                        for (int i = 0; i < secondIm.ast.getPts().size(); ++i) {
                            sf::Vector2f circleCenter(
                                    secondIm.sprite.getTransform().transformPoint(secondIm.ast.getPts()[i].x,
                                                                                  secondIm.ast.getPts()[i].y));
                            if (std::pow(mousePosFloat4Second.x - circleCenter.x, 2) +
                                std::pow(mousePosFloat4Second.y - circleCenter.y, 2) <= std::pow(CircleRadius, 2)) {
                                firstIm.hoveredCircleIndex = i;
                                secondIm.hoveredCircleIndex = i;
                                loopCompleted = false;
                                break;
                            }
                        }
                        if (loopCompleted) {
                            firstIm.hoveredCircleIndex = -1;
                            secondIm.hoveredCircleIndex = -1;
                        }
                        if (oldHoveredCircleIndex != secondIm.hoveredCircleIndex) {
                            firstIm.drawCircles(CircleRadius);
                            secondIm.drawCircles(CircleRadius);
                        }
                    }
                }


                if (firstIm.selectedCircleIndex != -1) {
                    if (mousePos.x > window.getSize().x / 3) {
                        mousePosFloat -= sf::Vector2f(window.getSize().x / 3, 0);
                    }
                    sf::Vector2i position(firstIm.sprite.getInverseTransform().transformPoint(mousePosFloat));
                    firstIm.ast.setPosition(firstIm.selectedCircleIndex, cv::Point(position.x, position.y));
//                        sf::Vector2i offset(sf::Vector2i(firstIm.sprite.getInverseTransform().transformPoint(mousePosFloat)) -
//                                                    sf::Vector2i(firstIm.sprite.getInverseTransform().transformPoint(mouseCoord)));
//                        firstIm.ast.movePt(firstIm.selectedCircleIndex, cv::Point(offset.x, offset.y));
                    firstIm.drawCircles(CircleRadius);
                }
                if (secondIm.selectedCircleIndex != -1) {
                    if (mousePos.x > window.getSize().x / 3) {
                        mousePosFloat -= sf::Vector2f(window.getSize().x / 3, 0);
                    }
                    sf::Vector2i position(secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat));
                    secondIm.ast.setPosition(secondIm.selectedCircleIndex, cv::Point(position.x, position.y));
//                        sf::Vector2i offset(secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat) -
//                                                    secondIm.sprite.getInverseTransform().transformPoint(mouseCoord));
//                        secondIm.ast.movePt(secondIm.selectedCircleIndex, cv::Point(offset.x, offset.y));
                    secondIm.drawCircles(CircleRadius);
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
                            secondIm.ast.insertPt(
                                    secondIm.ast.predictPairedPt(cv::Point(point.x, point.y), firstIm.ast));
                            firstIm.ast.insertPt(cv::Point(point.x, point.y));
                            secondIm.selectedCircleIndex = secondIm.ast.getPts().size() - 1;
                            firstIm.hoveredCircleIndex = firstIm.ast.getPts().size() - 1;
                            secondIm.hoveredCircleIndex = firstIm.ast.getPts().size() - 1;
                            firstIm.drawCircles(CircleRadius);
                            secondIm.drawCircles(CircleRadius);
                            setDirty();
                            pointMapping = true;
                        }
                    } else if (mousePos.x > window.getSize().x / 3 and mousePos.x < window.getSize().x / 3 * 2 and
                               (mousePos.y > 30) and secondIm.is_opened) {
                        mousePosFloat.x += -static_cast<float>(window.getSize().x) / 3.0f;
                        secondIm.selectedCircleIndex = secondIm.hoveredCircleIndex;


                        if (secondIm.selectedCircleIndex == -1 and
                            secondIm.sprite.getGlobalBounds().contains(mousePosFloat)) {
                            sf::Vector2f point = secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat);
                            firstIm.ast.insertPt(
                                    firstIm.ast.predictPairedPt(cv::Point(point.x, point.y), secondIm.ast));
                            secondIm.ast.insertPt(cv::Point(point.x, point.y));
                            firstIm.selectedCircleIndex = firstIm.ast.getPts().size() - 1;
                            firstIm.hoveredCircleIndex = firstIm.ast.getPts().size() - 1;
                            secondIm.hoveredCircleIndex = firstIm.ast.getPts().size() - 1;
                            firstIm.drawCircles(CircleRadius);
                            secondIm.drawCircles(CircleRadius);
                            setDirty();
                            pointMapping = true;
                        }
                    }
                }

            } else if (event.type == sf::Event::MouseButtonReleased and !pointMapping) {
                if (firstIm.selectedCircleIndex != -1 or secondIm.selectedCircleIndex != -1) {
                    firstIm.selectedCircleIndex = -1;
                    secondIm.selectedCircleIndex = -1;
                    thirdIm.genWarpImg(firstIm, secondIm);
                    thirdIm.drawPicture(secondIm.sprite, transparency);
                    setDirty();
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Delete) {
                    if (firstIm.hoveredCircleIndex != -1 or secondIm.hoveredCircleIndex != -1) {
                        int index;
                        if (firstIm.hoveredCircleIndex != -1) { index = firstIm.hoveredCircleIndex; }
                        else if (secondIm.hoveredCircleIndex != -1) { index = secondIm.hoveredCircleIndex; }
                        firstIm.ast.deletePt(index);
                        secondIm.ast.deletePt(index);

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
                    setDirty();
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
                if (ImGui::MenuItem("Warp image", nullptr, false, firstIm.ast.getPts().size() > 3)) {
                    currentState = DistortionEvaluationScreen;
                    evaluationImg.genEvalImg(firstIm, secondIm);
                    evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);
                    showEvaluationSettingsWindow = true;
                    zoom = 1;
                    translate = sf::Vector2f(0, 0);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", nullptr, false, currentState == DistortionEvaluationScreen)) {
                    currentState = mainScreen;
                    showEvaluationSettingsWindow = false;
                }
                ImGui::EndMenu();
            }

        }
        ImGui::EndMainMenuBar();


        if (showEvaluationSettingsWindow) {
            ImGui::Begin("Model Settings", &showEvaluationSettingsWindow);
            ImGui::Separator();
            ImGui::Text("Type of transformation:");
            if (ImGui::RadioButton("Affine", (int *) &evaluationImg.currentModelType, evaluationImg.affine)) {
                if (currentDistortImage == firstImage){evaluationImg.genEvalImg(firstIm, secondIm);evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);}
                if (currentDistortImage == secondImage){evaluationImg.genEvalImg(secondIm, firstIm);evaluationImg.drawPicture(firstIm.sprite, evaluationImg.transparency);}
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Perspective", (int *) &evaluationImg.currentModelType, evaluationImg.perspective)) {
                if (currentDistortImage == firstImage){evaluationImg.genEvalImg(firstIm, secondIm);evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);}
                if (currentDistortImage == secondImage){evaluationImg.genEvalImg(secondIm, firstIm);evaluationImg.drawPicture(firstIm.sprite, evaluationImg.transparency);}
            }
            ImGui::Separator();
            ImGui::Text("Which image should be warped:");
            if (ImGui::RadioButton("First", (int *) &currentDistortImage, firstImage)) {
                evaluationImg.genEvalImg(firstIm, secondIm);
                evaluationImg.drawPicture(secondIm.sprite, evaluationImg.transparency);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Second", (int *) &currentDistortImage, secondImage)) {
                evaluationImg.genEvalImg(secondIm, firstIm);
                evaluationImg.drawPicture(firstIm.sprite, evaluationImg.transparency);
            }
            ImGui::Separator();
            ImGui::SliderFloat("Transparency", &evaluationImg.transparency, 0.01f, 1.0f);

            ImGui::Separator();
            ImGui::Checkbox("Show points", &evaluationImg.showPoints);
            ImGui::SliderFloat("Point size", &evaluationImg.pointSize, 1.0f, 20.0f);
            ImGui::SliderFloat("Line wight", &evaluationImg.lineWidth, 1.0f, 20.0f);

            ImGui::End();
        }


        ImGui::Begin("edit", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::SetWindowPos(ImVec2(0.0f, window.getSize().y - 30.0f));
            ImGui::SetWindowSize(ImVec2(window.getSize().x, 30.0f));
            ImGui::Columns(4);
            ImGui::SliderFloat("Point width", &CircleRadius, 1.0f, 20.0f);
            ImGui::NextColumn();
            if (ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f))
                thirdIm.drawPicture(secondIm.sprite, transparency);
            ImGui::NextColumn();
            ImGui::Text("1 hoverID: %d", firstIm.hoveredCircleIndex);
            ImGui::NextColumn();
            ImGui::Text("2 hoverID: %d", secondIm.hoveredCircleIndex);
        }
        ImGui::End();


//        ImGui::Begin("statusbar", nullptr,
//                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
//                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
//        {
//            ImGui::SetWindowPos(ImVec2(0.0f, window.getSize().y - 30.0f));
//            ImGui::SetWindowSize(ImVec2(window.getSize().x, 30.0f));
//
//            ImGui::Columns(8);
//            ImGui::Text("win size: %d %d", window.getSize().x, window.getSize().y);
//            ImGui::NextColumn();
//            ImGui::Text("translate: %f %f", translate.x, translate.y);
//            ImGui::NextColumn();
//            ImGui::Text("zoom: %f", zoom);
//            ImGui::NextColumn();
//            ImGui::Text("mouse: %d %d", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
//            ImGui::NextColumn();
//            ImGui::Text("1 hoverID: %d", firstIm.hoveredCircleIndex);
//            ImGui::NextColumn();
//            ImGui::Text("2 hoverID: %d", secondIm.hoveredCircleIndex);
//            ImGui::NextColumn();
//            ImGui::Text("1 selectID: %d", firstIm.selectedCircleIndex);
//            ImGui::NextColumn();
//            ImGui::Text("2 selectID: %d", secondIm.selectedCircleIndex);
//        }
//        ImGui::End();
    }

    void render() {
//        if (!isDirty)return;

        window.clear();
        switch (currentState) {
            case mainScreen:
                if (firstIm.is_opened) {
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(firstIm.internalTexture.getTexture());
                    sf::Sprite internalPointSprite;
                    internalPointSprite.setTexture(firstIm.pointsTexture.getTexture());
                    firstIm.internalTexture.display();
                    window.draw(internalSprite);
                    firstIm.pointsTexture.display();
                    window.draw(internalPointSprite);
                }
                if (secondIm.is_opened) {
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(secondIm.internalTexture.getTexture());
                    internalSprite.move(window.getSize().x / 3, 0);
                    sf::Sprite internalPointSprite;
                    internalPointSprite.setTexture(secondIm.pointsTexture.getTexture());
                    internalPointSprite.move(window.getSize().x / 3, 0);
                    secondIm.internalTexture.display();
                    window.draw(internalSprite);
                    secondIm.pointsTexture.display();
                    window.draw(internalPointSprite);
                }
                if (thirdIm.is_opened) {
                    sf::Sprite internalSprite;
                    internalSprite.setTexture(thirdIm.internalTexture.getTexture());
                    internalSprite.move(window.getSize().x / 3 * 2, 0);

                    thirdIm.internalTexture.display();
                    window.draw(internalSprite);
                }

                isDirty = false;
                break;


            case DistortionEvaluationScreen:
                sf::Sprite internalSprite;
                internalSprite.setTexture(evaluationImg.internalTexture.getTexture());
                evaluationImg.internalTexture.display();
                window.draw(internalSprite);
                break;
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    void run() {
        ImGui::SFML::Init(window);
        while (window.isOpen()) {
            handleInput();

//            switch (currentState) {
//                case mainScreen:
//                    handleInput();
//
//                case DistortionEvaluationScreen:
//                    handleInputCastom();
//            }
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
