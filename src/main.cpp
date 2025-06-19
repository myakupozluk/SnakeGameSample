#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

#include "shaderprogram.hpp"
#include "square.hpp"

GLFWwindow* window = nullptr;

float length = 0.08f;

float vertices[] = {
    -length / 2,  length / 2, 0.0f,
    -length / 2, -length / 2, 0.0f,
     length / 2, -length / 2, 0.0f,
    -length / 2,  length / 2, 0.0f,
     length / 2, -length / 2, 0.0f,
     length / 2,  length / 2, 0.0f,
};

std::vector<Square*> snakeList;

void moveSnake() {
    for (auto next : snakeList) {
        next->move();
    }
    for (int i = snakeList.size() - 1; i > 0; i--) {
        snakeList[i]->setDirection(snakeList[i - 1]->getDirection());
    }
}

void drawSnake(ShaderProgram& program) {
    for (auto next : snakeList) {
        program.setVec3("uMove", next->getPosition());
        program.setVec4("uColor", next->getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void addToSnakeTail() {
    int elementCount = snakeList.size();

    if (elementCount == 0) {
        snakeList.push_back(new Square(0.0f, 0.0f, length));
        return;
    }

    Square* lastSquare = snakeList[elementCount - 1];
    glm::vec3 pos = lastSquare->getPosition();
    Square::DIRECTION dir = lastSquare->getDirection();

    switch (dir) {
        case Square::DIR_RIGHT: pos -= glm::vec3(length, 0.0f, 0.0f); break;
        case Square::DIR_LEFT:  pos += glm::vec3(length, 0.0f, 0.0f); break;
        case Square::DIR_UP:    pos -= glm::vec3(0.0f, length, 0.0f); break;
        case Square::DIR_DOWN:  pos += glm::vec3(0.0f, length, 0.0f); break;
    }

    Square* newSquare = new Square(pos.x, pos.y, length);
    newSquare->setDirection(dir);
    snakeList.push_back(newSquare);
}

bool isOutOfBounds(const glm::vec3& pos, float bound = 1.0f) {
    return pos.x > bound || pos.y > bound || pos.x < -bound || pos.y < -bound;
}

unsigned int VBO;
unsigned int VAO;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();

    if (action == GLFW_PRESS && !snakeList.empty()) {
        Square* first = snakeList[0];

        if (key == GLFW_KEY_LEFT && first->getDirection() != Square::DIR_RIGHT)
            first->setDirection(Square::DIR_LEFT);

        if (key == GLFW_KEY_RIGHT && first->getDirection() != Square::DIR_LEFT)
            first->setDirection(Square::DIR_RIGHT);

        if (key == GLFW_KEY_UP && first->getDirection() != Square::DIR_DOWN)
            first->setDirection(Square::DIR_UP);

        if (key == GLFW_KEY_DOWN && first->getDirection() != Square::DIR_UP)
            first->setDirection(Square::DIR_DOWN);

        if (key == GLFW_KEY_SPACE)
            addToSnakeTail();
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Yilan Oyunu", NULL, NULL);
    if (window == NULL) {
        std::cout << "Pencere olusturulamadi." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD yuklenemedi." << std::endl;
        return -1;
    }

    for (int i = 0; i < 6; i++) {
        addToSnakeTail();
    }

    ShaderProgram program;
    program.attachShader("./shaders/simplevs.glsl", GL_VERTEX_SHADER);
    program.attachShader("./shaders/simplefs.glsl", GL_FRAGMENT_SHADER);
    program.link();

    program.addUniform("uMove");
    program.addUniform("uColor");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        drawSnake(program);
        moveSnake();

        if (isOutOfBounds(snakeList[0]->getPosition())) {
            std::cout << "Yılan ekran dışına çıktı. Oyun bitti." << std::endl;
            glfwSetWindowShouldClose(window, true);
        }

        glBindVertexArray(VAO);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
