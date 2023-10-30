#include <SDL.h>
#include <GL/glew.h>
#include <string.h>
#include <windows.h>
#include <vector>

#include "GenTexture.h"
#include "FrameDisplayer.h"

void RunTestOpenGL(int width, int height, int frames)
{
    SDL_Window* window = SDL_CreateWindow("OpenGL", 400, 400, width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();

    SDL_GL_SetSwapInterval(0);

    FrameDisplayer displayer = FrameDisplayer();

    char* textureA = GenerateTexture(width, height, 0);
    char* textureB = GenerateTexture(width, height, 1);
    size_t textureSize = width * height * 4;

    bool running = true;
    bool isOdd = false;

    LARGE_INTEGER lastFrameTime;

    std::vector<float> times;

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

        displayer.DisplayFrame(width, height, texture);
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
        displayer.DisplayFrame(width, height, texture);
        SDL_GL_SwapWindow(window);
        ///////////////////////////////////////////////
        LARGE_INTEGER currentFrameTime, freq;
        QueryPerformanceCounter(&currentFrameTime);
        QueryPerformanceFrequency(&freq);

        // *1000 to convert to miliseconds
        double frameTime = 1000.0 * static_cast<double>(currentFrameTime.QuadPart - lastFrameTime.QuadPart) / freq.QuadPart;

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