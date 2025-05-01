#include "SDL2/SDL.h"
#include "defs.h"
#include "object.h"
#include <algorithm>
#include <mutex>
#include <stdio.h>
#include <thread>
#include <vector>

using namespace std;

int main() 
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
        HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *s = SDL_CreateRenderer(
        window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event event;

    int xMouse = 1;
    int yMouse = 1;

    Object c1 = Object(s);
    Object sun = Object(s);

    vector<int> args_oc = {15};

    vector<Object> planets; 

    c1.set_coords(800, 100);
    c1.set_color(0x00, 0x00, 0xFF, 0xFF);
    c1.set_mass(4);

    planets.push_back(c1);

    c1.set_coords(800, 300);
    c1.set_color(0x00, 0xFF, 0x00, 0xFF);
    c1.set_mass(20);

    planets.push_back(c1);
    

    sun.set_coords(WIDTH / 2, HEIGHT / 2);
    sun.set_color(0xFF, 0x00, 0x00, 0xFF);
    sun.set_mass(10000);

    planets[0].vx = 10;
    planets[0].vy = 5;

    planets[1].vx = 10;
    planets[1].vy = 5;

    while (running)
    {
        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) running = false;
        SDL_RenderClear(s);

        for(int i = 0;i < planets.size();i++)
        {
            pair<float, float> axy = planets[i].calculate_gravity(sun);
            for(int j = 0;j < planets.size();j++)
            {
                if(i == j) continue;
                axy.first += planets[i].calculate_gravity(planets[j]).first;
                axy.second += planets[i].calculate_gravity(planets[j]).second;
            }
            planets[i].vx += axy.first;
            planets[i].vy += axy.second;
        }

        for(int i = 0;i < planets.size();i++) 
        {
            planets[i].x += planets[i].vx;
            planets[i].y += planets[i].vy;
        }

        for(Object o: planets) o.render("circle", args_oc);
        sun.render("circle", args_oc);

        SDL_RenderPresent(s);
        SDL_RenderClear(s);
    }  
    
    SDL_DestroyRenderer(s);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
