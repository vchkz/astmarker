#include "image.h"




Image::Image() = default;


void Image::fromFile(const char *filePath) {
    cv::Mat Image = cv::imread(filePath);
    if (Image.empty()) {std::cout << "open image error" << std::endl;}
    else {
        std::cout << "Image loaded successfully from path: " << filePath << std::endl;
        isOpened = true;
        cv::cvtColor(Image, Image, cv::COLOR_BGR2RGBA);
        _data = Image;

        glGenTextures(1, &_glindex);

        glBindTexture(GL_TEXTURE_2D, _glindex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _data.cols, _data.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}
void Image::setScale(float scale) {
    if (!isOpened) return;
    // TODO: Применение масштаба к изображению
    // Например, можно умножить ширину и высоту изображения на масштаб
    _data.cols *= scale;
    _data.rows *= scale;

    // TODO: Обновление текстуры OpenGL с новыми размерами
    glBindTexture(GL_TEXTURE_2D, _glindex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _data.cols, _data.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data.data);
}
