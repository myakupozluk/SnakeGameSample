#include "shaderProgram.hpp"
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() {
     programId = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
     glDeleteProgram(programId);
}
;

void ShaderProgram::link() {
     glLinkProgram(programId);
}

void ShaderProgram::use() {
     glUseProgram(programId);
}

void ShaderProgram::addUniform(const std::string& uniformVariableName) {
     uniformVariables[uniformVariableName] = glGetUniformLocation(programId, uniformVariableName.c_str());
}

void ShaderProgram::setFloat(const std::string& variableName, float value) {
     glUniform1f(uniformVariables[variableName], value);
}

void ShaderProgram::setVec3(const std::string &variableName, glm::vec3 value) {
     glUniform3f(uniformVariables[variableName], value.x, value.y, value.z);
}

void ShaderProgram::setVec4(const std::string &variableName, glm::vec4 value) {
     glUniform4f(uniformVariables[variableName], value.r, value.g, value.b, value.a);
}

void ShaderProgram::attachShader(const char *fileName, unsigned int shaderType) {
     unsigned int shaderId = glCreateShader(shaderType);
     std::string sourceCode = getShaderFromFile(fileName);
     const char* chSourceCode = &sourceCode[0];
     glShaderSource(shaderId,1,&chSourceCode,0);
     glCompileShader(shaderId);
     int success;
     glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
     if (!success) {
          char infoLog[512];
          glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
          std::cerr << "Shader compilation failed (" << fileName << "):\n" << infoLog << std::endl;
     }
     glAttachShader(programId,shaderId);
     glDeleteShader(shaderId);
}

std::string ShaderProgram::getShaderFromFile(const char* fileName) {
     std::ifstream file(fileName);
     std::stringstream buffer;

     if (!file.is_open()) {
          std::cerr << "Failed to open shader file: " << fileName << std::endl;
          return "";
     }

     buffer << file.rdbuf();
     return buffer.str();
}



