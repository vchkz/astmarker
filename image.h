#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "asterisms.h"
#ifndef ASTMARKER_IMAGE_H
#define ASTMARKER_IMAGE_H
class Image {
public:
    Image();
    Asterism ast;


    sf::Sprite sprite;
    sf::RenderTexture internalTexture;
    sf::RenderTexture pointsTexture;

    int hoveredCircleIndex = -1;
    int selectedCircleIndex = -1;

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

    sf::Texture texture;
};
#endif //ASTMARKER_IMAGE_H
