#include "imageEval.h"


void ImageEval::genImage(const sf::Texture& firstTexture, const sf::Texture& secondTexture) {
    cv::Mat firstImg(cv::Size(firstTexture.getSize().x, firstTexture.getSize().y),CV_8UC4, (void*)firstTexture.copyToImage().getPixelsPtr(), cv::Mat::AUTO_STEP);
    cv::Mat secondImg(cv::Size(secondTexture.getSize().x, secondTexture.getSize().y),CV_8UC4, (void*)secondTexture.copyToImage().getPixelsPtr(), cv::Mat::AUTO_STEP);

    cv::imshow("sa", firstImg);
    cv::imshow("ssssa", secondImg);
    cv::waitKey();
}



void ImageEval::genEvalImg(const Image &srcImg, const Image &dstImg) {

    std::vector<cv::Point2f> src_img_points;
    std::vector<cv::Point2f> dst_img_points;

    for (const auto& circle : srcImg.drawPoints) {
        sf::Vector2f center = circle.getPosition();
        src_img_points.emplace_back(center.x, center.y);
    }
    for (const auto& circle : dstImg.drawPoints) {
        sf::Vector2f center = circle.getPosition();
        dst_img_points.emplace_back(center.x, center.y);
    }
    sf::Image tempImage = srcImg.texture.copyToImage();
    cv::Size size(tempImage.getSize().x, tempImage.getSize().y);
    cv::Mat image(size,CV_8UC4, (void*)tempImage.getPixelsPtr(), cv::Mat::AUTO_STEP);

    cv::Mat warped_img;

    switch (currentModelType) {
        case affine:
            transformMatrix = cv::estimateAffine2D(src_img_points, dst_img_points, cv::noArray(), cv::RANSAC);
            cv::warpAffine(image, warped_img, transformMatrix, image.size());
            break;
        case perspective:
            transformMatrix = cv::findHomography(src_img_points, dst_img_points, cv::RANSAC);
            cv::warpPerspective(image, warped_img, transformMatrix, image.size());
            break;
    }

    texture.create(warped_img.cols, warped_img.rows);
    texture.update(warped_img.data);
    sprite.setTexture(texture);
    is_opened = true;
}

sf::Vector2f ImageEval::transformPoint(const sf::Vector2f point) {
    cv::Mat pointMat = (cv::Mat_<float>(3, 1) << point.x, point.y, 1);

    cv::Mat transformedPointMat = transformMatrix * pointMat;
    return {transformedPointMat.at<float>(0, 0), transformedPointMat.at<float>(1, 0)};
}


