// based on learnopengl.com tutorial

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

// in case of window resize to new width, height
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// escape key -> close
void processInput(GLFWwindow* window);

// check whether shader was successfully compiled
void checkShader(GLenum shaderType, unsigned int id) {
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
        glDeleteShader(id);
    }
}

int main()
{

    // version and core profile hinting //////////////////////////////
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // option, value
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // option, value
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // size of rendering window
    glViewport(0, 0, WIDTH, HEIGHT);

    // in case of window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //////////////////////////////////////////////////////////////////

    // vertex shader source
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // output
        "}\0"; // null terminated

    // create, bind, compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment/pixel shader source
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0"; // null terminated

    // create and bind fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compilation
    checkShader(GL_VERTEX_SHADER, vertexShader);
    checkShader(GL_FRAGMENT_SHADER, fragmentShader);

    // link to shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for shader linking
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" <<
            infoLog << std::endl;
    }

    // activate the program with shaders
    glUseProgram(shaderProgram);

    /////////////////////////////////////////////////////////////////

    // vertices
    float vertices[] = {
        0.5f,   0.5f,  0.0f, // top right, 0
        0.5f,   -0.5f,  0.0f, // bottom right, 1
        -0.5f,  -0.5f,  0.0f, // bottom left, 2
        -0.5f,  0.5f,   0.0f // top lefft, 3
    };

    unsigned int indices[] = {
        0, 1, 3, // upper right triangle
        1, 2, 3 // lower left triangle
    };

    // generate vertex array object for triangle/rectangle
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // generate indices buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify how to read the vertices / vertex attrib config
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /////////////////////////////////////////////////////////////

    // comment to enable/disable wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // keep drawing images until explicitly told to stop
    // render loop (just an example)
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering: clearing the screen with our colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window); // prevent flickering
        glfwPollEvents(); // check for event
    }

    // delete shaders, no longer used
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // clean/delete GLFW's allocated resources
    glfwTerminate();

    return 0;
}




// in case of window resize to new width, height
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// escape key -> close
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}