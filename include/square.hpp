#ifndef  SQUARE_HPP
#define SQUARE_HPP
#include <glm/glm.hpp>

class Square {
    public:
        enum DIRECTION {
            DIR_RIGHT = 1,
            DIR_LEFT = -1,
            DIR_UP = 2,
            DIR_DOWN = -2,
        };

        Square(float x, float y, float len);
        glm::vec3 getPosition();
        glm::vec4 getColor();
        void move();
        DIRECTION getDirection();
        void setPosition(const glm::vec3& pos);
        void setDirection(DIRECTION dir);

    private:
        glm::vec4 color;
        glm::vec3 position;
        DIRECTION direction;
        float length;
};

#endif