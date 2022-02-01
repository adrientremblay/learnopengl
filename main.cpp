#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Callback function for window resizes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Initializing GLFW 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initializing GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // Telling OpenGL viewport size
    glViewport(0, 0, 800, 600);

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // Creating Vertex Buffer Object (VBO)
    // Ie. moving vertex data to VRAM
    unsigned int buffer; // unsigned int to store id of created buffer
    glGenBuffers(1, &buffer); // generating one buffer, storing the id in 'buffer'
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // selecting a buffer to use
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // giving the buffer some data
    // We must tell OpenGl how the data in the VBO is layed out
    // Index =  value in each vertex for each attribute
    // size = the number of attributes per vertex
    // stride = the total amount of bytes per vertex
    // pointer = byte offset to the vertex normal attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // Render Loop
    while (!glfwWindowShouldClose(window)) {

        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing triangle
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw command

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up GLFW resources
    glfwTerminate();
    return 0;
}