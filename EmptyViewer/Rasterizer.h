#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>   
#include <algorithm> 
#include <cmath>
#include <iostream>

class Rasterizer {
public:
    int width, height;
    std::vector<uint8_t> framebuffer;  // RGB * width * height
    std::vector<float> depthBuffer;

    Rasterizer(int w, int h) : width(w), height(h) {
        framebuffer.resize(w * h * 3, 0); // RGB
        depthBuffer.resize(w * h, std::numeric_limits<float>::infinity());
    }

    void clear() {
        std::fill(framebuffer.begin(), framebuffer.end(), 0);
        std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
    }

    void draw(const std::vector<glm::vec3>& vertices, const int* indexBuffer, const glm::mat4& mvp, int numTriangles)
    {
        for (int i = 0; i < numTriangles; ++i)
        {
            int idx1 = indexBuffer[i * 3];
            int idx2 = indexBuffer[i * 3 + 1];
            int idx3 = indexBuffer[i * 3 + 2];

            glm::vec4 clip1 = mvp * glm::vec4(vertices[idx1], 1.0f);
            glm::vec4 clip2 = mvp * glm::vec4(vertices[idx2], 1.0f);
            glm::vec4 clip3 = mvp * glm::vec4(vertices[idx3], 1.0f);

            // Perspective divide to NDC
            glm::vec4 ndc1 = clip1 / clip1.w;
            glm::vec4 ndc2 = clip2 / clip2.w;
            glm::vec4 ndc3 = clip3 / clip3.w;

            // Convert to screen space
            glm::vec3 screen1 = toScreenSpace(ndc1);
            glm::vec3 screen2 = toScreenSpace(ndc2);
            glm::vec3 screen3 = toScreenSpace(ndc3);

            // Rasterize
            rasterizeTriangle(screen1, screen2, screen3);
        }
    }

private:
    glm::vec3 toScreenSpace(const glm::vec4& ndc) {
        float x = (ndc.x * 0.5f + 0.5f) * width;
        float y = (1.0f - (ndc.y * 0.5f + 0.5f)) * height; // y-flip (OpenGL은 좌표계를 다르게 사용)
        float z = ndc.z;
        return glm::vec3(x, y, z);
    }

    void rasterizeTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
        // Calculate the bounding box for the triangle
        float minX = std::max(0.0f, std::floor(std::min({ p0.x, p1.x, p2.x })));
        float maxX = std::min((float)(width - 1), std::ceil(std::max({ p0.x, p1.x, p2.x })));
        float minY = std::max(0.0f, std::floor(std::min({ p0.y, p1.y, p2.y })));
        float maxY = std::min((float)(height - 1), std::ceil(std::max({ p0.y, p1.y, p2.y })));

        glm::vec2 v0 = glm::vec2(p0.x, p0.y);
        glm::vec2 v1 = glm::vec2(p1.x, p1.y);
        glm::vec2 v2 = glm::vec2(p2.x, p2.y);

        // Calculate the area of the triangle (used for edge function)
        float area = edgeFunction(v0, v1, v2);
        if (area == 0.0f) return;

        // Loop over the bounding box and check if each pixel is inside the triangle
        for (int y = (int)minY; y <= (int)maxY; ++y) {
            for (int x = (int)minX; x <= (int)maxX; ++x) {
                glm::vec2 p(x + 0.5f, y + 0.5f);  // Pixel center

                // Compute the barycentric coordinates for the point
                float w0 = edgeFunction(v1, v2, p);
                float w1 = edgeFunction(v2, v0, p);
                float w2 = edgeFunction(v0, v1, p);

                // Check if the point is inside the triangle
                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;

                    // Interpolate the depth using barycentric coordinates
                    float depth = w0 * p0.z + w1 * p1.z + w2 * p2.z;

                    int idx = y * width + x;
                    if (depth < depthBuffer[idx]) {
                        depthBuffer[idx] = depth;

                        // Set pixel color (for simplicity, we're using red here)
                        framebuffer[idx * 3 + 0] = 255; // R
                        framebuffer[idx * 3 + 1] = 0;   // G
                        framebuffer[idx * 3 + 2] = 0;   // B
                    }
                }
            }
        }
    }

    // Edge function to calculate the area and barycentric coordinates
    float edgeFunction(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }
};
