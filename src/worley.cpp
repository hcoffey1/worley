#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>

uint32_t FPS_LIMIT = 60;
int32_t SCREEN_WIDTH = 400;
int32_t SCREEN_HEIGHT = 400;

SDL_Window *WINDOW = nullptr;

int distance(int a, int b, int z, int w);
int drawWorleyNoise(SDL_Texture *texture, int pixelCount, int pixelWidth, SDL_PixelFormat *mappingFormat,
                     const std::vector<std::pair<uint32_t, uint32_t>> * points);

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
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

    SDL_Texture *texture = SDL_CreateTexture(rend, SDL_GetWindowPixelFormat(WINDOW),
                                             SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(texture == NULL)
    {
        fprintf(stderr, "Failed to create blank texture. SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PixelFormat * mappingFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(WINDOW));

    uint32_t *pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, (void **)(&pixels), &pitch) != 0)
    {
        fprintf(stderr, "Failed to lock texture. SDL Error : %s\n", SDL_GetError());
        return 1;
    }
    int pixelWidth = (pitch/4);
    int pixelCount = pixelWidth * SCREEN_HEIGHT;

    int numPoints = 3;
    int depth = SCREEN_WIDTH;
    std::vector<std::pair<uint32_t, uint32_t>> points;

    for (int i = 0; i < numPoints; i++)
    {
        points.push_back(std::pair<uint32_t, uint32_t>(rand() % pixelCount, rand() % depth));
    }

    SDL_UnlockTexture(texture);

    bool quit = false;
    std::chrono::_V2::system_clock::time_point time_start, time_end;
    std::chrono::duration<double> time_elapsed;
    while (!quit)
    {
        time_start = std::chrono::high_resolution_clock::now();

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

        drawWorleyNoise(texture, pixelCount, pixelWidth, mappingFormat, &points);

        SDL_Rect renderRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(rend, texture, NULL, &renderRect);
        SDL_RenderPresent(rend);

        for (int i = 0; i < numPoints; i++)
        {
            points[i].first = (points[i].first + 1) % pixelCount;
            points[i].second = (points[i].second + 1) % depth;
        }

        //FPS Limiter
        time_end = std::chrono::high_resolution_clock::now();
        time_elapsed = time_end - time_start;
        float _timeLeft = time_elapsed.count();
        _timeLeft = (1.0 / FPS_LIMIT) - _timeLeft;
        if (_timeLeft < 0)
            _timeLeft = 0;
        SDL_Delay(_timeLeft);
    }

    SDL_FreeFormat(mappingFormat);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();

    return 0;
}

int distance(int a, int b, int z, int w)
{
    return pow(pow(z, 2) + pow((b % w - a % w), 2) + pow((b / w - a / w), 2), 0.5);
}

int drawWorleyNoise(SDL_Texture *texture, int pixelCount, int pixelWidth, SDL_PixelFormat *mappingFormat,
                     const std::vector<std::pair<uint32_t, uint32_t>> * points)
{
    uint32_t *pixels;
    int pitch;
    int numPoints = points->size();
    if (SDL_LockTexture(texture, NULL, (void **)(&pixels), &pitch) != 0)
    {
        fprintf(stderr, "Failed to lock texture. SDL Error : %s\n", SDL_GetError());
        return 1;
    }

    for (int i = 0; i < pixelCount; i++)
    {
        std::vector<uint32_t> distances;
        for (int j = 0; j < numPoints; j++)
        {
            int d = distance(i, (*points)[j].first, (*points)[j].second, pixelWidth);
            d = (d * 1.0 / SCREEN_WIDTH) * 0xff;
            if (d > 0xff)
                d = 0xff;
            distances.push_back(d);
        }
        std::sort(distances.begin(), distances.end());

        pixels[i] = SDL_MapRGB(mappingFormat, distances[0], distances[0], distances[0]);
    }

    SDL_UnlockTexture(texture);

    return 0;
}