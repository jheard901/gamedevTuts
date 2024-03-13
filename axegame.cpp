
#include "raylib.h"

/*
Note:
X coord goes from left to right, counting up
Y coord goes from top to bottom, counting up
*/

int main()
{
    // size for window
    int sizeX = 600; int sizeY = 450;
    //circle info (circle drawn from its midpoint)
    float circleSize = 25.0;
    int circleX = sizeX/5; int circleY = sizeY/2; //position    
    int circleLeftEdge{circleX-circleSize};
    int circleRightEdge{circleX+circleSize};
    int circleTopEdge{circleY-circleSize};
    int circleBotEdge{circleY+circleSize};
    // rect info (the axe, drawn from top left corner), {} is called braced initialization in this instance
    int rectSizeX{50}; int rectSizeY{50};
    int rectX{sizeX/2}; int rectY{0}; //position
    int rectLeftEdge{rectX};
    int rectRightEdge{rectX+rectSizeX};
    int rectTopEdge{rectY};
    int rectBotEdge{rectY+rectSizeY};
    int direction{10}; //for moving
    Color axeColor{120, 190, 190, 255};

    InitWindow(sizeX, sizeY, "axe game");
    SetTargetFPS(60);

    // game logic here
    while (!WindowShouldClose())
    {
        // draw frame
        BeginDrawing();

        ClearBackground(BLUE);

        DrawCircle(circleX, circleY, circleSize, WHITE);
        DrawRectangle(rectX, rectY, rectSizeX, rectSizeY, axeColor);

        //move the axe
        rectY += direction;
        //determine directon
        if(rectY > sizeY || rectY < 0)
        {
            direction = -direction; //'-' in front inverses the number
        }
        
        

        //input to move circle and keep in window
        if(IsKeyDown(KEY_D) && circleX < sizeX)
        {
            circleX += 5;
        }
        if(IsKeyDown(KEY_A) && circleX > 0)
        {
            circleX -= 5;
        }

        //remove frame
        EndDrawing();
    }
}