#include <glad/glad.h> // helps loads drivers
#include <GLFW/glfw3.h> // window system
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // prototype for window resize func
void processInput(GLFWwindow* window); // prototype for input function

// vertex data for the triangle
const float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, 0.5f, 0.0f, // top left
    -0.5f, -0.5f, 0.0f, // bottom left
};

const unsigned int indices[] = {
        0, 1, 3,
        0, 2, 3
};

// vertex shader source
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 fragColor;\n" // todo: can I change the name of this from FragColor? why camel case?
    "\n"
    "void main() {\n"
    "   fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

int main () {
    // Initialization and configuration of GLFW
    glfwInit(); // initialize the GLFW library
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Telling GLFW we want to use OpenGL version 3 ...
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // point 3 so version 3,3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // telling GLFW we want to use OpenGL Core profile
    // Making a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); // generate a window
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

    // Creating Vertex Buffer Object (VBO) so we can store vertex data in VRAM on GPU
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Creating vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int vertexShaderCompileSuccess;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompileSuccess);
    if (!vertexShaderCompileSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    int fragmentShaderCompileSuccess;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompileSuccess);
    if (!fragmentShaderCompileSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
    }

    // creating the shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int shaderProgramLinkSuccess;
    char infoLog3[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramLinkSuccess);
    if (!shaderProgramLinkSuccess) {
       glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog3 << std::endl;
    }
    glDeleteShader(vertexShader); // no longer needed after linking
    glDeleteShader(fragmentShader); // no longer needed after linking

    // creating VAO to store rendering info
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // creating EBO to store rendering indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Rendering initialization code
    glBindVertexArray(VAO);
    // Copying vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Index array stuff
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Telling OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // saying we want to draw wireframes

    // OpenGL window configuration
    glViewport(0, 0, 800, 600); // tell OpenGL the size of the viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // setting the callback func for window resize
    // Render loop
    while (!glfwWindowShouldClose(window)) { // checks if GLFW has been told to close the window
        // INPUT
        processInput(window); // processing user input

        // RENDERING
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // no need to explicitly bind since it's done when binding VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind

        // EVENTS AND SWAP BUFFERS
        glfwSwapBuffers(window); // swap the color buffer (represents pixels on the window)
        glfwPollEvents(); // checks if any io events have been triggered
    }

    // de-allocation
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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