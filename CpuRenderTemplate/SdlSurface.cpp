#include <SDL.h>
#include "GenTexture.h"
#include <string.h>
#include <windows.h>
#include <vector>

void RunTestSurface(int width, int height, int frames) 
{
	SDL_Window* window = SDL_CreateWindow("SDL Surface Test", 100, 100,
		width, height, SDL_WINDOW_SHOWN);

	auto windowSurface = SDL_GetWindowSurface(window);

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

        memcpy(windowSurface->pixels, texture, textureSize);
        SDL_UpdateWindowSurface(window);
    }

    QueryPerformanceCounter(&lastFrameTime);

    for(int i = 0; i < frames; i++)
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

        memcpy(windowSurface->pixels, texture, textureSize);
        SDL_UpdateWindowSurface(window);

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

    printf("Surface Results:\n");

    printf("Average: %f\n", averageTime);
    printf("Min: %f\n", minFrame);
    printf("Max: %f\n", maxFrame);

    printf("\n\n");

    SDL_DestroyWindow(window);
}