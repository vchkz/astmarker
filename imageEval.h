#ifndef ASTMARKER_IMAGEEVAL_H
#define ASTMARKER_IMAGEEVAL_H
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "image.h"
#include <vector>
class ImageEval : public Image{
public:
    float lineWidth = 5.0f;

    void drawEvalPoints(const Asterism& baseAst);
    bool showPoints = true;

    enum modelType {
        affine,
        perspective
    } currentModelType = perspective;

    void genEvalImg(const Image &srcImg, const Image &dstImg);

private:
    cv::Mat transformMatrix;
};

#endif //ASTMARKER_IMAGEEVAL_H
