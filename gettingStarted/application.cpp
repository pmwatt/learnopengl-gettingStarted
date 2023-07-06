// based on learnopengl.com tutorial

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>


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

    //////////////////////////////////////////////////////////////////

    // create window object //////////////////////////////////////////
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    glViewport(0, 0, 80, 600);

    // in case of window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // generate vertices
    unsigned int VBO;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // specify how to read the vertices

    // keep drawing images until explicitly told to stop
    // render loop (just an example)
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // rendering: clearing the screen with our colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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