#pragma once
#ifndef ASTERISMS_HPP
#define ASTERISMS_HPP


#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>


class Asterism {
public:
    Asterism() = default;
    Asterism(const Asterism&) = default;
    Asterism(const cv::Rect rect);
    ~Asterism() = default;
    Asterism& operator=(const Asterism&) = default;

    //! \brief Загрузка точек из текстового файла
    //!
    //! \param[in] path - путь к файлу с расширение .txt
    //! \throw std::runtime_error - в случае ошибки в открытии файла
    void loadPts(const std::string& path);

    //! \brief Сохранение точек в текстовый файл
    //!
    //! \param[in] path - путь к файлу с расширение .txt
    void savePts(const std::string& path) const noexcept;

    //! \brief Количество отмеченных точек
    int countPts() const noexcept { return _points.size() - _free_indices.size(); }

    //! \brief Добавление новой точки
    //!
    //! \param[in] pt - добавляемая точка
    //! \return idx - индекс точки
    //! \throw std::runtime_error - в случае, если передаваемая точка лежит вне области, на которой задано созвездие
    int insertPt(const cv::Point2f& pt);

    //! \brief Перемещение точки
    //!
    //! \param[in] idx - индекс передвигаемой точки
    //! \param[in] offset - сдвиг: offset.x - горизонатльный и offset.y - вертикальный
    //! \throw std::runtime_error - в случае, если в результате перемещения получается точка, лежащая вне области, на которой задано созвездие
    void movePt(const int& idx, const cv::Point2f& offset);

    //! \brief Удаление точки
    //!
    //! \param[in] idx - индекс удаляемой точки
    void deletePt(const int& idx);

    //! \brief Установка новых коориднат точки
    //!
    //! \param[in] idx - индекс обновляемой точки
    //! \param[in] position - новые координаты точки
    //! \throw std::runtime_error - в случае, если позиция передаваемой точки находится вне области, на которой задано созвездие
    void setPosition(const int& idx, const cv::Point2f& position);

    //! \brief Получение коориднат точки
    //!
    //! \param[in] idx - индек точки, координаты которой требуются
    //! \return pt - точка с индексом idx
    cv::Point2f getSubdivPosition(const int& idx) const noexcept;
    cv::Point2f getPosition(const int& idx) const noexcept;

    //! \brief Вычисление предполагаемой позиции точки на основании позиции соответствующей точки в парном созвездии
    //!
    //! \param[in] srcPt - соответствующая точка в парном созвездии
    //! \param[in] srcAst - парное созвездие
    //! \throw std::runtime_error - в случае, если в созвездиях разное количество точек
    cv::Point2f predictPosition(const cv::Point2f& srcPt, Asterism& srcAst);

    //! \brief Поиск точки, ближайшей к данной
    //!
    //! \param[in] pt - входная точка
    //! \return idx - индекс ближайшей точки
    //! \throw std::runtime_error - в случае, если передаваемая точка лежит вне области, на которой задано созвездие
    int findNearestPt(const cv::Point2f& pt);

    //! \brief Поиск треугольника, внутри которого лежит данная точка
    //!
    //! \param[in] pt - входная точка (не из созвездия)
    //! \return Вектор из 3 индексов, которые соответствуют вершинам треугольника, внутри которого лежит данная точка
    //! \throw std::runtime_error - в случае, если передаваемая точка лежит вне области, на которой задано созвездие
    std::vector<int> getTriangleIndices(const cv::Point2f& pt);

private:
    cv::Rect _rect;
    std::vector<cv::Point2f> _corners;
    cv::Subdiv2D _subdiv;
    std::vector<cv::Point2f> _points;
    std::vector<int> _free_indices;
    std::vector<int> _bias;

    void _updateSubdiv();
};

#endif