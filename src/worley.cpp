//Hayden Coffey
//2020
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include "WPoint.hpp"
#include "CommandLine.hpp"

uint32_t FPS_LIMIT = 60;
int32_t SCREEN_WIDTH = 400;
int32_t SCREEN_HEIGHT = 400;
int32_t DEPTH = 100;

float R = 1, G = 0, B = 0;
enum COLOR_STAGE
{
    RG,
    GB,
    BR,
    COLOR_STAGE_SIZE
};
float COLOR_DELTA = 0.01;

SDL_Window *WINDOW = nullptr;

int distance(int index, const WPoint *wp);

int threaded_drawWorleyNoise(SDL_Texture *texture, int pixelCount, SDL_PixelFormat *mappingFormat,
                             const std::vector<WPoint> *points, int threadCount);

void _threaded_drawWorleyNoise(uint32_t *pixels, const SDL_PixelFormat *mappingFormat,
                               const std::vector<WPoint> *points, int startIndex, int endIndex);

int main(int argc, char **argv)
{
    int threadCount = 1;
    int numPoints = 1;
    char *arg;
    arg = getCmdOption(argv, argv + argc, "-threads");
    if (arg)
    {
        threadCount = atoi(arg);
    }

    arg = getCmdOption(argv, argv + argc, "-points");
    if (arg)
    {
        numPoints = atoi(arg);
    }

    //Set up SDL to work with graphics
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    //Create an application window
    WINDOW = SDL_CreateWindow("Worley Noise",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!WINDOW)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Get the pixel format used by the created window
    SDL_PixelFormat *mappingFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(WINDOW));

    //Initialize a renderer and bind it to the window;
    uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(WINDOW, -1, render_flags);

    if (!rend)
    {
        printf("error creating renderer\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(WINDOW);
        SDL_Quit();
        return 1;
    }

    //Create a blank texture to draw the noise on
    SDL_Texture *texture = SDL_CreateTexture(rend, SDL_GetWindowPixelFormat(WINDOW),
                                             SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL)
    {
        fprintf(stderr, "Failed to create blank texture. SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    //Lock the texture to get a pointer to the pixel data, and the pitch value.
    uint32_t *pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, (void **)(&pixels), &pitch) != 0)
    {
        fprintf(stderr, "Failed to lock texture. SDL Error : %s\n", SDL_GetError());
        return 1;
    }
    SDL_UnlockTexture(texture);

    //Pitch is the width of the image in bytes, each pixel is 4 bytes.
    int pixelWidth = (pitch / 4);
    int pixelCount = pixelWidth * SCREEN_HEIGHT;

    //Let the WPoint class know the dimensions of the texture
    WPoint::initPixelSize(pixelWidth, SCREEN_HEIGHT, DEPTH);

    //Initialize the center points for the Worley Noise algorithm
    std::vector<WPoint> points;
    for (int i = 0; i < numPoints; i++)
    {
        WPoint tmp;
        tmp.setVelocity(1 - 2 * (rand()%2), 1 - 2 * (rand()%2), 1 - 2 * (rand()%2));
        points.push_back(tmp);
    }

    //Image color transition
    COLOR_STAGE color_stage = RG;

    //Animation Loop
    bool quit = false;
    std::chrono::_V2::system_clock::time_point time_start, time_end;
    std::chrono::duration<double> time_elapsed;
    while (!quit)
    {
        time_start = std::chrono::high_resolution_clock::now();

        //Quit if requested
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_q)
                {
                    quit = true;
                    break;
                }
            }
        }

        //Modify texture with Worley Noise pattern
        threaded_drawWorleyNoise(texture, pixelCount, mappingFormat, &points, threadCount);

        //Render texture
        SDL_Rect renderRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(rend, texture, NULL, &renderRect);
        SDL_RenderPresent(rend);

        //Update center point positions
        for (int i = 0; i < numPoints; i++)
        {
            points[i].step();
        }

        //Rotate through color modifiers over many iterations
        switch (color_stage)
        {
        case RG:
            R -= COLOR_DELTA;
            G += COLOR_DELTA;
            if (R <= 0)
            {
                R = 0;
                G = 1;
                color_stage = GB;
            }
            break;
        case GB:
            G -= COLOR_DELTA;
            B += COLOR_DELTA;
            if (G <= 0)
            {
                G = 0;
                B = 1;
                color_stage = BR;
            }
            break;
        case BR:
            B -= COLOR_DELTA;
            R += COLOR_DELTA;
            if (B <= 0)
            {
                B = 0;
                R = 1;
                color_stage = RG;
            }
            break;
        case COLOR_STAGE_SIZE:
            break;
        }

        //printf("%f %f %f\n", R, G, B);

        //FPS Limiter
        time_end = std::chrono::high_resolution_clock::now();
        time_elapsed = time_end - time_start;
        float _timeLeft = time_elapsed.count();
        _timeLeft = (1.0 / FPS_LIMIT) - _timeLeft;
        if (_timeLeft < 0)
            _timeLeft = 0;
        SDL_Delay(_timeLeft);
    }

    //Free up memory
    SDL_FreeFormat(mappingFormat);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();

    return 0;
}

//Get distance between pixel (via index in image) and a center point
int distance(int index, const WPoint *wp)
{
    int32_t width = wp->getPixelWidth();
    return pow(pow(wp->getZ(), 2) + pow((wp->getX() - index % width), 2) + pow((wp->getY() - index / width), 2), 0.5);
}

//Divides work needed to calculate Worley Noise pixel values over specified number of threads
int threaded_drawWorleyNoise(SDL_Texture *texture, int pixelCount, SDL_PixelFormat *mappingFormat,
                             const std::vector<WPoint> *points, int threadCount)
{
    uint32_t *pixels;
    int pitch;

    //Lock texture so it can be modified
    if (SDL_LockTexture(texture, NULL, (void **)(&pixels), &pitch) != 0)
    {
        fprintf(stderr, "Failed to lock texture. SDL Error : %s\n", SDL_GetError());
        return 1;
    }

    //Divide workload over threads
    int delta = pixelCount / threadCount;
    std::vector<std::thread *> threads;
    std::thread *tmp;
    for (int i = 0; i < pixelCount; i += delta)
    {
        tmp = new std::thread(_threaded_drawWorleyNoise, pixels, mappingFormat, points, i, i + delta);
        threads.push_back(tmp);
    }

    for (int i = 0; i < threadCount; i++)
    {
        (*threads[i]).join();
        delete threads[i];
    }
    threads.clear();

    SDL_UnlockTexture(texture);

    return 0;
}

//Sets pixel values to calculated Worley Noise values over specified range
void _threaded_drawWorleyNoise(uint32_t *pixels, const SDL_PixelFormat *mappingFormat,
                               const std::vector<WPoint> *points, int startIndex, int endIndex)
{
    int numPoints = points->size();
    for (int i = startIndex; i < endIndex; i++)
    {
        std::vector<uint32_t> distances;
        for (int j = 0; j < numPoints; j++)
        {
            int d = distance(i, &((*points)[j]));
            d = (d * 1.0 / SCREEN_WIDTH) * 0xff; //Transform distance into 0-255 range (pixel colors are 1 byte)
            if (d > 0xff)
                d = 0xff;
            distances.push_back(d);
        }

        //Sort distances as Worley Distance requires
        std::sort(distances.begin(), distances.end());
        pixels[i] = SDL_MapRGB(mappingFormat, 0xff - distances[0], 0xff - distances[1], 0xff - distances[1]); //Change distances index to get different patterns
        //pixels[i] = SDL_MapRGB(mappingFormat, 0xff - distances[0] * R, 0xff - distances[0] * G, 0xff - distances[0] * B); //Change distances index to get different patterns
        //pixels[i] = SDL_MapRGB(mappingFormat, distances[0] * R, distances[0] * G, distances[0] * B); //Change distances index to get different patterns
    }
}