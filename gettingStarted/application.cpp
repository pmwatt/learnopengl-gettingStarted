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
        "layout (location = 0) in vec3 aPos;\n" // attrib position 0
        "layout (location = 1) in vec3 aColor;\n" // attrib position 1
        "out vec3 ourColor;\n" // output to pixel shader
        "void main()\n"
        "{\n"
        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // output
        "ourColor = aColor;\n" // set ourColor to input color from the vertex data
        "}\0"; // null terminated

    // create, bind, compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment/pixel shader source
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n" // from vertex shader
        "void main()\n"
        "{\n"
        "FragColor = vec4(ourColor, 1.0);\n" // casting
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

    // delete shaders, no longer used
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /////////////////////////////////////////////////////////////////

    // vertices
    float vertices[] = {
        // positions            // colors
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f // top
    };

    // generate vertex buffer array and bind (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // specify how to read the vertices / vertex attrib config
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // cast to (void*)?
    glEnableVertexAttribArray(1);

    /////////////////////////////////////////////////////////////

    // keep drawing images until explicitly told to stop
    // render loop (just an example)
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering: clearing the screen with our colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update uniform colour
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // get uniform id
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // set uniform

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // prevent flickering
        glfwPollEvents(); // check for event
    }

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