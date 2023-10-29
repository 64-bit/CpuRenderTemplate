#include <SDL.h>
#include <GL/glew.h>

#include "GenTexture.h"
#include <string.h>
#include <windows.h>
#include <vector>


GLuint glCreatePrograms() 
{
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D ourTexture;
        void main() {
            FragColor = texture(ourTexture, TexCoord);
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void RunTestOpenGL(int width, int height, int frames)
{
    SDL_Window* window = SDL_CreateWindow("OpenGL", 400, 400, width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();

    SDL_GL_SetSwapInterval(0);

    char* textureA = GenerateTexture(width, height, 0);
    char* textureB = GenerateTexture(width, height, 1);
    size_t textureSize = width * height * 4;

    bool running = true;
    bool isOdd = false;

    LARGE_INTEGER lastFrameTime;

    std::vector<float> times;

    GLuint vao, vbo, ebo, tex, shaderProg;

    shaderProg = 0;

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenTextures(1, &tex);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shaderProg = glCreatePrograms();

    //Warm up
    for (int i = 0; i < 20; i++)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
        }

        char* texture = isOdd ? textureA : textureB;
        isOdd = !isOdd;

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);

        glUseProgram(shaderProg);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, tex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    QueryPerformanceCounter(&lastFrameTime);

    for (int i = 0; i < frames; i++)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
        }

        char* texture = isOdd ? textureA : textureB;
        isOdd = !isOdd;

        ///////////////////////////////////////////////
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);

        glUseProgram(shaderProg);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, tex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
        ///////////////////////////////////////////////
        LARGE_INTEGER currentFrameTime, freq;
        QueryPerformanceCounter(&currentFrameTime);
        QueryPerformanceFrequency(&freq);

        double frameTime = static_cast<double>(currentFrameTime.QuadPart - lastFrameTime.QuadPart) / freq.QuadPart;

        frameTime *= 1000.0;

        times.push_back(frameTime);

        lastFrameTime = currentFrameTime;
    }

    float totalTime = 0.0f;
    float minFrame = 1000000.0;
    float maxFrame = 0.0f;

    for (int i = 0; i < times.size(); i++)
    {
        float t = times[i];
        totalTime += t;
        minFrame = min(minFrame, t);
        maxFrame = max(maxFrame, t);
    }

    float averageTime = totalTime / frames;

    printf("OpenGL Results:\n");

    printf("Average: %f\n", averageTime);
    printf("Min: %f\n", minFrame);
    printf("Max: %f\n", maxFrame);

    printf("\n\n");

    SDL_DestroyWindow(window);
}