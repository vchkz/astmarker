#pragma once
#ifndef ASTERISMS_HPP
#define ASTERISMS_HPP


#include <opencv2/imgproc/imgproc.hpp>
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
    void savePts(const std::string& path) const;

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
    //! \throw std::out_of_range - в случае, если переданный индекс больше или равен количеству точек, либо отрицательный
    void movePt(const int& idx, const cv::Point2f& offset);

    //! \brief Удаление точки
    //!
    //! \param[in] idx - индекс удаляемой точки
    //! \throw std::out_of_range - в случае, если переданный индекс больше или равен количеству точек, либо отрицательный
    void deletePt(const int& idx);

    //! \brief Установка новых коориднат точки
    //!
    //! \param[in] idx - индекс обновляемой точки
    //! \param[in] position - новые координаты точки
    //! \throw std::runtime_error - в случае, если позиция передаваемой точки находится вне области, на которой задано созвездие
    //! \throw std::out_of_range - в случае, если переданный индекс больше или равен количеству точек, либо отрицательный
    void setPosition(const int& idx, const cv::Point2f& position);

    //! \brief Получение коориднат точки
    //!
    //! \param[in] idx - индек точки, координаты которой требуются
    //! \return pt - точка с индексом idx
    //! \throw std::out_of_range - в случае, если переданный индекс больше или равен количеству точек, либо отрицательный
    cv::Point2f getPosition(const int& idx) const;

    //! \brief Вычисление предполагаемой позиции точки на основании позиции соответствующей точки в парном созвездии
    //!
    //! \param[in] srcPt - соответствующая точка в парном созвездии
    //! \param[in] srcAst - парное созвездие
    //! \throw std::runtime_error - в случае, если в созвездиях разное количество точек
    //! \throw std::runtime_error - в случае, если позиция передаваемой точки находится вне области, на которой задано передаваемое созвездие
    cv::Point2f predictPosition(const cv::Point2f& srcPt, Asterism& srcAst);

    //! \brief Поиск точки, ближайшей к данной
    //!
    //! \param[in] pt - входная точка
    //! \return idx - индекс ближайшей точки или -1, когда в созвездии нет точек
    //! \throw std::runtime_error - в случае, если передаваемая точка лежит вне области, на которой задано созвездие
    int findNearestPt(const cv::Point2f& pt);

    //! \brief Получение размера области созвездия
    //!
    //! \return (ширина, высота) прямоугольной области
    cv::Size getRectSize() const noexcept { return _rect.size(); }

private:
    cv::Rect2f _rect;
    cv::Subdiv2D _subdiv;
    std::vector<cv::Point2f> _points;
    std::vector<int> _free_indices;
    std::vector<int> _bias;

    void _updateSubdiv();
};

#endif