
#include "raylib.h"

/*
Note:
X coord goes from left to right, counting up
Y coord goes from top to bottom, counting up

Added multiple axes of different sizes
*/

//helper functions
//int UpdateCircleLeftEdge(int cx, int cs);
//int UpdateCircleRightEdge(int cx, int cs);
//int UpdateCircleTopEdge(int cy, int cs);
//int UpdateCircleBotEdge(int cy, int cs);
//int UpdateRectLeftEdge(int rx);
//int UpdateRectRightEdge(int rx, int rsx);
//int UpdateRectTopEdge(int ry);
//int UpdateRectBotEdge(int ry, int rsy);

int main()
{
    // size for window
    int sizeX = 600; int sizeY = 450;
    //circle info (circle drawn from its midpoint)
    float circleSize = 25.0;
    int circleX = sizeX/5; int circleY = sizeY/2; //position    
    int circleLeftEdge{circleX-int(circleSize)};
    int circleRightEdge{circleX+int(circleSize)};
    int circleTopEdge{circleY-int(circleSize)};
    int circleBotEdge{circleY+int(circleSize)};
    // rect info (the axe, drawn from top left corner), {} is called braced initialization in this instance
    int rectSizeX{50}; int rectSizeY{50};
    int rectX{sizeX/2}; int rectY{0}; //position
    int rectLeftEdge{rectX};
    int rectRightEdge{rectX+rectSizeX};
    int rectTopEdge{rectY};
    int rectBotEdge{rectY+rectSizeY};
    int direction{10}; //for moving
    Color axeColor{120, 190, 190, 255};
    bool bCollisionWithAxe{false};
    int gameOverFontSize = 40;

    
    

    InitWindow(sizeX, sizeY, "axe game");
    SetTargetFPS(60);

    
    while (!WindowShouldClose())
    {
        // draw frame
        BeginDrawing();

        ClearBackground(BLUE);

        

        if(bCollisionWithAxe)
        {
            DrawText("GAME OVA", sizeX/2 - gameOverFontSize*3, sizeY/2 - gameOverFontSize, gameOverFontSize, BLACK);
        }
        else
        {
            // game logic here
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

            //update collision positions
            circleLeftEdge = circleX - circleSize;
            circleRightEdge = circleX + circleSize;
            circleTopEdge = circleY - circleSize;
            circleBotEdge = circleY + circleSize;            
            rectLeftEdge = rectX;
            rectRightEdge = rectX + rectSizeX;
            rectTopEdge = rectY;
            rectBotEdge = rectY + rectSizeY;

            //check for collisions only when circle can potentially overlap with axe vertically
            if(circleRightEdge > rectLeftEdge && circleLeftEdge < rectRightEdge)
            {
                if(circleTopEdge < rectBotEdge && circleBotEdge > rectTopEdge)
                {
                    bCollisionWithAxe = true;
                }
            }
        }

        

        //remove frame
        EndDrawing();
    }
}

/*
int UpdateCircleLeftEdge(int cx, int cs) { return cx - cs; }
int UpdateCircleRightEdge(int cx, int cs) { return cx + cs; }
int UpdateCircleTopEdge(int cy, int cs) { return cy - cs; }
int UpdateCircleBotEdge(int cy, int cs) { return cy + cs; }

int UpdateRectLeftEdge(int rx) { return rx; }
int UpdateRectRightEdge(int rx, int rsx) { return rx + rsx; }
int UpdateRectTopEdge(int ry) { return ry; }
int UpdateRectBotEdge(int ry, int rsy) { return ry + rsy; }
*/

/*
//basically what the above helper functions do
void UpdateCircleCollisions(int cx, int cy, int size)
{
    circleLeftEdge = circleX - circleSize;
    circleRightEdge = circleX + circleSize;
    circleTopEdge = circleY - circleSize;
    circleBotEdge = circleY + circleSize;
}
void UpdateRectCollisions(int rx, int ry, int rsx, int rsy)
{
    rectLeftEdge = rectX;
    rectRightEdge = rectX + rectSizeX;
    rectTopEdge = rectY;
    rectBotEdge = rectY + rectSizeY;
}
*/