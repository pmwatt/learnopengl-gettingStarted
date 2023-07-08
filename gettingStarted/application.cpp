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
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // orange
        "}\0"; // null terminated

    // fragment/pixel shader source
    const char* fragmentShaderSource2 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n" // yellow
        "}\0"; // null terminated

    // create and bind fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // create and bind the 2nd fragment shader
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    // check for shader compilation
    checkShader(GL_VERTEX_SHADER, vertexShader);
    checkShader(GL_FRAGMENT_SHADER, fragmentShader);

    // link to shader program
    unsigned int shaderProgram[2];
    shaderProgram[0] = glCreateProgram();
    shaderProgram[1] = glCreateProgram();

    glAttachShader(shaderProgram[0], vertexShader); // same vertex shader
    glAttachShader(shaderProgram[1], vertexShader);
    glLinkProgram(shaderProgram[0]);

    glAttachShader(shaderProgram[0], fragmentShader); // different pixel shader
    glAttachShader(shaderProgram[1], fragmentShader2);
    glLinkProgram(shaderProgram[1]);

    // check for shader linking
    for (int i = 0; i < 2; ++i) {
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram[i], GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << i << "::LINKING_FAILED\n" <<
                infoLog << std::endl;
        }
    }

    /////////////////////////////////////////////////////////////////

    // vertices
    float vertices1[] = {
        // tirnagle 1
        0.5f,   0.5f,  0.0f, // top right
        0.5f,   -0.5f,  0.0f, // bottom right
        -0.5f,  -0.5f,  0.0f // bottom left
    };
    float vertices2[] = {
        // triangle 2
        -0.5f,  -0.5f,  0.0f, // bottom left
        -0.5f,  0.5f,   0.0f, // top left
        0.5f,   0.5f,  0.0f // top right
    };

    // generate vertex array object for triangle/rectangle
    unsigned int VAO1, VAO2;
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);

    // generate vertex buffer
    glBindVertexArray(VAO1);
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /////////////////////////////////////////////////////////////

    // comment to enable/disable wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // keep drawing images until explicitly told to stop
    // render loop (just an example)
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering: clearing the screen with our colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram[0]); // orange
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram[1]); // yellow
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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