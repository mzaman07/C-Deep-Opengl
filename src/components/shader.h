#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>


char* openStringFile(char* filePath);
int openAndCompileShader(char* vertexPath, char* fragmentPath);
unsigned int compileShaders(char* vertexPath, char* fragmentPath);
void useShader(int shaderProgram);
void detachShader();
void setBool(int shaderProgram, char* name, int value);
void setInt(int shaderProgram, char* name, int value);
void setFloat(int shaderProgram, char* name, float value);
void setVec3(int shaderProgram, char* name, float x, float y, float z);
void setMat4(int shaderProgram, char* name, mat4 value);