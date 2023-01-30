#include <glad/glad.h> // helps loads drivers
#include <GLFW/glfw3.h> // window system
#include <iostream>
#include <cmath>
#include "include/Shader.h"
#include "include/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // prototype for window resize func
void processInput(GLFWwindow* window); // prototype for input function

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// vertex data for the triangle
const float vertices[] = {
        // positions                                         colors                               texture coords
    0.5f, 0.5f, 0.0f,                    1.0f, 0.0f, 0.0f,              1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,                   0.0f, 1.0f, 0.0f,            1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,                   0.0f, 0.0f, 1.0f,             0.0f, 0.0f,
   -0.5f, 0.5f, 0.0f,                   1.0f, 1.0f, 1.0f,             0.0f, 1.0f
};

const unsigned int indices[] = {
        0, 2, 3,     1, 0, 2
};

int main () {
    // Initialization and configuration of GLFW
    glfwInit(); // initialize the GLFW library
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Telling GLFW we want to use OpenGL version 3 ...
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // point 3 so version 3,3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // telling GLFW we want to use OpenGL Core profile
    // Making a GLFW window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL); // generate a window
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // set it as the context for the current thread

    // Loading GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    Shader ourShader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

    unsigned int VBOs[2], VAOs[2], EBOs[2];
    // Creating Vertex Buffer Object (VBOs) so we can store vertex data in VRAM on GPU
    glGenBuffers(2, VBOs);
    // creating VAOs to store rendering meta info
    glGenVertexArrays(2, VAOs);
    // creating EBOs to store rendering indices
    glGenBuffers(2, EBOs);

    // Rendering initialization for triangle 1
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copying vertex data into VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // index array stuff
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); // Telling OpenGL how to interpret vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); // Telling OpenGL how to interpret vertex data
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); // Telling OpenGL how to interpret vertex data
    glEnableVertexAttribArray(2);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // saying we want to draw wireframes

    Texture texture1 = Texture("images/adrien.jpeg");
    Texture texture2 = Texture("images/anime.jpg");

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader.shaderProgram, "texture2"), 1);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // OpenGL window configuration
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // tell OpenGL the size of the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // setting the callback func for window resize

    // Render loop
    float startTimeValue = glfwGetTime();
    while (!glfwWindowShouldClose(window)) { // checks if GLFW has been told to close the window
        // INPUT
        processInput(window); // processing user input

        // RENDERING
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = ((cos(timeValue)) + 0.5f);
        ourShader.use();
        ourShader.setVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        ourShader.setVec3f("aPosOffset", sin(timeValue - startTimeValue) - 0.25f, 0.0f, 0.0f);

        /*
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(sin(timeValue - startTimeValue) - 0.25f, 0.0f, 0.0f));
        trans = glm::rotate(trans, timeValue, glm::vec3(0.0, 0.0, -1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
         */
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1.textureId);
        glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture2.textureId);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind

        // EVENTS AND SWAP BUFFERS
        glfwSwapBuffers(window); // swap the color buffer (represents pixels on the window)
        glfwPollEvents(); // checks if any io events have been triggered
    }

    // de-allocation
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
//    glDeleteProgram(shaderProgram1); todo: deallocate with the shader class

    glfwTerminate(); // clean up GLFW resources
    return 0;
}

// window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// process input callback
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}