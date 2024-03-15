
#include "raylib.h"

#include <sstream>

/*
Note:
X coord goes from left to right, counting up
Y coord goes from top to bottom, counting up

Added multiple axes of different sizes
Can retry on losing
Needs a win condition, like maybe after x time passes, then you win.
*/

//a timer (from FAQ: https://github.com/raysan5/raylib/wiki/Frequently-Asked-Questions#how-do-i-make-a-timer)
typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
} Timer;
void StartTimer(Timer *timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}
double GetElapsed(Timer timer)
{
    return GetTime() - timer.startTime;
}
bool IsTimerDone(Timer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}

//the rectangles, but as a class
class Axe
{
protected:
    int axeSizeX, axeSizeY; //size
    int axeX, axeY; //position
    int axeDirection; //for movement
    int isx, isy, ix, iy, id; //initial values
    int axeLeftEdge, axeRightEdge, axeTopEdge, axeBotEdge; //collision edges
public:
    Axe()
    {
        isx = 30; isy = 50; ix = 0; iy = 0; id = 10;
        axeSizeX = isx;
        axeSizeY = isy;
        axeX = ix;
        axeY = iy;
        axeDirection = id;
        axeLeftEdge = axeX;
        axeRightEdge = axeX + axeSizeX;
        axeTopEdge = axeY;
        axeBotEdge = axeY + axeSizeY;
    }
    Axe(int sizeX, int sizeY, int posX, int posY, int dir)
    {
        isx = sizeX; isy = sizeY; ix = posX; iy = posY; id = dir;
        axeSizeX = isx;
        axeSizeY = isy;
        axeX = ix;
        axeY = iy;
        axeDirection = id;
        axeLeftEdge = axeX;
        axeRightEdge = axeX + axeSizeX;
        axeTopEdge = axeY;
        axeBotEdge = axeY + axeSizeY;
    }
    int GetAxeX() { return axeX; }
    int GetAxeY() { return axeY; }
    int GetAxeSizeX() { return axeSizeX; }
    int GetAxeSizeY() { return axeSizeY; }
    int GetAxeLeftEdge() { return axeLeftEdge; }
    int GetAxeRightEdge() { return axeRightEdge; }
    int GetAxeTopEdge() { return axeTopEdge; }
    int GetAxeBotEdge() { return axeBotEdge; }
    int GetAxeDirection() { return axeDirection; }
    void SetAxeDirection(int newDirection) { axeDirection = newDirection; }
    bool IsOutofBounds(int upperBounds, int lowerBounds)
    {
        if(axeY > upperBounds || axeY < lowerBounds)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void UpdatePosition()
    {
        axeY += axeDirection;
    }
    void UpdateCollisions()
    {
        axeLeftEdge = axeX;
        axeRightEdge = axeX + axeSizeX;
        axeTopEdge = axeY;
        axeBotEdge = axeY + axeSizeY;
    }
    void Reset()
    {
        axeSizeX = isx;
        axeSizeY = isy;
        axeX = ix;
        axeY = iy;
        axeDirection = id;
    }
};

int main()
{
    // size for window
    int sizeX = 690; int sizeY = 420;
    // circle/player info (circle drawn from its midpoint)
    float circleSize = 25.0;
    int circleInitialX = int(sizeX*0.1); int circleInitialY = int(sizeY*0.5); //initial position
    int circleX = circleInitialX; int circleY = circleInitialY; //current position
    int circleLeftEdge{circleX-int(circleSize)};
    int circleRightEdge{circleX+int(circleSize)};
    int circleTopEdge{circleY-int(circleSize)};
    int circleBotEdge{circleY+int(circleSize)};
    // axes info
    Axe axe1(30, 50, int(sizeX*0.8), 0, 10);
    Axe axe2(100, 60, int(sizeX*0.25), 0, 6);
    Axe axe3(40, 60, int(sizeX*0.6), sizeY-60, -10);
    Color axeColor{120, 190, 190, 255};

    // game vars {} is called braced initialization
    Timer gameTime;
    std::stringstream elapsedTime;
    bool bCollisionWithAxe{false};
    int gameOverFontSize = 40;

    InitWindow(sizeX, sizeY, "me axe game");
    SetTargetFPS(60);
    StartTimer(&gameTime, 0);

    while (!WindowShouldClose())
    {
        // draw frame
        BeginDrawing();

        ClearBackground(BLUE);

        

        if(bCollisionWithAxe)
        {
            // game over
            DrawText("GAME OVA", sizeX/2 - gameOverFontSize*3, sizeY/2 - gameOverFontSize, gameOverFontSize, BLACK);
            DrawText("Press 'Enter' to retry", sizeX/3, int(float(sizeY)*0.75), gameOverFontSize/2, BLACK);
            if(IsKeyDown(KEY_ENTER))
            {
                //reset all object positions
                circleX = circleInitialX; circleY = circleInitialY;
                axe1.Reset();
                axe2.Reset();
                axe3.Reset();
                bCollisionWithAxe = false;
            }
        }
        else
        {
            // game logic here

            //elapsed time | solution from: https://stackoverflow.com/questions/6404586/double-to-const-char

            //so, just realized the function DrawText exists which functions like sprintf
            //refer to video for timer usage: https://www.youtube.com/watch?v=vGlvTWUctTQ
            elapsedTime << GetElapsed(gameTime);
            const char* elapsedTimeStr = elapsedTime.str().c_str();
            DrawText(elapsedTimeStr, int(sizeX*0.75), int(sizeY*0.9), gameOverFontSize/2, BLACK);
            

            //the player
            DrawCircle(circleX, circleY, circleSize, WHITE);

            //the axes
            DrawRectangle(axe1.GetAxeX(), axe1.GetAxeY(), axe1.GetAxeSizeX(), axe1.GetAxeSizeY(), axeColor);
            DrawRectangle(axe2.GetAxeX(), axe2.GetAxeY(), axe2.GetAxeSizeX(), axe2.GetAxeSizeY(), axeColor);
            DrawRectangle(axe3.GetAxeX(), axe3.GetAxeY(), axe3.GetAxeSizeX(), axe3.GetAxeSizeY(), axeColor);

            //move the axes
            axe1.UpdatePosition();
            axe2.UpdatePosition();
            axe3.UpdatePosition();

            //determine axe directons
            if(axe1.IsOutofBounds(sizeY, 0))
            {
                axe1.SetAxeDirection(-axe1.GetAxeDirection());
            }
            if(axe2.IsOutofBounds(sizeY, 0))
            {
                axe2.SetAxeDirection(-axe2.GetAxeDirection());
            }
            if(axe3.IsOutofBounds(sizeY, 0))
            {
                axe3.SetAxeDirection(-axe3.GetAxeDirection());
            }

            //input to move circle and keep in window
            if((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && circleX < sizeX)
            {
                circleX += 5;
            }
            if((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && circleX > 0)
            {
                circleX -= 5;
            }

            //update collision positions
            circleLeftEdge = circleX - circleSize;
            circleRightEdge = circleX + circleSize;
            circleTopEdge = circleY - circleSize;
            circleBotEdge = circleY + circleSize;
            axe1.UpdateCollisions();
            axe2.UpdateCollisions();
            axe3.UpdateCollisions();

            //check for collisions only when circle can potentially overlap with axe vertically
            if(circleRightEdge > axe1.GetAxeLeftEdge() && circleLeftEdge < axe1.GetAxeRightEdge())
            {
                if(circleTopEdge < axe1.GetAxeBotEdge() && circleBotEdge > axe1.GetAxeTopEdge())
                {
                    bCollisionWithAxe = true;
                }
            }

            if(circleRightEdge > axe2.GetAxeLeftEdge() && circleLeftEdge < axe2.GetAxeRightEdge())
            {
                if(circleTopEdge < axe2.GetAxeBotEdge() && circleBotEdge > axe2.GetAxeTopEdge())
                {
                    bCollisionWithAxe = true;
                }
            }

            if(circleRightEdge > axe3.GetAxeLeftEdge() && circleLeftEdge < axe3.GetAxeRightEdge())
            {
                if(circleTopEdge < axe3.GetAxeBotEdge() && circleBotEdge > axe3.GetAxeTopEdge())
                {
                    bCollisionWithAxe = true;
                }
            }
        }

        

        //remove frame
        EndDrawing();
    }

}
