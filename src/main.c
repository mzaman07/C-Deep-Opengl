#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cglm/cglm.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    printf("New Height %d and Width %d\n", height, width);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

float calculateRadians(float degrees) {
    return degrees * (M_PI / 180);
}

float calculateDegrees(float radians) {
    return radians * (180 / M_PI);
}

void drawTriangle() {

    float vertices[] = { 0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        0.0f, 0.5f, 0.0f   // top 
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void drawRectangle() {

    // vertex shader compilation
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
    }

    // fragment shader compilation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \0", infoLog);
    }

    // link both shaders
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
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // link vertex attribs

    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



}

void triangles2() {
    float vertices[] = {
 0.5f,  0.5f, 0.0f,  // top right
 0.5f, -0.5f, 0.0f,  // bottom right
-0.5f, -0.5f, 0.0f,  // bottom left
-0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
    };


}


// define pointers in here then use it for functions in the other 
// to do heap allocation free
int main(int argc, char *argv[]){
    printf("Hello, from 3dCgraphics!\n");
    // how glfw will handle errors via a provided callback
    // glfwSetErrorCallback(error_callback);
    char* vertexPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\vertex.glsl\0";

    char* fragmentPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\fragment.glsl\0";

    //int fileIO = openAndCompileShader(vertexPath, "");

    //char* test = openStringFile(vertexPath);
    //printf("%s",test);
    //free(test);
    //test = NULL;


    // Make sure glfw is initialized
    if (!glfwInit()) {
        // Initialization failed
        printf("Could not initialize GLFW!");
        return -1;
    }

    // Specify the opengl we want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);


    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Test", NULL, NULL);

    if (!window) {
        printf("Failed to create window");
        glfwTerminate();
        return -1;
    }

    // this is needed before you can use OpenGL
    glfwMakeContextCurrent(window);\
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = compileShaders(vertexPath, fragmentPath);
    // vertex shader compilation
   /* unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);*/

    // check for errors 
    /*int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s \0", infoLog);
    }*/

    // fragment shader compilation
    /*unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s \0", infoLog);
    }*/

    // link both shaders
    /*unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);*/

    // check for errors
    /*glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINK::SHADER_LINK_FAILED\n %s \0", infoLog);
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);*/

    // link vertex attribs

    //float vertices[] = {
    // 0.5f,  0.5f, 0.0f,  // top right
    // 0.5f, -0.5f, 0.0f,  // bottom right
    //-0.5f, -0.5f, 0.0f,  // bottom left
    //-0.5f,  0.5f, 0.0f   // top left 
    //};

    // colors in rgb format
    //float vertices[] = {
    //    // positions         // colors
    //    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //    0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    //};

    // for rotating image texture
    //float vertices[] = {
    // // position            // colors      // tex coords
    // 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// top right
    // 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,// bottom right
    //-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// bottom left
    //-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f// top left 
    //};

    // 3D box vertices
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // different cube positions 
    vec3 cubePosition[10];
    glm_vec3_zero(cubePosition[0]);
    cubePosition[0][0] = 0.0f;
    cubePosition[0][1] = 0.0f;
    cubePosition[0][2] = 0.0f;
    glm_vec3_zero(cubePosition[1]);
    cubePosition[1][0] = 2.0f;
    cubePosition[1][1] = 5.0f;
    cubePosition[1][2] = -15.0f;
    glm_vec3_zero(cubePosition[2]);
    cubePosition[2][0] = -1.5f;
    cubePosition[2][1] = -2.2f;
    cubePosition[2][2] = -2.5f;
    glm_vec3_zero(cubePosition[3]);
    cubePosition[3][0] = -3.8f;
    cubePosition[3][1] = -2.0f;
    cubePosition[3][2] = -12.3f;
    glm_vec3_zero(cubePosition[4]);
    cubePosition[4][0] = 2.4f;
    cubePosition[4][1] = -0.4f;
    cubePosition[4][2] = -3.5f;
    glm_vec3_zero(cubePosition[5]);
    cubePosition[5][0] = -1.7f;
    cubePosition[5][1] = 3.0f;
    cubePosition[5][2] = -7.5f;
    glm_vec3_zero(cubePosition[6]);
    cubePosition[6][0] = 1.3f;
    cubePosition[6][1] = -2.0f;
    cubePosition[6][2] = -2.5f;
    glm_vec3_zero(cubePosition[7]);
    cubePosition[7][0] = 1.5f;
    cubePosition[7][1] = 2.0f;
    cubePosition[7][2] = -2.5f;
    glm_vec3_zero(cubePosition[8]);
    cubePosition[8][0] = 1.5f;
    cubePosition[8][1] = 0.2f;
    cubePosition[8][2] = -1.5f;
    glm_vec3_zero(cubePosition[9]);
    cubePosition[9][0] = -1.3f;
    cubePosition[9][1] = 1.0f;
    cubePosition[9][2] = -1.5f;
    printf("position 0,0 %f\n", cubePosition[0][0]);
    printf("position 1,2 %f\n", cubePosition[1][2]);
    printf("position 2,1 %f\n", cubePosition[2][1]);

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   /* unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture coord attribute
    /*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);*/
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load the texture for gpu params
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading from file

    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\container.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        // file open success
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    // load the texture for gpu params
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading from file

    //int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load("C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\awesomeface.png", &width, &height, &nrChannels, 0);

    if (data) {
        // file open success
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);
    useShader(shaderProgram);
    setInt(shaderProgram, "texture1", 0);
    setInt(shaderProgram, "texture2", 1);

    
    //glBindVertexArray(0);
    // use this to see the polygon shape of the rendered triangles
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // projection only needs to be done once
    mat4 projection;
    glm_mat4_identity(projection);
    glm_perspective(calculateRadians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    while (!glfwWindowShouldClose(window)) {
        // Keep running until user closes window
        // input handle method for now
        processInput(window);
        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
        // rendering commands for now
        //glViewport(0, 0, 1920, 1080);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        useShader(shaderProgram);


        float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // transform for animations
        //vec4 vec;
        //glm_vec4_zero(vec);
        //vec[0] = 1.0f;
        //vec[1] = 0.0f;
        //vec[2] = 0.0f;
        //vec[3] = 1.0f;
        //mat4 trans;
        //glm_mat4_identity(trans);
        //vec3 vec3Trans;
        //glm_vec3_zero(vec3Trans);
        //vec3Trans[0] = 0.5f;
        //vec3Trans[1] = -0.5f;
        //// rotation around unit vector
        //vec3 rotVec;
        //glm_vec3_zero(rotVec);
        //rotVec[2] = 1.0f;
        //glm_translate(trans, vec3Trans);
        //glm_rotate(trans, (float)glfwGetTime(), rotVec);
        //// scale vector
        //vec3 sclVec;
        //glm_vec3_zero(sclVec);
        //sclVec[0] = 0.5f;
        //sclVec[1] = 0.5f;
        //sclVec[2] = 0.5f;
        //glm_scale(trans, sclVec);

        // projection stuff
       /* mat4 model;
        glm_mat4_identity(model);

        vec3 rotVec2;
        glm_vec3_zero(rotVec2);
        rotVec2[0] = 0.5f;
        rotVec2[1] = 1.0f;

        glm_rotate(model, timeValue, rotVec2);*/
        // camera position - relative to z-axis
        mat4 view;
        vec3 viewEye;
        vec3 viewCenter;
        vec3 viewUp;

        // rotate the camera and view projection 
        const float radius = 10.0f;
        float camX = sin(timeValue) * radius;
        float camZ = cos(timeValue) * radius;
        glm_vec3_zero(viewEye);
        glm_vec3_zero(viewCenter);
        glm_vec3_zero(viewUp);
        viewEye[0] = camX;
        viewEye[2] = camZ;
        viewUp[1] = 1.0f;

        glm_lookat(viewEye, viewCenter, viewUp, view);

/*        mat4 view;
        glm_mat4_identity(view)*/;
        // note that we're translating the scene in the reverse direction of where we want to move
        vec3 vTrans;
        glm_vec3_zero(vTrans);
        //vTrans[1] = -1.5f;
        vTrans[2] = -3.0f;

        glm_translate(view, vTrans);
        



       /* unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);*/
        //setMat4(shaderProgram, "model", model);
        setMat4(shaderProgram, "view", &view[0][0]);
        setMat4(shaderProgram, "projection", &projection[0][0]);
        /*unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);*/
        
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            mat4 model;
            glm_mat4_identity(model);
            glm_translate(model, cubePosition[i]);
            vec3 rotVec2;
            glm_vec3_zero(rotVec2);
            rotVec2[0] = 0.0f;
            rotVec2[1] = 0.3f;
            rotVec2[2] = 0.5f;
            float angle = 20.0f * i;
            if (i == 0) {
                glm_rotate(model, timeValue, rotVec2);
            }
            else {
                glm_rotate(model, angle, rotVec2);
            }
            setMat4(shaderProgram, "model", model);
            /*unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);*/
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

