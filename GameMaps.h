#pragma once
#include <map>
#include <string>
#include "raylib.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 800;

struct GameMap {
    std::vector<Rectangle> obstacles;
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
    { "Obstacles", 
        { 
            {
                {0, 0, (float)SCREENWIDTH, 10}, 
                {0, (float)SCREENHEIGHT - 10, (float)SCREENWIDTH, 10}, 
                {0, 0, 10, (float)SCREENHEIGHT}, 
                {(float)SCREENWIDTH - 10, 0, 10, (float)SCREENHEIGHT},
                {100, 100, 600, 20}, // Top bar
                {300, 300, 20, 300}  // Middle pillar
            }
        }
    }
};