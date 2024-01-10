#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "asterisms.h"
#ifndef ASTMARKER_IMAGE_H
#define ASTMARKER_IMAGE_H
class Image {
public:
    Image();
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderTexture internalTexture;

    sf::RenderTexture pointsTexture;
    Asterism ast;
//    std::vector<sf::Vector2f> coordPoints;
    int hoveredCircleIndex = -1;
    int selectedCircleIndex = -1;
    float scaleFactor = 1.0f;

    bool is_opened = false;

    static float transparency;
    static float pointRadius;

    void openImage();
    void importPoints();
    void exportPoints();

    void drawCircles();
    void drawPicture();
    void drawPicture(sf::Sprite overlaySprite);

    void genWarpImg(const Image &firstSourceImage, const Image &secondSourceImage);
//    sf::Vector2f predictPoint(sf::Vector2<float> pointCoord, const Image &srcImage);
};
#endif //ASTMARKER_IMAGE_H
