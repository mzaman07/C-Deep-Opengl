#include <stdio.h>
#include <glad/glad.h>

char* openStringFile(char* filePath);
int openAndCompileShader(char* vertexPath, char* fragmentPath);
unsigned int compileShaders(char* vertexPath, char* fragmentPath);
void useShader(int shaderProgram);
void detachShader();
void setBool(int shaderProgram, char* name, int value);
void setInt(int shaderProgram, char* name, int value);
void setFloat(int shaderProgram, char* name, float value);