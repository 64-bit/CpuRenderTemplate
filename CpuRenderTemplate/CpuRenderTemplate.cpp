// CpuRenderTemplate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SDL.h>
#undef main

#include <iostream>

const int WIDTH = 1920, HEIGHT = 1080;


void RunTestSurface(int width, int height, int frames);
void RunTestOpenGL(int width, int height, int frames);


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    RunTestSurface(WIDTH, HEIGHT, 1000);
    RunTestOpenGL(WIDTH, HEIGHT, 1000);

    //SDL_Window* window = SDL_CreateWindow("OpenGL", 400, 400, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    //SDL_GLContext context = SDL_GL_CreateContext(window);

    //bool running = true;
    //while (running)
    //{
    //    SDL_Event e;
    //    while (SDL_PollEvent(&e))
    //    {
    //        if (e.type == SDL_QUIT)
    //        {
    //            running = false;
    //        }
    //    }

    //    SDL_GL_SwapWindow(window);
    //}

    //SDL_GL_DeleteContext(context);
    //SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
