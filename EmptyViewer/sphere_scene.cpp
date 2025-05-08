#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>   
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include"Camera.h"

using namespace glm;



// 전역 변수
int     gNumVertices = 0;    // Number of 3D vertices.
int     gNumTriangles = 0;    // Number of triangles.
int* gIndexBuffer = nullptr; // Vertex indices for the triangles.
std::vector<glm::vec3> gVertexBuffer; // Vertex positions
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;

void create_scene()
{
    int width = 32;
    int height = 16;

    float theta, phi;
    int t = 0;

    gNumVertices = (height - 2) * width + 2;
    gNumTriangles = (height - 3) * (width - 1) * 2 + (width - 1) * 2;

    gVertexBuffer.resize(gNumVertices);

    gIndexBuffer = new int[3 * gNumTriangles];

    for (int j = 1; j < height - 1; ++j)
    {
        theta = (float)j / (height - 1) * M_PI;

        for (int i = 0; i < width; ++i)
        {
            phi = (float)i / (width - 1) * 2 * M_PI;

            float x = sinf(theta) * cosf(phi);
            float y = cosf(theta);
            float z = -sinf(theta) * sinf(phi);

            gVertexBuffer[t++] = glm::vec3(x, y, z);
        }
    }
  

    gVertexBuffer[t++] = glm::vec3(0.0f, 1.0f, 0.0f);

    gVertexBuffer[t++] = glm::vec3(0.0f, -1.0f, 0.0f);

    t = 0;

    for (int j = 0; j < height - 3; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            gIndexBuffer[t++] = j * width + i;
            gIndexBuffer[t++] = (j + 1) * width + (i + 1);
            gIndexBuffer[t++] = j * width + (i + 1);
            gIndexBuffer[t++] = j * width + i;
            gIndexBuffer[t++] = (j + 1) * width + i;
            gIndexBuffer[t++] = (j + 1) * width + (i + 1);
        }
    }
    for (int i = 0; i < width - 1; ++i)
    {
        gIndexBuffer[t++] = (height - 2) * width;
        gIndexBuffer[t++] = i;
        gIndexBuffer[t++] = i + 1;
        gIndexBuffer[t++] = (height - 2) * width + 1;
        gIndexBuffer[t++] = (height - 3) * width + (i + 1);
        gIndexBuffer[t++] = (height - 3) * width + i;
    }
}

void resize_callback(GLFWwindow*, int nw, int nh)
{
    //This is called in response to the window resizing.
    //The new width and height are passed in so we make 
    //any necessary changes:
    Width = nw;
    Height = nh;
    //Tell the viewport to use all of our screen estate
    glViewport(0, 0, nw, nh);

    //This is not necessary, we're just working in 2d so
    //why not let our spaces reflect it?
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, static_cast<double>(Width)
        , 0.0, static_cast<double>(Height)
        , 1.0, -1.0);

    //Reserve memory for our render so that we don't do 
    //excessive allocations and render the image
    OutputImage.reserve(Width * Height * 3);
	create_scene();
}

    
int main(int argc, char* argv[])
{
    // -------------------------------------------------
    // Initialize Window
    // -------------------------------------------------

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //We have an opengl context now. Everything from here on out 
    //is just managing our window or opengl directly.

    //Tell the opengl state machine we don't want it to make 
    //any assumptions about how pixels are aligned in memory 
    //during transfers between host and device (like glDrawPixels(...) )
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    //We call our resize function once to set everything up initially
    //after registering it as a callback with glfw
    glfwSetFramebufferSizeCallback(window, resize_callback);
    resize_callback(NULL, Width, Height);

    glEnable(GL_DEPTH_TEST);

    create_scene();

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f));
    model = glm::scale(model, glm::vec3(2.0f));

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = camera.getProjectionMatrix();

    glm::mat4 mv = view * model;
    glm::mat4 mvp = proj * mv;

    // -------------------------------------------------
    // Main Loop
    // -------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 프로젝션 행렬과 뷰 행렬 적용
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(proj));
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(mv));

        // 구체를 그리기


        glBegin(GL_TRIANGLES);
        for (int i = 0; i < gNumTriangles; ++i)
        {
            glVertex3f(gVertexBuffer[gIndexBuffer[i * 3]].x,
                gVertexBuffer[gIndexBuffer[i * 3]].y,
                gVertexBuffer[gIndexBuffer[i * 3]].z);
            glVertex3f(gVertexBuffer[gIndexBuffer[i * 3 + 1]].x,
                gVertexBuffer[gIndexBuffer[i * 3 + 1]].y,
                gVertexBuffer[gIndexBuffer[i * 3 + 1]].z);
            glVertex3f(gVertexBuffer[gIndexBuffer[i * 3 + 2]].x,
                gVertexBuffer[gIndexBuffer[i * 3 + 2]].y,
                gVertexBuffer[gIndexBuffer[i * 3 + 2]].z);
        }
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

