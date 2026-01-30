#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cglm/cglm.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vec3 cameraPos;
vec3 cameraFront;
vec3 cameraUp;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
int firstMouse = 1;

vec3 lightPos;

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    printf("lastpos x:%f and y:%f \n", lastX, lastY);
    printf("current mouse pos x: %f and y: %f\n", xpos, ypos);
    if (firstMouse == 1) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    printf("offsets changed by sensitivity x:%f and y:%f\n", xoffset, yoffset);

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    vec3 direction;
    glm_vec3_zero(direction);
    direction[0] = cos(yaw) * cos(pitch);
    direction[1] = sin(pitch);
    direction[2] = sin(yaw) * cos(pitch);
    printf("direction mouse test x:%f, y:%f, z:%f \n", direction[0], direction[1], direction[2]);
    printf("cameraFront mouse test x:%f, y:%f, z:%f \n", cameraFront[0], cameraFront[1], cameraFront[2]);
    glm_vec3_normalize_to(direction, cameraFront);
    printf("cameraFront after normalize test x:%f, y:%f, z:%f \n", cameraFront[0], cameraFront[1], cameraFront[2]);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    printf("scroll offset x: %f and y: %f, fov: %f \n", xoffset, yoffset, fov);
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_muladds(cameraFront, cameraSpeed, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_mulsubs(cameraFront, cameraSpeed, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 crossResultLeft;
        glm_vec3_cross(cameraFront, cameraUp, crossResultLeft);
        glm_vec3_normalize(crossResultLeft);
        glm_vec3_mulsubs(crossResultLeft, cameraSpeed, cameraPos);
        //cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //cameraPos += cameraSpeed * cameraFront;
        vec3 crossResultRight;
        glm_vec3_cross(cameraFront, cameraUp, crossResultRight);
        glm_vec3_normalize(crossResultRight);
        glm_vec3_muladds(crossResultRight, cameraSpeed, cameraPos);
    }
    // reset everything back to origin position
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glm_vec3_zero(cameraPos);
        glm_vec3_zero(cameraFront);
        glm_vec3_zero(cameraUp);
        cameraPos[2] = 3.0f;
        cameraFront[2] = -1.0f;
        cameraUp[1] = 1.0f;
        printf("reset cam on cameraFront test x:%f, y:%f, z:%f\n", cameraFront[0], cameraFront[1], cameraFront[2]);
        printf("reset cam on cameraPos test x:%f, y:%f, z:%f\n", cameraPos[0], cameraPos[1], cameraPos[2]);
        printf("reset cam on cameraUp test x:%f, y:%f, z:%f\n", cameraUp[0], cameraUp[1], cameraUp[2]);
        yaw = -90.0f;
        pitch = 0.0f;
        fov = 45.0f;
    }
}

// define pointers in here then use it for functions in the other 
// to do heap allocation free
int main(int argc, char* argv[]) {
    printf("Hello, from color 3dCgraphics!\n");
    glm_vec3_zero(cameraPos);
    glm_vec3_zero(cameraFront);
    glm_vec3_zero(cameraUp);
    cameraPos[2] = 3.0f;
    cameraFront[2] = -1.0f;
    cameraUp[1] = 1.0f;
    glm_vec3_zero(lightPos);
    lightPos[0] = 1.2f;
    lightPos[1] = 1.0f;
    lightPos[2] = 2.0f;
    // how glfw will handle errors via a provided callback
    // glfwSetErrorCallback(error_callback);
    char* vertexPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\lighting\\lvertex.glsl\0";

    char* fragmentPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\lighting\\lfragment.glsl\0";
    // need a lighting path for color vertex
    char* cVertexPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\lighting\\lcvertex.glsl\0";
    char* cFragmentPath = "C:\\Users\\Pigeon_Borb\\Desktop\\learn\\game dev and physics\\cbased\\openglC\\src\\lighting\\lcfragment.glsl\0";

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


    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);

    if (!window) {
        printf("Failed to create window");
        glfwTerminate();
        return -1;
    }

    // this is needed before you can use OpenGL
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = compileShaders(vertexPath, fragmentPath);
    unsigned int colorProgram = compileShaders(cVertexPath, cFragmentPath);
    // vertex shader compilation
    printf("shaderProgram %u \n colorProgram %u \n", shaderProgram, colorProgram);

    // 3D box vertices
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
    };


    // VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);

     // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // lighting color attribute
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load the texture for gpu params


    // loading from file



    // use this to see the polygon shape of the rendered triangles
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {

        float timeValue = glfwGetTime();
        deltaTime = timeValue - lastFrame;
        lastFrame = timeValue;

        // Keep running until user closes window
        // input handle method for now
        processInput(window);
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        useShader(shaderProgram);
        setVec3(shaderProgram, "objectColor", 1.0f, 0.5f, 0.31f);
        setVec3(shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);

        // projection stuff
        mat4 projection;
        glm_mat4_identity(projection);
        glm_perspective(fov, 800.0f / 600.0f, 0.1f, 100.0f, projection);

         // camera position - relative to z-axis
        mat4 view;

        vec3 cameraPosFront;
        glm_vec3_add(cameraPos, cameraFront, cameraPosFront);

        glm_lookat(cameraPos, cameraPosFront, cameraUp, view);

        // note that we're translating the scene in the reverse direction of where we want to move
        vec3 vTrans;
        glm_vec3_zero(vTrans);
        //vTrans[1] = -1.5f;
        vTrans[2] = -3.0f;

        glm_translate(view, vTrans);




        /* unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);*/
            //setMat4(shaderProgram, "model", model);
        setMat4(shaderProgram, "view", view);
        setMat4(shaderProgram, "projection", projection);
        /*unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);*/

       
        mat4 model;
        glm_mat4_identity(model);
        //glm_translate(model, cubePosition[i]);
       /* vec3 rotVec2;
        glm_vec3_zero(rotVec2);
        rotVec2[0] = 0.0f;
        rotVec2[1] = 0.3f;
        rotVec2[2] = 0.0f;
        glm_rotate(model, timeValue, rotVec2);*/
        setMat4(shaderProgram, "model", model);
        glBindVertexArray(VAO);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // world transform
        useShader(colorProgram);
        setMat4(colorProgram, "projection", projection);
        setMat4(colorProgram, "view", view);
        glm_mat4_identity(model);
        glm_translate(model, lightPos);
        // shrink cube by every side
        vec3 shrinkScale;
        glm_vec3_zero(shrinkScale);
        shrinkScale[0] = 0.2f;
        shrinkScale[1] = 0.2f;
        shrinkScale[2] = 0.2f;
        glm_scale(model, shrinkScale);
        setMat4(colorProgram, "model", model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(colorProgram);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

