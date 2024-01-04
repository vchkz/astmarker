#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include "image.h"



struct App {
    sf::RenderWindow window;

    sf::Vector2f translate;
    sf::Vector2f mouseCoord;

    Image firstIm;
    Image secondIm;
    Image thirdIm;

    const float CircleRadius = 10;

    float zoom;

    App() : window(sf::VideoMode(600, 600), "SFML Image Viewer"), zoom(1.0f) {

        firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
        secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
        thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);




    }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Если нет желания каждый раз открывать картинки, то пошеруди в image.cpp, там в openfunnyman() настрой че как надо
            // Этот блок кода я ввел потому что мне было лень каждый раз открывать картинки вручную
            /////////////////////////////////////
            /*if (firstIm.is_opened == false) {
                firstIm.openFunnyMan();
                secondIm.openFunnyMan();
            }*/
            /////////////////////////////////////
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                firstIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                secondIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);
                thirdIm.internalTexture.create(window.getSize().x / 3, window.getSize().y);

            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                auto oldZoom = zoom;
                // Масштабирование колёсиком мыши
                if (event.mouseWheelScroll.delta > 0) {
                    zoom *= 1.1f;
                } else {
                    zoom /= 1.1f;
                }
                auto centerX =  firstIm.internalTexture.getSize().x / 2;
                auto centerY =  firstIm.internalTexture.getSize().y / 2;
                sf::Vector2f temp(((centerX) * (oldZoom - zoom) / 2), ((centerY) * (oldZoom - zoom) / 2));
                translate += temp;

            }
            // Условие для удаления точки. ХЗ как еще реализовать без использования контекстного меню и с таким кодом.
            // Буду только рад улучшениям
            ///////////////////////////////////////////////////////
            if (firstIm.drawPoints.size() != 0) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                sf::Vector2f mousePosFloat4Second(mousePosFloat.x - static_cast<float>(window.getSize().x) / 3.0f, mousePosFloat.y);
                
                if ((mousePos.x < window.getSize().x / 3) && firstIm.is_opened && secondIm.is_opened) {
                    for (std::size_t i = 0; i < firstIm.drawPoints.size(); ++i) {
                        sf::Rect pointBounds = firstIm.drawPoints[i].getGlobalBounds();
                        sf::Vector2f pointSize(firstIm.sprite.getTransform().transformPoint(pointBounds.left, pointBounds.top));
                        pointBounds.left = pointSize.x;
                        pointBounds.top = pointSize.y;
                        if (pointBounds.contains(mousePosFloat) && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            firstIm.drawPoints.erase(firstIm.drawPoints.begin() + i);
                            secondIm.drawPoints.erase(secondIm.drawPoints.begin() + i);
                        }
                    }
                }
                else if (mousePos.x > window.getSize().x / 3 && mousePos.x < window.getSize().x / 3 * 2 && secondIm.is_opened){ 
                    for (std::size_t i = 0; i < secondIm.drawPoints.size(); ++i) {
                        sf::Rect pointBounds = secondIm.drawPoints[i].getGlobalBounds();
                        sf::Vector2f pointSize(secondIm.sprite.getTransform().transformPoint(pointBounds.left, pointBounds.top));
                        pointBounds.left = pointSize.x;
                        pointBounds.top = pointSize.y;
                        if (pointBounds.contains(mousePosFloat4Second) && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            secondIm.drawPoints.erase(secondIm.drawPoints.begin() + i);
                            firstIm.drawPoints.erase(firstIm.drawPoints.begin() + i);
                        }
                    }
                }
            }
            ///////////////////////////////////////////////////////////////
            if (event.type == sf::Event::MouseMoved) {
                // Движение мыши
                sf::Vector2f newCoord(event.mouseMove.x, event.mouseMove.y);

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                sf::Vector2f mousePosFloatCen(static_cast<float>(mousePos.x - CircleRadius), static_cast<float>(mousePos.y - CircleRadius));
                sf::Vector2f mousePosFloat4Second(mousePosFloat.x -static_cast<float>(window.getSize().x) / 3.0f, mousePosFloat.y);
                sf::Vector2f mousePosFloat4SecondCen(mousePosFloatCen.x - static_cast<float>(window.getSize().x) / 3.0f, mousePosFloatCen.y);


                if ((mousePos.x < window.getSize().x / 3) && firstIm.is_opened && secondIm.is_opened){
                    bool loopCompleted = true;
                    for (std::size_t i = 0; i < firstIm.drawPoints.size(); ++i) {
                        sf::Rect pointBounds = firstIm.drawPoints[i].getGlobalBounds();
                        sf::Vector2f pointSize(firstIm.sprite.getTransform().transformPoint(pointBounds.left, pointBounds.top));
                        pointBounds.left = pointSize.x;
                        pointBounds.top = pointSize.y;
                        if (pointBounds.contains(mousePosFloat)) {
                            firstIm.hoveredCircleIndex = static_cast<int>(i);
                            loopCompleted = false;
                            break;
                        }
                    }
                    if (loopCompleted){firstIm.hoveredCircleIndex = -1;}
                }
                if(mousePos.x > window.getSize().x / 3 && mousePos.x < window.getSize().x / 3 * 2 && secondIm.is_opened){
                    bool loopCompleted = true;
                    for (std::size_t i = 0; i < secondIm.drawPoints.size(); ++i) {
                        sf::Rect pointBounds = secondIm.drawPoints[i].getGlobalBounds();
                        sf::Vector2f pointSize(secondIm.sprite.getTransform().transformPoint(pointBounds.left, pointBounds.top));
                        pointBounds.left = pointSize.x;
                        pointBounds.top = pointSize.y;
                        if (pointBounds.contains(mousePosFloat4Second)) {
                            secondIm.hoveredCircleIndex = static_cast<int>(i);
                            loopCompleted = false;
                            break;
                        }
                    }
                    if (loopCompleted){secondIm.hoveredCircleIndex = -1;}
                }

                if (firstIm.selectedCircleIndex != -1) {
                    firstIm.drawPoints[firstIm.selectedCircleIndex].setPosition(firstIm.sprite.getInverseTransform().transformPoint(mousePosFloatCen));

                }
                if (secondIm.selectedCircleIndex != -1) {
                    secondIm.drawPoints[secondIm.selectedCircleIndex].setPosition(secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat4SecondCen));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    translate += newCoord - mouseCoord;
                }
                mouseCoord = newCoord;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x - CircleRadius), static_cast<float>(mousePos.y - CircleRadius));

                if ((mousePos.x < window.getSize().x / 3) && firstIm.is_opened && secondIm.is_opened) {
                        firstIm.selectedCircleIndex = firstIm.hoveredCircleIndex;
                        if (true) {
                            if (firstIm.selectedCircleIndex == -1) {
                                sf::CircleShape newCircle(CircleRadius);
                                newCircle.setFillColor(sf::Color::Red);
                                newCircle.setPosition(firstIm.sprite.getInverseTransform().transformPoint(mousePosFloat));
                                secondIm.predictPoint(newCircle.getPosition(), firstIm, CircleRadius);
                                firstIm.drawPoints.push_back(newCircle);
                            }
                        }
                }
                else if (mousePos.x > window.getSize().x / 3 && mousePos.x < window.getSize().x / 3 * 2 && secondIm.is_opened && firstIm.is_opened) {
                        mousePosFloat.x += -static_cast<float>(window.getSize().x) / 3.0f;
                        secondIm.selectedCircleIndex = secondIm.hoveredCircleIndex;
                        if (true) {
                            if (secondIm.selectedCircleIndex == -1) {
                                sf::CircleShape newCircle(CircleRadius);
                                newCircle.setFillColor(sf::Color::Red);
                                newCircle.setPosition(secondIm.sprite.getInverseTransform().transformPoint(mousePosFloat));
                                firstIm.predictPoint(newCircle.getPosition(), secondIm, CircleRadius);
                                secondIm.drawPoints.push_back(newCircle);
                            }
                        }
                }
            } 
            if (event.type == sf::Event::MouseButtonReleased) {
                if (firstIm.selectedCircleIndex != -1 || secondIm.selectedCircleIndex != -1) {
                    firstIm.selectedCircleIndex = -1;
                    secondIm.selectedCircleIndex = -1;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                thirdIm.genWarpImg(firstIm, secondIm);
            }
        }
    }


    void update() {
        ImGui::SFML::Update(window, sf::seconds(1.0f / 60.0f));


        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Image 1")) firstIm.openImage();
                if (ImGui::MenuItem("Open Image 2")) secondIm.openImage();
                if (ImGui::MenuItem("Import a set of points", "CTRL+O", false, true));
                ImGui::Separator();
                if (ImGui::MenuItem("Save", "CTRL+S", false, true));
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) window.close();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }


        ImGui::Begin("statusbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            ImGui::SetWindowPos(ImVec2(0.0f, window.getSize().y - 30.0f));
            ImGui::SetWindowSize(ImVec2(window.getSize().x, 30.0f));

            ImGui::Columns(6);
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
        }
        ImGui::End();

    }

    void render() {
        window.clear();
        if (firstIm.is_opened){
            firstIm.internalTexture.clear();
            sf::Sprite internalSprite;
            internalSprite.setTexture(firstIm.internalTexture.getTexture());
            firstIm.sprite.setPosition(translate);
            firstIm.sprite.setScale(zoom, zoom);
            firstIm.internalTexture.draw(firstIm.sprite);

            for (std::size_t i = 0; i < firstIm.drawPoints.size(); ++i) {
                sf::CircleShape tempCircle = firstIm.drawPoints[i];
                tempCircle.setPosition(firstIm.sprite.getTransform().transformPoint(firstIm.drawPoints[i].getPosition()));
                if(firstIm.hoveredCircleIndex == i || secondIm.hoveredCircleIndex == i)tempCircle.setFillColor(sf::Color::Blue);
                firstIm.internalTexture.draw(tempCircle);
            }
            firstIm.internalTexture.display();
            window.draw(internalSprite);
        }
        if (secondIm.is_opened){
            secondIm.internalTexture.clear();
            sf::Sprite internalSprite;
            internalSprite.setTexture(secondIm.internalTexture.getTexture());
            internalSprite.move(window.getSize().x / 3, 0);
            secondIm.sprite.setPosition(translate);
            secondIm.sprite.setScale(zoom, zoom);

            secondIm.internalTexture.draw(secondIm.sprite);

            for (std::size_t i = 0; i < secondIm.drawPoints.size(); ++i) {
                sf::CircleShape tempCircle = secondIm.drawPoints[i];
                tempCircle.setPosition(secondIm.sprite.getTransform().transformPoint(secondIm.drawPoints[i].getPosition()));
                if(firstIm.hoveredCircleIndex == i || secondIm.hoveredCircleIndex == i)tempCircle.setFillColor(sf::Color::Blue);
                secondIm.internalTexture.draw(tempCircle);
            }
            secondIm.internalTexture.display();
            window.draw(internalSprite);
        }
        if (thirdIm.is_opened){
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


        ImGui::SFML::Render(window);
        window.display();

    }

    void run() {
        ImGui::SFML::Init(window);
        while (window.isOpen()) {
            handleInput();
            update();
            render();
        }
        ImGui::SFML::Shutdown();
    }
    ~App() {
        std::cout << firstIm.drawPoints.size() << " " << secondIm.drawPoints.size();
    }
};

int main() {
    App app;
    app.run();

    return 0;
}
