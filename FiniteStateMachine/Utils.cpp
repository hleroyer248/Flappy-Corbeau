#include "Utils.h"
#include <cmath>

sf::Color getRainbowColor(float time) {
    float speed = 6.f;

    int r = static_cast<int>(std::sin(time * speed) * 127 + 128);
    int g = static_cast<int>(std::sin(time * speed + 2) * 127 + 128);
    int b = static_cast<int>(std::sin(time * speed + 4) * 127 + 128);

    return sf::Color(r, g, b);
}