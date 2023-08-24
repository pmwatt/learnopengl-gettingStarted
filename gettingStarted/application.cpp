// based on learnopengl.com tutorial

#include "shader.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// (16/07/23) from https://github.com/nothings/stb/blob/master/stb_image.h
// for loading images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>



////////////////////////////////////////////////////////////////


// error handling - end program
#define ASSERT(x) if (!(x)) __debugbreak() // for MSVC compiler

// error handling - end program and show line/file
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // # changes to string


////////////////////////////////////////////////////////////////
// GLOBAL VARS & FUNCTION DECLARATIONS


constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr float ALPHA_DIFF = 0.001;
constexpr float ALPHA_INIT = 0.5;
float alpha = 0.5; // 2nd texture's actual transparency, lazy storage


static void GLClearError();
static bool GLLogCall(const char* function, const char* file, int line);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window, Shader * shader);
GLFWwindow* setupWindow();



////////////////////////////////////////////////////////////////


int main()
{
    GLFWwindow* window = setupWindow();



    //////////////////////////////////////////////////////////////////


    int width, height, nrChannels; // temp vars

    // load textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    // flip images loaded by stbi
    stbi_set_flip_vertically_on_load(true);



    // set 1st texture unit ////////////////////
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set texture wrapping for s & t (as repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering for minifying/magnifying (as linear interpolation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char* data = stbi_load(".\\Images\\container.jpg", &width, &height,
        &nrChannels, 0);
    // generate mipmap if successful
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 1\n";
    }
    stbi_image_free(data);



    // set 2nd texture unit ///////////////////////////
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set texture wrapping for s & t (as repeat)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering for minifying/magnifying (as linear interpolation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load(".\\Images\\awesomeface.png", &width, &height,
        &nrChannels, 0);
    // generate mipmap if successful
    if (data) {
        // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 2\n";
    }
    stbi_image_free(data);



    ////////////////////////////////////////////////////////////////////




    float vertices[] = {
        // positions            // colors               // texture coords
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,       0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0, 1, 2, // bottom right triangle
        2, 3, 0 // upper left triangle
    };
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // EBO for reusing indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify how to read the vertices / vertex attrib config
    // position attribute (xyz)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute (rgb)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute (s&t)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //////////////////////////////////////////////////////////////


    Shader shaderProgram{ ".\\Shaders\\shader.vert", ".\\Shaders\\shader.frag" };
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);
    shaderProgram.setFloat("alpha", 0.5);

    /////////////////////////////////////////////////////////////

    // prepare to draw
    glBindVertexArray(VAO);

    // wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, &shaderProgram);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1); // crate
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2); // face


        // 1st rotating container
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(),
            glm::vec3(0.0f, 0.0f, 1.0f));
        // pass the trans matrix to the shader
        unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
       
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


        // 2nd scaling container
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        trans = glm::scale(trans, glm::vec3(sin((float)glfwGetTime()),
            sin((float)glfwGetTime()), 0.0f));
        // pass the trans matrix to the shader
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


        glfwSwapBuffers(window); // prevent flickering
        glfwPollEvents(); // check for event
    }

    // deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // clean/delete GLFW's allocated resources
    glfwTerminate();

    return 0;
}




////////////////////////////////////////////////////////////////





GLFWwindow* setupWindow() {
    // version and core profile hinting //////////////////////////////
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // for apple
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        __debugbreak();
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        __debugbreak();
    }

    // size of rendering window
    glViewport(0, 0, WIDTH, HEIGHT);

    // in case of window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    return window;
}

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR); // 0 or GL_NO_ERROR
}

static bool GLLogCall(const char* function, const char* file, int line) {
    bool isError = true;
    while (GLenum error = glGetError()) {
        isError = false;
        std::cout << "[OpenGL Error]" << "(" << error << "): " << function
            << " " << file << ": " << line << std::endl;
    }
    return isError;
}

// in case of window resize to new width, height
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// escape key -> close
void processInput(GLFWwindow* window, Shader* shader)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (alpha <= 1.0)
            alpha += ALPHA_DIFF;
        shader->setFloat("alpha", alpha);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (0.0 <= alpha)
            alpha -= ALPHA_DIFF;
        shader->setFloat("alpha", alpha);
    }
}