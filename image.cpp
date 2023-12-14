#include "image.h"
#include <nfd.h>


Image::Image() = default;
void Image::openImage() {
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
//        std::cout << "nfd error" << std::endl;
    }
}

