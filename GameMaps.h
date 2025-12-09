#pragma once
#include <map>
#include <string>
#include "raylib.h"

const int SCREENWIDTH = 1200;
const int SCREENHEIGHT = 1200;

struct GameMap {
    std::vector<Rectangle> obstacles;
    std::vector<Rectangle> winConditions;
};

inline std::map<std::string, GameMap> gameMaps = {
    { "Box", 
        {
            {
                {0, 0, (float)SCREENWIDTH, 10}, 
                {0, (float)SCREENHEIGHT - 10, (float)SCREENWIDTH, 10}, 
                {0, 0, 10, (float)SCREENHEIGHT}, 
                {(float)SCREENWIDTH - 10, 0, 10, (float)SCREENHEIGHT} 
            }
        }
    },
    { "Race", 
        { 
            {
                // borders 
                {0, 0, (float)SCREENWIDTH, 10}, 
                {0, (float)SCREENHEIGHT - 10, (float)SCREENWIDTH, 10}, 
                {0, 0, 10, (float)SCREENHEIGHT}, 
                {(float)SCREENWIDTH - 10, 0, 10, (float)SCREENHEIGHT},
                // x, y, width, height
                //{100, 250, 600, 20}, // Top bar
                {200, 0, 20, 550},
                {200, 650, 20, 550}
            },
            {
                {300, 200, 50, 50}
            }
        }
    }
};