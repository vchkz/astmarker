#include "image.h"
#include <nfd.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <ctime>
//#include <opencv2/cudaarithm.hpp>
//#include <opencv2/cudaimgproc.hpp>


Image::Image() = default;
void Image::openImage() {
//    texture.loadFromFile("D:/SLP/test.png");
//    is_opened = true;
//    sprite.setTexture(texture);

    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);
    if (result == NFD_OKAY) {
        if (!texture.loadFromFile(outPath)) {
            exit(EXIT_FAILURE);
        }
        is_opened = true;
        sprite.setTexture(texture);
    }
    else {
        std::cout << "nfd error" << std::endl;
    }
}
//clock_t start = clock();
//clock_t end = clock();
//double duration = double(end - start) / CLOCKS_PER_SEC;
//std::cout << "time: " << duration  << std::endl;
void Image::genWarpImg(const Image &firstSourceImage, const Image &secondSourceImage){
    sf::Image tempImage = firstSourceImage.texture.copyToImage();
    cv::Size size(tempImage.getSize().x, tempImage.getSize().y);
    cv::Mat image(size,CV_8UC4, (void*)tempImage.getPixelsPtr(), cv::Mat::AUTO_STEP);
    cv::cvtColor(image, image, cv::COLOR_RGBA2RGB);
    float width = firstSourceImage.texture.getSize().x;
    float height = firstSourceImage.texture.getSize().y;
    std::vector<cv::Point2f> a_n = {{0,0}, {0,height}, {width, 0}, {width, height}};
    std::vector<cv::Point2f> a_k = {{0,0}, {0,height}, {width, 0}, {width, height}};

    for (const auto& circle : firstSourceImage.drawPoints) {
        sf::Vector2f center = circle.getPosition();
        a_n.emplace_back(center.x, center.y);
    }
    for (const auto& circle : secondSourceImage.drawPoints) {
        sf::Vector2f center = circle.getPosition();
        a_k.emplace_back(center.x, center.y);
    }

    cv::Subdiv2D subdiv(cv::Rect(0, 0, width * 10, height * 10));

    for (auto &p : a_n) {
        subdiv.insert(p);
    }

    std::vector<cv::Vec6f> triangulation_n;
    subdiv.getTriangleList(triangulation_n);

    cv::Mat result_image = cv::Mat::zeros(image.size(), image.type());


    for (auto &t: triangulation_n) {
//        clock_t start = clock();
        std::vector<cv::Point2f> src_points = {{t[0], t[1]}, {t[2], t[3]}, {t[4], t[5]}};
        std::vector<cv::Point2f> dst_points;
        for (auto &pt: src_points) {
            for (size_t i = 0; i < a_n.size(); ++i) {
                if (a_n[i] == pt) {
                    dst_points.push_back(a_k[i]);
                    break;
                }
            }
        }
        cv::Mat transform_matrix = cv::getAffineTransform(src_points, dst_points);

        cv::Mat warped_triangle;
        cv::warpAffine(image, warped_triangle, transform_matrix, image.size(), cv::INTER_NEAREST,
                       cv::BORDER_REFLECT_101);
        std::vector<cv::Point> int_dst_points;

        for (const auto &point : dst_points) {
            int_dst_points.emplace_back(static_cast<int>(point.x), static_cast<int>(point.y));
        }

        cv::Mat mask = cv::Mat::zeros(image.size(), image.type());
        cv::fillConvexPoly(mask, int_dst_points, cv::Scalar(1, 1, 1), 16, 0);

        warped_triangle.copyTo(result_image, mask);




//        clock_t end = clock();
//        double duration = double(end - start) / CLOCKS_PER_SEC;
//        std::cout << "time: " << duration  << std::endl;


    }




    if (result_image.channels() == 3) {
        cv::cvtColor(result_image, result_image, cv::COLOR_BGR2BGRA);
    }

    texture.create(result_image.cols, result_image.rows);
    texture.update(result_image.data);
    sprite.setTexture(texture);
    is_opened = true;



}

void Image::predictPoint(sf::Vector2<float> pointCoord, const Image &srcImage) {


    int width = texture.getSize().x;
    int height = texture.getSize().y;


    std::vector<cv::Point2f> left_img_points;
    std::vector<cv::Point2f> right_img_points;

//    cv::Rect left_img_rect(0, 0, left_img.cols, left_img.rows);
//    cv::Rect right_img_rect(0, 0, right_img.cols, right_img.rows);
    cv::Rect left_img_rect(0, 0, width, height);
    cv::Rect right_img_rect(0, 0, width, height);

    cv::Subdiv2D left_img_subdiv(left_img_rect);
    cv::Subdiv2D right_img_subdiv(right_img_rect);

    left_img_points = { cv::Point2f(0, 0),
                        cv::Point2f(0, height - 1),
                        cv::Point2f(width - 1, 0),
                        cv::Point2f(width - 1, height - 1) };
    right_img_points = { cv::Point2f(0, 0),
                         cv::Point2f(0, height - 1),
                         cv::Point2f(width - 1, 0),
                         cv::Point2f(width - 1, height - 1) };


    for (const auto& circle : srcImage.drawPoints) {
        sf::Vector2f center = circle.getPosition();
        left_img_points.emplace_back(center.x, center.y);
    }
    for (const auto& circle : drawPoints) {
        sf::Vector2f center = circle.getPosition();
        right_img_points.emplace_back(center.x, center.y);
    }





    left_img_subdiv.insert(left_img_points);
    right_img_subdiv.insert(right_img_points);

    cv::Point2f point(pointCoord.x, pointCoord.y);


    int e, v;
    int l = left_img_subdiv.locate(point, e, v);
    int ne = left_img_subdiv.getEdge(e, cv::Subdiv2D::NEXT_AROUND_LEFT);

    std::vector<cv::Point2f> left_triangle = { left_img_subdiv.getVertex(left_img_subdiv.edgeOrg(e)),
                                      left_img_subdiv.getVertex(left_img_subdiv.edgeDst(e)),
                                      left_img_subdiv.getVertex(left_img_subdiv.edgeDst(ne)) };


    std::vector<cv::Point2f> right_triangle = { right_img_subdiv.getVertex(left_img_subdiv.edgeOrg(e)),
                                       right_img_subdiv.getVertex(left_img_subdiv.edgeDst(e)),
                                       right_img_subdiv.getVertex(left_img_subdiv.edgeDst(ne)) };



    cv::Mat_<float> M = getAffineTransform(left_triangle, right_triangle);


    cv::Mat pointMat = (cv::Mat_<float>(3, 1) << point.x, point.y, 1);

    cv::Mat transformedPointMat = M * pointMat;

    cv::Point2f transformedPoint(transformedPointMat.at<float>(0, 0), transformedPointMat.at<float>(1, 0));


    sf::CircleShape newCircle(10);
    newCircle.setFillColor(sf::Color::Red);
    newCircle.setPosition(transformedPoint.x, transformedPoint.y);
    drawPoints.push_back(newCircle);

}

