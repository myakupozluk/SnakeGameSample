#include <chrono>
#include <iostream>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderProgram.hpp"
#include <glm/glm.hpp>
#include "square.hpp"
#include <vector>

bool canMove = false;
float length = 0.08f;

float vertices[] = {
    -length/2, -length/2, 0.0f,
    -length/2,  length/2, 0.0f,
     length/2,  length/2, 0.0f,
    -length/2, -length/2, 0.0f,
     length/2,  length/2, 0.0f,
     length/2, -length/2, 0.0f
};

std::vector<Square*> snakeList;

glm::vec3 generateRandomFoodPosition(float length) {
    int gridCount = static_cast<int>(2.0f / length) - 2;
    int xIndex = (std::rand() % gridCount) - gridCount / 2;
    int yIndex = (std::rand() % gridCount) - gridCount / 2;
    float x = xIndex * length;
    float y = yIndex * length;
    return glm::vec3(x, y, 0.0f);
}

void addSquare() {
    int numberOfSquares = snakeList.size();
    if (numberOfSquares == 0) {
        snakeList.push_back(new Square(0.0f, 0.0f, length));
    } else {
        Square* lastSquare = snakeList[numberOfSquares - 1];
        glm::vec3 pos = lastSquare->getPosition();
        Square::DIRECTION lastSquareDir = lastSquare->getDirection();
        switch (lastSquareDir) {
            case Square::DIR_RIGHT: pos.x -= length; break;
            case Square::DIR_LEFT:  pos.x += length; break;
            case Square::DIR_UP:    pos.y -= length; break;
            case Square::DIR_DOWN:  pos.y += length; break;
        }
        Square* newSquare = new Square(pos.x, pos.y, length);
        newSquare->setDirection(lastSquareDir);
        snakeList.push_back(newSquare);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!snakeList.empty()) {
            Square* firstSquare = snakeList[0];
            if (firstSquare->getDirection() != Square::DIR_RIGHT && key == GLFW_KEY_LEFT)
                firstSquare->setDirection(Square::DIR_LEFT);
            if (firstSquare->getDirection() != Square::DIR_LEFT && key == GLFW_KEY_RIGHT)
                firstSquare->setDirection(Square::DIR_RIGHT);
            if (firstSquare->getDirection() != Square::DIR_DOWN && key == GLFW_KEY_UP)
                firstSquare->setDirection(Square::DIR_UP);
            if (firstSquare->getDirection() != Square::DIR_UP && key == GLFW_KEY_DOWN)
                firstSquare->setDirection(Square::DIR_DOWN);
        }
        if (key == GLFW_KEY_SPACE) canMove = true;
        if (key == GLFW_KEY_ESCAPE) glfwTerminate();
        if (key == GLFW_KEY_ENTER) addSquare();
    }
}

void moveSnake() {
    for (auto next : snakeList) next->move();
    for (int i = (int)snakeList.size() - 1; i > 0; i--)
        snakeList[i]->setDirection(snakeList[i - 1]->getDirection());
}

void drawSnake(ShaderProgram& program) {
    for (auto next : snakeList) {
        program.setVec3("uMove", next->getPosition());
        program.setVec4("uColor", next->getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 800, "Snake Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    addSquare();

    glm::vec3 foodPos = generateRandomFoodPosition(length);
    Square* food = new Square(foodPos.x, foodPos.y, length);

    ShaderProgram program;
    program.attachShader("../shaders/vertexshader.glsl", GL_VERTEX_SHADER);
    program.attachShader("../shaders/fragmentshader.glsl", GL_FRAGMENT_SHADER);
    program.link();
    program.addUniform("uMove");
    program.addUniform("uColor");

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        program.use();

        drawSnake(program);
        if (canMove) moveSnake();

        if (glm::distance(snakeList[0]->getPosition(), food->getPosition()) < 0.001f) {
            addSquare();
            glm::vec3 newFoodPos = generateRandomFoodPosition(length);
            food->setPosition(newFoodPos);
        }

        program.setVec3("uMove", food->getPosition());
        program.setVec4("uColor", glm::vec4(1, 0, 0, 1));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(VAO);
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    glfwTerminate();
    return 0;
}
