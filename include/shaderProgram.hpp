#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#include <string>
#include <map>
#include <glm/glm.hpp>

class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();
        void attachShader(const char* fileName, unsigned int shaderType);
        void link();
        void use();
        void addUniform(const std::string& uniformVariableName);
        void setFloat(const std::string& variableName, float value);
        void setVec3(const std::string& variableName, glm::vec3 value);
        void setVec4(const std::string& variableName, glm::vec4 value);
    private:
        std::map<std::string, unsigned int> uniformVariables;
        unsigned int programId;
        std::string getShaderFromFile(const char* fileName);
};

#endif
