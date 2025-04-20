#include "glad.h"
#include "glfw3.h"

#include <iostream>

// Callback to adjust viewport on window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex shader source remains the same for both objects
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

// Single fragment shader (orange color)
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Younus", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Build and compile shader program
    // ---------------------------------
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // (Optional: check for shader compile errors)

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // (Optional: check for shader compile errors)

    // Shader program linking
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // (Optional: check for linking errors)

    // Delete shaders once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for an orange square (made of two triangles) on the left side
    float squareVertices[] = {
        // First triangle
        -0.9f, -0.5f, 0.0f,  // bottom left
        -0.1f, -0.5f, 0.0f,  // bottom right
        -0.9f,  0.5f, 0.0f,  // top left
        // Second triangle
        -0.9f,  0.5f, 0.0f,  // top left
        -0.1f, -0.5f, 0.0f,  // bottom right
        -0.1f,  0.5f, 0.0f   // top right
    };

    // Vertex data for an obtuse angled triangle on the right side
    // Chosen vertices: (0.3, -0.3), (0.9, -0.3), (0.2, 0.8)
    // The layout ensures that one angle becomes obtuse
    float triangleVertices[] = {
         0.3f, -0.5f, 0.0f,  // vertex A
         0.9f, -0.5f, 0.0f,  // vertex B
         0.0f,  0.5f, 0.0f   // vertex C
    };

    // Setup VAO and VBO for square
    unsigned int VAO_square, VBO_square;
    glGenVertexArrays(1, &VAO_square);
    glGenBuffers(1, &VBO_square);

    glBindVertexArray(VAO_square);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Unbind to avoid accidental modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Setup VAO and VBO for triangle
    unsigned int VAO_triangle, VBO_triangle;
    glGenVertexArrays(1, &VAO_triangle);
    glGenBuffers(1, &VBO_triangle);

    glBindVertexArray(VAO_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Unbind 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Set white background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader program for both shapes
        glUseProgram(shaderProgram);

        // Draw the square using its VAO (6 vertices forming 2 triangles)
        glBindVertexArray(VAO_square);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw the triangle using its VAO (3 vertices)
        glBindVertexArray(VAO_triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean-up: de-allocate resources
    glDeleteVertexArrays(1, &VAO_square);
    glDeleteBuffers(1, &VBO_square);
    glDeleteVertexArrays(1, &VAO_triangle);
    glDeleteBuffers(1, &VBO_triangle);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GLFW callback: adjust the viewport when the window size changes.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
