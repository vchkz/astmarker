#include "asterisms.h"

#include "iostream"
Asterism::Asterism(cv::Rect rect)
        : _rect(rect)
        , _corners({ cv::Point2f(0, 0),
                     cv::Point2f(rect.width - 1, 0),
                     cv::Point2f(0, rect.height - 1),
                     cv::Point2f(rect.width - 1, rect.height - 1) })
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

void Asterism::savePts(const std::string& path) const noexcept {
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
    int idx = _subdiv.findNearest(pt) - 8;
    return idx;
}

void Asterism::movePt(const int& idx, const cv::Point2f& offset) {
    cv::Point2f pt = getPosition(idx);
    if (!_rect.contains(pt + offset)) {
        std::cout<<"32432423";
        throw std::runtime_error("Point is out of bounds.");
    }
    setPosition(idx, pt + offset);
    _updateSubdiv();
}

void Asterism::deletePt(const int& idx) {
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
    return _subdiv.findNearest(pt) - 8;
}

void Asterism::setPosition(const int& idx, const cv::Point2f& position) {
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

cv::Point2f Asterism::getPosition(const int& idx) const noexcept {
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
    std::vector<int> triangleIndices = srcAst.getTriangleIndices(srcPt);

    std::vector<cv::Point2f> srcTriangle = { srcAst.getSubdivPosition(triangleIndices[0]),
                                             srcAst.getSubdivPosition(triangleIndices[1]),
                                             srcAst.getSubdivPosition(triangleIndices[2]) };

    std::vector<cv::Point2f> dstTriangle = { getSubdivPosition(triangleIndices[0]),
                                             getSubdivPosition(triangleIndices[1]),
                                             getSubdivPosition(triangleIndices[2]) };

    cv::Mat_<float> M = cv::getAffineTransform(srcTriangle, dstTriangle);

    cv::Mat srcPtMat = (cv::Mat_<float>(3, 1) << srcPt.x, srcPt.y, 1);

    cv::Mat dstPtMat = M * srcPtMat;

    cv::Point2f dstPt = cv::Point2f(dstPtMat.at<float>(0, 0), dstPtMat.at<float>(1, 0));

    return dstPt;
}

std::vector<int> Asterism::getTriangleIndices(const cv::Point2f& pt) {
    if (!_rect.contains(pt)) {
        throw std::runtime_error("Point is out of bounds.");
    }
    int e, v;
    int l = _subdiv.locate(pt, e, v);
    int ne = _subdiv.getEdge(e, cv::Subdiv2D::NEXT_AROUND_LEFT);

    std::vector<int> triangleIndices = { _subdiv.edgeOrg(e),
                                         _subdiv.edgeDst(e),
                                         _subdiv.edgeDst(ne)};
    return triangleIndices;
}

void Asterism::_updateSubdiv() {
    _subdiv = cv::Subdiv2D(cv::Rect(0, 0, _rect.width * 10, _rect.height * 10));
    for (auto& pt : _corners)
        _subdiv.insert(pt);
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

cv::Point2f Asterism::getSubdivPosition(const int &idx) const noexcept {
    return cv::Point2f(_subdiv.getVertex(idx));
}
