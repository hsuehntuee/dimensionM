#include "button.hpp"
// #include "player.hpp"
// include "colli.hpp"
#include "fodder.hpp"
#include "text.hpp"
#include "weapon.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main()
{
    Uint64 startTime = SDL_GetPerformanceCounter();

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *g_window = NULL;
    SDL_Renderer *g_renderer = NULL;

    g_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

    if (g_renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        // Initialize renderer color
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        }
    }
    vec v[4] = {{30, 30}, {30, 40}, {40, 40}, {40, 30}};

    SDL_Event e;
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    bool quit = 0;

    Polygon p(v, 4);

    // 宣告區
    // Entity ent1(g_renderer, "../images/brown2.png", 100, 100, 100, 100);
    // Solid sol1(g_renderer, "../images/anime.png", 200, 200, 200, 200, &e);

    std::vector<Solid *> obst;
    obst.push_back(new Solid(g_renderer, "../images/brown2.png", 200, 300, 60, 30, &e, p));
    Player ply1(g_renderer, "../images/brown2.png", 30, 300, 100, 100, &e, 5,
                40); // 不知為何玩家的第一張圖（靜止）傳不進去，請幫我處理一下
    ply1.set_source("../images/2.png", "../images/3.png", "../images/4.png");
    // Fodder fd1(g_renderer, POLY, 30, 30, 100, 100);
    std::vector<Fodder *> fods;
    // fods.push_back(&fd1);
    /*
        vec ff[4] = {{100, 100}, {100, 200}, {200, 200}, {200, 100}};
        Polygon g(ff, 4);
        Fodder *aa = new Fodder(g_renderer, EXPO, 500, 40, 100, 60, g);
        fods.push_back(aa);*/

    // fods.push_back(new Fodder(g_renderer, LOGA, 500, 300, 100, 50));
    std::vector<Weapon *> wps;
    // Mob mb1(g_renderer, "../images/anime.png", 400, 300, 200, 100, &e, 5);
    wps.push_back(new Weapon(g_renderer, 600, 400, DE_FOURIER));
    wps.push_back(new Weapon(g_renderer, 300, 100, INTEGRATION));

    // 宣告區

    while (!quit)
    {
        // Handle events on queue
        SDL_PollEvent(&e);
        // User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }

        key_state = SDL_GetKeyboardState(NULL);
        SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_renderer);
        Uint64 end = SDL_GetPerformanceCounter();

        // 寫程式區
        for (auto i : obst)
        {
            i->display();
        }
        // mb1.display();
        ply1.handle_event(key_state, &e);
        ply1.display();
        SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
        /*drawPolygon(g_renderer, aa->_poly);*/
        ply1.detect(wps, fods, obst); // 這會遍歷所有上述三者的vector檢查有無碰撞

        for (auto i : fods)
        {
            i->display(ply1, obst);
        }

        for (auto i : wps)
        {
            i->display();
        }

        // 寫程式區

        SDL_RenderPresent(g_renderer);
        double elapse = (double)(end - startTime) / SDL_GetPerformanceFrequency();

        // Render texture to screen
    }

    for (auto i : obst)
    {
        i->~Solid();
        delete i;
    }

    for (auto i : wps)
    {
        i->~Weapon();
        delete i;
    }
}
