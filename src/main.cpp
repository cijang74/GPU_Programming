#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define ALLOW_REPEAT_KEYSTROKE 1

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float rgb[3] = { 0.2f, 0.3f, 1.0f };
void clamp(float &value, float min_value, float max_value)
{
    if (value < min_value) value = min_value;
    if (value > max_value) value = max_value;
}
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

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(rgb[0], rgb[1], rgb[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

#if ALLOW_REPEAT_KEYSTROKE
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        rgb[0] += 0.001f;
        clamp(rgb[0], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        rgb[0] -= 0.001f;
        clamp(rgb[0], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
        rgb[1] += 0.001f;
        clamp(rgb[1], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
    {
        rgb[1] -= 0.001f;
        clamp(rgb[1], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        rgb[2] += 0.001f;
        clamp(rgb[2], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
    {
        rgb[2] -= 0.001f;
        clamp(rgb[2], 0.0f, 1.0f);
    }
#else
    static bool pressed_f1_f6[6] = {false, };
    for (int i = 0; i < 6; i++)
    {
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_PRESS)
            pressed_f1_f6[i] = true;
  
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_RELEASE && pressed_f1_f6[i] == true)
        {
            if (i % 2 == 0) rgb[i / 2] += 0.1f;
            else rgb[i / 2] -= 0.1f;
            clamp(rgb[i / 2], 0.0f, 1.0f);
            pressed_f1_f6[i] = false;

            printf("backgroundColor : %.2f %.2f %.2f\n", rgb[0], rgb[1], rgb[2]);
        }
    }
#endif
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}