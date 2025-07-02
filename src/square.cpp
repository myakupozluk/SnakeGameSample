#include "square.hpp"
#include <cstdlib>

Square::Square(float x, float y, float len) {
    position = glm::vec3(x, y, 0.0f);

    color.r = (10 + std::rand() % 25) / 25.0f;
    color.g = (10 + std::rand() % 25) / 25.0f;
    length = len;

    direction = DIR_RIGHT;
}

glm::vec3 Square::getPosition() {
    return position;
}

glm::vec4 Square::getColor() {
    return color;
}

void Square::move() {
    switch (direction) {
        case DIR_RIGHT:
            position.x += length;
            break;
        case DIR_LEFT:
            position.x -= length;
            break;
        case DIR_UP:
            position.y += length;
            break;
        case DIR_DOWN:
            position.y -= length;
            break;
    }
}

void Square::setDirection(Square::DIRECTION dir) {
    direction = dir;
}

Square::DIRECTION Square::getDirection() {
    return direction;
}

void Square::setPosition(const glm::vec3& pos) {
    position = pos;
}