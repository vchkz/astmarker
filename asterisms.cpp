#include "asterisms.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

Asterism::Asterism(cv::Rect rect)
        : _rect(rect)
{
    _updateSubdiv();
}

void Asterism::loadPts(const std::string& path) {
    std::ifstream in(path);
    if (in.is_open()) {
        float x = 0;
        float y = 0;
        while (in >> x >> y) {
            insertPt(cv::Point2f(x, y));
        }
    }
    else {
        throw std::runtime_error("Can't open the file. May be it does not exist.");
    }
    in.close();
}

void Asterism::savePts(const std::string& path) const {
    std::ofstream out(path);
    if (out.is_open()) {
        for (auto& pt : _points) {
            if (_rect.contains(pt)) {
                out << pt.x << " " << pt.y << std::endl;
            }
        }
    }
    out.close();
}

int Asterism::insertPt(const cv::Point2f& pt) {
    if (!_rect.contains(pt)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    if (_free_indices.empty()) {
        _points.push_back(pt);
    }
    else {
        _points[_free_indices.back()] = pt;
        _free_indices.pop_back();
    }
    _updateSubdiv();
    int idx = _subdiv.findNearest(pt) - 4;
    return idx;
}

void Asterism::movePt(const int& idx, const cv::Point2f& offset) {
    if (idx < 0 && idx >= countPts()) {
        throw std::out_of_range("Index is out of range.");
    }
    cv::Point2f pt = getPosition(idx);
    if (!_rect.contains(pt + offset)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    setPosition(idx, pt + offset);
    _updateSubdiv();
}

void Asterism::deletePt(const int& idx) {
    if (idx < 0 && idx >= countPts()) {
        throw std::out_of_range("Index is out of range.");
    }
    int rawIdx = idx;
    if (!_free_indices.empty()) {
        rawIdx += _bias[idx];
    }
    _points[rawIdx] = _rect.br();
    _free_indices.push_back(rawIdx);
    _updateSubdiv();
}

int Asterism::findNearestPt(const cv::Point2f& pt) {
    if (!_rect.contains(pt)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    if (countPts() == 0) {
        return -1;
    }
    return _subdiv.findNearest(pt) - 4;
}

void Asterism::setPosition(const int& idx, const cv::Point2f& position) {
    if (idx < 0 && idx >= countPts()) {
        throw std::out_of_range("Index is out of range.");
    }
    if (!_rect.contains(position)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    int rawIdx = idx;
    if (!_free_indices.empty()) {
        rawIdx += _bias[idx];
    }
    _points[rawIdx] = position;
    _updateSubdiv();
}

cv::Point2f Asterism::getPosition(const int& idx) const {
    if (idx < 0 && idx >= countPts()) {
        throw std::out_of_range("Index is out of range.");
    }
    int rawIdx = idx;
    if (!_free_indices.empty()) {
        rawIdx += _bias[idx];
    }
    return _points[rawIdx];
}

cv::Point2f Asterism::predictPosition(const cv::Point2f& srcPt, Asterism& srcAst) {
    if (countPts() != srcAst.countPts()) {
        throw std::runtime_error("Asterisms must have the same number of points.");
    }
    cv::Rect srcRect(0, 0, srcAst.getRectSize().width, srcAst.getRectSize().height);
    if (!srcRect.contains(srcPt)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    cv::Subdiv2D srcSubdiv(srcRect);
    srcSubdiv.insert({ cv::Point2f(0, 0),
                       cv::Point2f(srcRect.width - 1, 0),
                       cv::Point2f(0, srcRect.height - 1),
                       cv::Point2f(srcRect.width - 1, srcRect.height - 1) });
    for (int i = 0; i < srcAst.countPts(); i += 1) {
        srcSubdiv.insert(srcAst.getPosition(i));
    }
    cv::Subdiv2D dstSubdiv(_rect);
    dstSubdiv.insert({ cv::Point2f(0, 0),
                       cv::Point2f(_rect.width - 1, 0),
                       cv::Point2f(0, _rect.height - 1),
                       cv::Point2f(_rect.width - 1, _rect.height - 1) });
    for (int i = 0; i < countPts(); i += 1) {
        dstSubdiv.insert(getPosition(i));
    }

    int e, v;
    int l = srcSubdiv.locate(srcPt, e, v);
    int ne = srcSubdiv.getEdge(e, cv::Subdiv2D::NEXT_AROUND_LEFT);

    std::vector<int> triangleIndices = { srcSubdiv.edgeOrg(e),
                                         srcSubdiv.edgeDst(e),
                                         srcSubdiv.edgeDst(ne)};

    std::vector<cv::Point2f> srcTriangle = { srcSubdiv.getVertex(triangleIndices[0]),
                                             srcSubdiv.getVertex(triangleIndices[1]),
                                             srcSubdiv.getVertex(triangleIndices[2]) };

    std::vector<cv::Point2f> dstTriangle = { dstSubdiv.getVertex(triangleIndices[0]),
                                             dstSubdiv.getVertex(triangleIndices[1]),
                                             dstSubdiv.getVertex(triangleIndices[2]) };

    cv::Mat_<float> M = cv::getAffineTransform(srcTriangle, dstTriangle);

    cv::Mat srcPtMat = (cv::Mat_<float>(3, 1) << srcPt.x, srcPt.y, 1);

    cv::Mat dstPtMat = M * srcPtMat;

    cv::Point2f dstPt = cv::Point2f(dstPtMat.at<float>(0, 0), dstPtMat.at<float>(1, 0));

    return dstPt;
}

void Asterism::_updateSubdiv() {
    _subdiv = cv::Subdiv2D(cv::Rect(0, 0, _rect.width, _rect.height));
    _bias.resize(countPts());
    int bias = 0;
    for (int i = 0; i < _points.size(); i += 1) {
        if (_rect.contains(_points[i])) {
            _subdiv.insert(_points[i]);
            _bias[i - bias] = bias;
        }
        else {
            bias += 1;
        }
    }
}