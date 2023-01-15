#include <glad/glad.h> // helps loads drivers
#include <GLFW/glfw3.h> // window system
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // prototype for window resize func
void processInput(GLFWwindow* window); // prototype for input function

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// vertex data for the triangle
const float vertices1[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    0.75f, 0.5f, 0.0f, // farther top right
};

const unsigned int indices1[] = {
        0, 1, 2,
};

const float vertices2[] = {
        -0.5f, 0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.75f, 0.5f, 0.0f, // farther top left
};

const unsigned int indices2[] = {
        0, 1, 2
};

// vertex shader source
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

const char* orangeFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "   fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

const char* yellowFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "   fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\n";

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

    int success;
    char infoLog[512];
    // Creating vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating fragment shader
    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &orangeFragmentShaderSource, NULL);
    glCompileShader(fragmentShader1);
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!infoLog) {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // creating the shader program 1
    unsigned int shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
       glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating fragment shader
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!infoLog) {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // creating the shader program 2
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader); // no longer needed after linking
    glDeleteShader(fragmentShader1); // no longer needed after linking
    glDeleteShader(fragmentShader2); // no longer needed after linking

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW); // copying vertex data into VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW); // index array stuff
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); // Telling OpenGL how to interpret vertex data
    glEnableVertexAttribArray(0);
    // Rendering initialization for triangle 2
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices2, GL_STATIC_DRAW); // copying vertex data into VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices2, GL_STATIC_DRAW); // index array stuff
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); // Telling OpenGL how to interpret vertex data
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // saying we want to draw wireframes

    // OpenGL window configuration
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // tell OpenGL the size of the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // setting the callback func for window resize
    // Render loop
    while (!glfwWindowShouldClose(window)) { // checks if GLFW has been told to close the window
        // INPUT
        processInput(window); // processing user input

        // RENDERING
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind

        // EVENTS AND SWAP BUFFERS
        glfwSwapBuffers(window); // swap the color buffer (represents pixels on the window)
        glfwPollEvents(); // checks if any io events have been triggered
    }

    // de-allocation
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

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