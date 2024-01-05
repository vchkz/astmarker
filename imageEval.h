#ifndef ASTMARKER_IMAGEEVAL_H
#define ASTMARKER_IMAGEEVAL_H
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "image.h"
#include <vector>
class ImageEval : public Image{
public:
    float transparency = 0.5f;
    float lineWidth = 5.0f;
    float pointSize = 10.0f;

    enum modelType {
        affine = 0,
        perspective
    } currentModelType = affine;

    cv::Mat transformMatrix;
    void genImage(const sf::Texture& firstTexture, const sf::Texture& secondTexture);
    void genEvalImg(const Image &srcImg, const Image &dstImg);
    sf::Vector2f transformPoint(sf::Vector2f point);



};

#endif //ASTMARKER_IMAGEEVAL_H
