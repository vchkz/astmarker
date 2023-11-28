#ifndef IMAGE_H
#define IMAGE_H



#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cmath>
#include <algorithm>

/**
 * @brief Класс Image представляет изображение, состоящее из слоев.
 */
class Image {
public:
    /**
     * @brief Конструктор класса Image
     */
    Image();

    /**
     * @brief Деструктор класса Image
     */
//    virtual ~Image();

    std::string name; /**< Имя изображения */
    std::string fullPath; /**< Полный путь к изображению */
    bool isOpened;
    int size[2] = {256, 256}; /**< Размер изображения */
    GLuint _glindex; /**< Идентификатор текстуры OpenGL */
//    unsigned char *_data = nullptr; /**< Данные изображения */
    cv::Mat _data;
    void fromFile(const char *filename);
    void setScale(float scale);



};

#endif // IMAGE_H
