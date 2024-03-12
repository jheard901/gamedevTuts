
#include "raylib.h"

int main()
{
    // {} is called braced initialization in this instance
    float rootBeer = 1.99; double cheeseBurger{5.99}; bool bShouldHaveLunch{};
    
    // size for window
    int sizeX = 350; int sizeY = 200;
    
    InitWindow(sizeX, sizeY, "axe game");

    // game logic here
    while (!WindowShouldClose())
    {
        // draw frame
        BeginDrawing();

        ClearBackground(BLUE);

        //remove fram
        EndDrawing();
    }
}