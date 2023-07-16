// based on learnopengl.com tutorial

#include "shader.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

// (16/07/23) from https://github.com/nothings/stb/blob/master/stb_image.h
// for loading images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

// in case of window resize to new width, height
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// escape key -> close
void processInput(GLFWwindow* window);


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

    // shader
    Shader shaderProgram{ ".\\Shaders\\shader.vert", ".\\Shaders\\shader.frag" };

    // vertices
    float vertices[] = {
        // positions            // colors               // texture coords
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,       0.0f, 1.0f // top left
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
    // position attribute (xyz)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute (rgb)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute (s&t)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    /////////////////////////////////////////////////////////////

    // load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture wrapping for s & t (as repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering for minifying/magnifying
    // (as linear interpolation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(".\\Images\\container.jpg", &width, &height,
        &nrChannels, 0);

    // generate mipmap if successful
    if (data) {
        glTexImage2D(GL_TEXTURE, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture\n";
    }
    // cleanup texture
    stbi_image_free(data);

    /////////////////////////////////////////////////////////////

    // keep drawing images until explicitly told to stop
    // render loop (just an example)
    float offset = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // background colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        // draw texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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