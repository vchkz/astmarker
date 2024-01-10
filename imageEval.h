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
    void drawEvalImg(Image ovelayImg);
    void drawEvalPoints(Image baseImg);
    bool showPoints = true;

    enum modelType {
        affine,
        perspective
    } currentModelType = perspective;

    cv::Mat transformMatrix;
    void genEvalImg(const Image &srcImg, const Image &dstImg);
//    void transformPoints(std::vector<sf::Vector2f> points);



};

#endif //ASTMARKER_IMAGEEVAL_H
