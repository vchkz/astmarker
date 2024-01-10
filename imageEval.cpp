#include "imageEval.h"
#include "iostream"

#define M_PI 3.14159265358979323846


void ImageEval::genEvalImg(const Image &srcImg, const Image &dstImg) {

    std::vector<cv::Point2f> src_img_points;
    std::vector<cv::Point2f> dst_img_points;

    for (int i = 0; i < srcImg.ast.countPts(); ++i) {
        src_img_points.emplace_back(srcImg.ast.getPosition(i));
    }
    for (int i = 0; i < dstImg.ast.countPts(); ++i) {
        dst_img_points.emplace_back(dstImg.ast.getPosition(i));
    }




    sf::Image tempImage = srcImg.texture.copyToImage();
    cv::Size size(tempImage.getSize().x, tempImage.getSize().y);
    cv::Mat image(size,CV_8UC4, (void*)tempImage.getPixelsPtr(), cv::Mat::AUTO_STEP);
    cv::Mat warped_img;

    //warping points
    ast = Asterism(cv::Rect(0, 0, size.width, size.height));


    std::vector<cv::Point2f> transformedPoints;

    switch (currentModelType) {
        case affine:
            transformMatrix = cv::estimateAffine2D(src_img_points, dst_img_points, cv::noArray(), cv::RANSAC);
            transformMatrix.convertTo(transformMatrix, CV_32F);
            cv::warpAffine(image, warped_img, transformMatrix, image.size());
            cv::transform(src_img_points, transformedPoints, transformMatrix);
            break;
            //warping points




        case perspective:
            transformMatrix = cv::findHomography(src_img_points, dst_img_points, cv::RANSAC);
            transformMatrix.convertTo(transformMatrix, CV_32F);
            cv::warpPerspective(image, warped_img, transformMatrix, image.size());

            cv::perspectiveTransform(src_img_points, transformedPoints, transformMatrix);

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

    drawPicture(dstImg.sprite);
    drawEvalPoints(dstImg.ast);
}



void ImageEval::drawEvalPoints(const Asterism& baseAst) {
    if (showPoints) {
        pointsTexture.clear(sf::Color(0,0,0,0));
        for (std::size_t i = 0; i < ast.countPts(); ++i) {
            cv::Point srcPointCV(baseAst.getPosition(i));
            cv::Point dstPointCV(ast.getPosition(i));
            sf::Vector2f srcPoint(
                    sprite.getTransform().transformPoint(srcPointCV.x, srcPointCV.y));
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
            pointsTexture.draw(lineShape1);
            pointsTexture.draw(lineShape2);

            //points
            sf::CircleShape srcCircle(pointRadius);
            srcCircle.setPosition(srcPoint);
            srcCircle.setFillColor(sf::Color::Blue);
            srcCircle.move(-pointRadius, -pointRadius);
            pointsTexture.draw(srcCircle);

            sf::CircleShape dstCircle(pointRadius);
            dstCircle.setPosition(dstPoint);
            dstCircle.move(-pointRadius, -pointRadius);
            dstCircle.setFillColor(sf::Color::Red);
            pointsTexture.draw(dstCircle);

        }
    }
}



