#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#ifndef ASTMARKER_IMAGE_H
#define ASTMARKER_IMAGE_H
class Image {
public:
    Image();
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderTexture internalTexture;
    std::vector<sf::Vector2f> coordPoints;
    int hoveredCircleIndex = -1;
    int selectedCircleIndex = -1;
    float scaleFactor = 1.0f;

    bool is_opened = false;

    void openImage();

    void genWarpImg(const Image &firstSourceImage, const Image &secondSourceImage);
    sf::Vector2f predictPoint(sf::Vector2<float> pointCoord, const Image &srcImage);
};
#endif //ASTMARKER_IMAGE_H
