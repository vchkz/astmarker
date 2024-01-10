#include "imageEval.h"
#include "iostream"

#define M_PI 3.14159265358979323846


void ImageEval::genEvalImg(const Image &srcImg, const Image &dstImg) {

    std::vector<cv::Point2f> src_img_points;
    std::vector<cv::Point2f> dst_img_points;

    for (const auto& point : srcImg.ast.getPts()) {
        src_img_points.emplace_back(point.x, point.y);
    }
    for (const auto& point : dstImg.ast.getPts()) {
        dst_img_points.emplace_back(point.x, point.y);
    }


    sf::Image tempImage = srcImg.texture.copyToImage();
    cv::Size size(tempImage.getSize().x, tempImage.getSize().y);
    cv::Mat image(size,CV_8UC4, (void*)tempImage.getPixelsPtr(), cv::Mat::AUTO_STEP);

    cv::Mat warped_img;

    //warping points
//    coordPoints.clear();
    ast = Asterism(size.width, size.height);

    std::vector<cv::Point2f> cvPoints;
    for (const auto& point : srcImg.ast.getPts()) {
        cvPoints.emplace_back(point.x, point.y);
    }
    std::vector<cv::Point2f> transformedPoints;

    switch (currentModelType) {
        case affine:
            transformMatrix = cv::estimateAffine2D(src_img_points, dst_img_points, cv::noArray(), cv::RANSAC);
            transformMatrix.convertTo(transformMatrix, CV_32F);
            cv::warpAffine(image, warped_img, transformMatrix, image.size());

            cv::transform(cvPoints, transformedPoints, transformMatrix);

            break;

            //warping points




        case perspective:
            transformMatrix = cv::findHomography(src_img_points, dst_img_points, cv::RANSAC);
            transformMatrix.convertTo(transformMatrix, CV_32F);
            cv::warpPerspective(image, warped_img, transformMatrix, image.size());

            cv::perspectiveTransform(cvPoints, transformedPoints, transformMatrix);



            break;
    }

    for (const auto& point : transformedPoints) {
        ast.insertPt(point);
//        coordPoints.emplace_back(point.x, point.y);
    }


    texture.create(warped_img.cols, warped_img.rows);
    texture.update(warped_img.data);
    sprite.setTexture(texture);
    is_opened = true;




}

//void ImageEval::drawEvalImg(Image ovelayImg) {
//    internalTexture.clear();
//
//}

void ImageEval::drawEvalPoints(Image baseImg) {
    if (showPoints) {
        for (std::size_t i = 0; i < ast.getPts().size(); ++i) {
            cv::Point srcPointCV(baseImg.ast.getPts()[i]);
            cv::Point dstPointCV(ast.getPts()[i]);
            sf::Vector2f srcPoint(
                    baseImg.sprite.getTransform().transformPoint(srcPointCV.x, srcPointCV.y));
            sf::Vector2f dstPoint(
                    sprite.getTransform().transformPoint(dstPointCV.x, dstPointCV.y));

            //lines
            sf::Vector2f delta = dstPoint - srcPoint;
            sf::RectangleShape lineShape1;
            sf::RectangleShape lineShape2;
            lineShape1.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y),
                                            lineWidth));
            lineShape2.setSize(sf::Vector2f(std::sqrt(delta.x * delta.x + delta.y * delta.y),
                                            -lineWidth));
            lineShape1.setPosition(srcPoint);
            lineShape2.setPosition(srcPoint);
            float angle = std::atan2(delta.y, delta.x) * (180.f / static_cast<float>(M_PI));
            lineShape1.setRotation(angle);
            lineShape2.setRotation(angle);
            lineShape1.setFillColor(sf::Color::Red);
            lineShape2.setFillColor(sf::Color::Red);
            internalTexture.draw(lineShape1);
            internalTexture.draw(lineShape2);

            //points
            sf::CircleShape srcCircle(pointSize);
            srcCircle.setPosition(srcPoint);
            srcCircle.setFillColor(sf::Color::Blue);
            srcCircle.move(-pointSize, -pointSize);
            internalTexture.draw(srcCircle);

            sf::CircleShape dstCircle(pointSize);
            dstCircle.setPosition(dstPoint);
            dstCircle.move(-pointSize, -pointSize);
            dstCircle.setFillColor(sf::Color::Red);
            internalTexture.draw(dstCircle);

        }
    }
}



