#include <SFML/Graphics.hpp>
#ifndef ASTMARKER_IMAGE_H
#define ASTMARKER_IMAGE_H
class Image {
public:
    Image();
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderTexture internalTexture;
    std::vector<sf::CircleShape> drawPoints;
    int hoveredCircleIndex = -1;
    int selectedCircleIndex = -1;

    bool is_opened = false;
    void openImage();

    void genWarpImg(const Image &firstSourceImage, const Image &secondSourceImage);
    void predictPoint(sf::Vector2<float> pointCoord, const Image &srcImage, const float &radius);
};
#endif //ASTMARKER_IMAGE_H
