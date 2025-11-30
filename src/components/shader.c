#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdlib.h>

// generic open file to heap memory pointer
char* openStringFile(char* pathFile) {
    FILE* file;

    file = fopen(pathFile, "rb");

    if (file == NULL) {
        printf("File could not be opened");
        fclose(file);
        return NULL;
    }

    // get the file size we need
    //while (fgetc(fVertexPtr) != EOF) {
    //    vertexSize++;
    //}
    // get length of contents
    fseek(file, 0L, SEEK_END);
    int length = ftell(file);
    // point it back to the start of the content
    fseek(file, 0L, SEEK_SET);

    char* fileContents = calloc(sizeof(char) * (length + 1), sizeof(char));
        //memset(malloc(length), '\0', length);
    int i = 0;
    int c = "";
    while ((c = fgetc(file)) != EOF) {
        printf("%c %d ", c, i);
        fileContents[i] = (char)c;
        i++;
    }
    
    //fread(fileContents, 1, length - 1, file);
 
    fclose(file);
    printf("%s\n", fileContents);
    printf("%lu\n", strlen(fileContents));
    return fileContents;
}
/*
Open a GLSL shader file and compile it
returns 0 - if successful
1 - if there is any failure
*/
int openAndCompileShader(char* vertexPath, char* fragmentPath) {
    FILE *fVertexPtr;
    FILE *fFragmentPtr;

    fVertexPtr = fopen(vertexPath, "r");

    if(fVertexPtr == NULL) {
        printf("File could not be opened");
        return 1;
    }
    int vertexSize = 0;

    // get the file size we need
    //while (fgetc(fVertexPtr) != EOF) {
    //    vertexSize++;
    //}
    // get length of contents
    fseek(fVertexPtr, 0, SEEK_END);
    int vertexLength = ftell(fVertexPtr);
    // point it back to the start of the content
    fseek(fVertexPtr, 0, SEEK_SET);

    char* vertexCode = malloc(sizeof(char) * (vertexLength + 1));
    // read every byte
    //fgets(vertexCode, sizeof(vertexCode), fVertexPtr);
    //
    //printf("string:\n %s",vertexCode);

    // get the vertex shader 
    int i = 0;
    char c = "";
    while ((c = fgetc(fVertexPtr)) != EOF) {
        printf("%c ", c);
        vertexCode[i] = c;
        i++;
    }

    //for (int j = 0; j < vertexLength; j++) {
    //    printf("%c", vertexCode[j]);
    //}
    fclose(fVertexPtr);
    free(vertexCode);
    return 0;
}

unsigned int compileShaders(char *vertexPath, char *fragmentPath) {
    // vertex shader compilation
    //char *vertexPath = "";
    char* vertexShaderSource = openStringFile(vertexPath);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for errors 
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s \0", infoLog);

        return 0;
    }

    // fragment shader compilation
    char* fragmentShaderSource = openStringFile(fragmentPath);

    // fragment shader compilation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    printf("Test idea for frag shade");
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \0", infoLog);

        return -1;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for errors
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINK::SHADER_LINK_FAILED\n %s \0", infoLog);

        return -1;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexShaderSource);
    free(fragmentShaderSource);
    vertexShaderSource = NULL;
    fragmentShaderSource = NULL;

    return shaderProgram;
}


void setBool(int shaderProgram, char* name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void setInt(int shaderProgram, char* name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void setFloat(int shaderProgram, char* name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void useShader(int shaderProgram) {
    glUseProgram(shaderProgram);
}