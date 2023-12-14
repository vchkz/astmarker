#include <SFML/Graphics.hpp>
#ifndef ASTMARKER_IMAGE_H
#define ASTMARKER_IMAGE_H
class Image {
public:
    Image();
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderTexture internalTexture;
    bool is_opened = false;
    void openImage();
};
#endif //ASTMARKER_IMAGE_H
