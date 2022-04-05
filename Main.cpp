
#include "SDL.h"

#include "Utility.hpp"

#include <iostream>
#include <random>

#define STATES_AMOUNT 4

static constexpr int HEIGHT = 30;
static constexpr int WIDTH = 15;
static constexpr int block_render_size = 15;
static constexpr int FPS = 60;

bool isValidMove(bool field[HEIGHT][WIDTH], int x, int y, int figure)
{
    for (int i = 0; i < 16; ++i)
    {
        bool cell_empty = !(figure & (0b1 << i));
        if (cell_empty) continue;

        if (x + i%4 < 0 || y + i/4 < 0 || x + i%4 >= WIDTH || y + i/4 >= HEIGHT 
        || field[y + i/4][x + i%4] == true) 
        {
            return false;
        }
    }
    return true;
}

void cap_fps(float deltaTime, uint32_t fps)
{
    uint32_t dt = static_cast<uint32_t>(deltaTime);
    if (1000/fps > dt)
        SDL_Delay((1000/fps) - dt);
}

int main()
{
    // INIT FIGURES
    auto tetFigures = createTetrisFigures();
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<std::mt19937_64::result_type> randFunc(0, tetFigures.size() - 1);

    bool keys[4] = {false};

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 
    || SDL_CreateWindowAndRenderer(WIDTH * block_render_size, 
    HEIGHT * block_render_size, SDL_WINDOW_SHOWN, &window, &renderer) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    bool gameOver = false;
    int figId = randFunc(rng);
    int figX = WIDTH / 2 - 2;
    int figY = 0;
    int state = 0;
    bool field[HEIGHT][WIDTH] = {false};
    int fpsCnt = 0;

    while (!gameOver)
    {
        int curFigureState = tetFigures[figId][state];
        int tic = SDL_GetTicks();
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    gameOver = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        keys[Key_Left] = true;
                        break;
                    case SDLK_RIGHT:
                        keys[Key_Right] = true;
                        break;
                    case SDLK_DOWN:
                        keys[Key_Down] = true;
                        break;
                    case SDLK_SPACE:
                        keys[Key_Space] = true;
                        break;
                    }
                    break;
            }
        }

        // Process Keys
        if (keys[Key_Left])
        {
            if (isValidMove(field, figX - 1, figY, curFigureState))
                figX--;
            keys[Key_Left] = false;
        }
        if (keys[Key_Right])
        {
            if (isValidMove(field, figX + 1, figY, curFigureState))
                figX++;
            keys[Key_Right] = false;
        }
        if (keys[Key_Down])
        {
            if (isValidMove(field, figX, figY + 1, curFigureState))
                figY++;
            keys[Key_Down] = false;
        }
        if (keys[Key_Space])
        {
            int newState = (state + 1) % STATES_AMOUNT;
            if (isValidMove(field, figX, figY, tetFigures[figId][newState]))
            {
                state = newState;
                curFigureState = tetFigures[figId][state];
            }
            keys[Key_Space] = false;
        }

        if (fpsCnt >= FPS)
        {
            fpsCnt = fpsCnt - FPS;
            // Move tetfigure one block down, up to the ground
            if (figY + 1 < HEIGHT && isValidMove(field, figX, figY + 1, curFigureState))
            {
                figY += 1;
            }
            else 
            { // Our tetFigure being placed
                for (int i = 0; i < 16; ++i)
                {
                    bool cell_not_empty = (curFigureState & (0b1 << i));
                    if (cell_not_empty)
                    {
                        field[figY + i/4][figX + i%4] = true;
                    }
                }
                // Check for 4 lines from our placed piece
                for (int difY = 0; difY < 4; ++difY)
                {
                    if (figY + difY < HEIGHT)
                    {
                        bool lineComplete = true;
                        for (int w = 0; w < WIDTH; ++w)
                        {
                            // if cell empty
                            if (field[figY + difY][w] == false)
                            {
                                lineComplete = false;
                                break;
                            }
                        }
                        if (lineComplete)
                        {
                            for (int h = figY + difY; h > 0; --h)
                            {
                                for (int w = 0; w < WIDTH; ++w)
                                {
                                    field[h][w] = field[h - 1][w];
                                }
                                for (int w = 0; w < WIDTH; ++w)
                                    field[0][w] = false;
                            }
                        }
                    }
                }

                // New TetFigure
                figId = randFunc(rng);
                figY = 0;
                figX = WIDTH / 2 - 2;
                state = 0;

                if (!isValidMove(field, figX, figY, curFigureState))
                    gameOver = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        for (int i = 0; i < 16; ++i)
        {
            bool cell_not_empty = (curFigureState & (0b1 << i));
            // Convert the coordinates
            int ty = figY * block_render_size;
            int tx = figX * block_render_size; 

            if (cell_not_empty)
            {
                SDL_Rect r{tx + block_render_size * (i%4), ty + (i/4) * block_render_size,
                block_render_size, block_render_size};
                SDL_RenderFillRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
                SDL_RenderDrawRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            }
        }

        for (int h = 0; h < HEIGHT; ++h)
        for (int w = 0; w < WIDTH; ++w)
        {
            if (field[h][w] == true)
            {
                SDL_Rect r{w * block_render_size, h * block_render_size, 
                block_render_size, block_render_size};
                SDL_RenderFillRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
                SDL_RenderDrawRect(renderer, &r);
                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            }
        }

        SDL_RenderPresent(renderer);
        fpsCnt++;
        cap_fps(SDL_GetTicks() - tic, FPS);

    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
