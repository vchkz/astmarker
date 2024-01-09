#pragma once
#ifndef ASTERISMS_HPP
#define ASTERISMS_HPP


#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <string>
#include <stdexcept>


class Asterism {
public:
    Asterism() = default;
    Asterism(int width, int height);
    Asterism(cv::Rect rect);

    Asterism& operator=(const Asterism& other);

    cv::Subdiv2D subdiv;

    void loadPts(const std::string& path);
    void savePts(const std::string& path) const;

    int insertPt(const cv::Point& pt);
    void setPosition(const int& idx, const cv::Point& newCoords);
    void movePt(const int& idx, const cv::Point& newCoords);
    void deletePt(const int& idx);

    int findNearestPt(const cv::Point& pt);
    cv::Point getCoords(const int& idx);


    cv::Point predictPairedPt(const cv::Point& pt, Asterism& pairedAst);

    void writePts(std::vector<cv::Point>& pts) const;

    std::vector<cv::Point> getPts() const;

private:
    cv::Rect _rect;
    std::vector<cv::Point> _corners;
    std::vector<cv::Point> _points;
    std::vector<int> _free_indices;

    void _updateSubdiv();
};

#endif