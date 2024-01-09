#include "imageEval.h"
#include "iostream"




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



