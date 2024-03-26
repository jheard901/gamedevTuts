
#include "raylib.h"

/*
Note:
X coord goes from left to right, counting up
Y coord goes from top to bottom, counting up

Added multiple axes of different sizes
Can retry on losing
Needs a win condition, like maybe after x time passes, then you win.

I think I can now see the end in sight for how to conclude working on this:

-Add a victory screen after x time passes: player can try again, or exit program
-Reset timer on game over and logic to reset other stuff too
-Add some music while playing
X-change color of player to some kind of bright silver
-add phases to game e.g. every 10 seconds the pattern of the axes change; add a max of 10 axes
-each phase change axe size, direction; maybe color too to differentiate the type of axe
X-axes should move from right to left, and return back to right side after reaching -x value
-make last phase like the scene from resident evil in the laser hallway
-maybe I troll and player has to cheat to win, meaning 
    you have to go above the visible portion of the window
    using up on the numpad i.e. 2, you can go down using 8.
    Maybe leave a tiny space for the player to squeeze against
    top of window and limit player from going completely off
    screen using 2 or 8; WS and the arrow keys will not be used.
    Would be cool if there was some kind of way to clue someone in
    to figuring out to use the numpad, like if we say "use directional
    input to move" in the rules without specifying WASD, or the arrows.
*/

//a timer (from FAQ: https://github.com/raysan5/raylib/wiki/Frequently-Asked-Questions#how-do-i-make-a-timer)
typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
    double currentTime;
} Timer;
void StartTimer(Timer *timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
    timer->currentTime = 0;
}
void UpdateTimer(Timer *timer)
{
    // for showing time incrementing
    if(timer != nullptr)
    {
        timer->currentTime += GetFrameTime();
    }

    // the timer (decreases time based off SetTargetFPS(), I think)
    if(timer != nullptr && timer->lifeTime > 0)
    {
        timer->lifeTime -= GetFrameTime();
    }
}
double GetElapsed(Timer timer)
{
    return GetTime() - timer.startTime;
}
bool IsTimerDone(Timer timer)
{
    return timer.lifeTime <= 0;
}
bool IsTimerDoneOld(Timer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}

//enum class tips from: https://stackoverflow.com/questions/12183008/how-to-use-enums-in-c
enum class Phase { ONE, TWO, THREE, FOUR };

//the rectangles, but as a class
class Axe
{
protected:
    int axeSizeX, axeSizeY; //size
    double axeX; int axeY; //position
    int axeDirection; //for up/down movement
    float axeSpeed; //for left/right movement
    int isx, isy, ix, iy, id; float is; //initial values    
    int axeLeftEdge, axeRightEdge, axeTopEdge, axeBotEdge; //collision edges
    bool bOffscreen;
public:
    Axe()
    {
        isx = 30; isy = 50; ix = 0; iy = 0; id = 10; is = -0.6;
        axeSizeX = isx;
        axeSizeY = isy;
        axeX = ix;
        axeY = iy;
        axeDirection = id;
        axeSpeed = is;
        axeLeftEdge = axeX;
        axeRightEdge = axeX + axeSizeX;
        axeTopEdge = axeY;
        axeBotEdge = axeY + axeSizeY;
        bOffscreen = true;
    }
    Axe(int sizeX, int sizeY, int posX, int posY, int dir, float spd)
    {
        isx = sizeX; isy = sizeY; ix = posX; iy = posY; id = dir; is = spd;
        axeSizeX = isx;
        axeSizeY = isy;
        axeX = ix;
        axeY = iy;
        axeDirection = id;
        axeSpeed = is;
        axeLeftEdge = axeX;
        axeRightEdge = axeX + axeSizeX;
        axeTopEdge = axeY;
        axeBotEdge = axeY + axeSizeY;
        bOffscreen = true;
    }
    double GetAxeX() { return axeX; }
    int GetAxeY() { return axeY; }
    int GetAxeSizeX() { return axeSizeX; }
    int GetAxeSizeY() { return axeSizeY; }
    int GetAxeLeftEdge() { return axeLeftEdge; }
    int GetAxeRightEdge() { return axeRightEdge; }
    int GetAxeTopEdge() { return axeTopEdge; }
    int GetAxeBotEdge() { return axeBotEdge; }
    int GetAxeDirection() { return axeDirection; }
    float GetAxeSpeed() { return axeSpeed; }
    bool GetAxeOffscreen() { return bOffscreen; }    
    void SetAxeDirection(int newDirection) { axeDirection = newDirection; }
    void SetAxeSpeed(float newSpeed) { axeSpeed = newSpeed; }
    void SetAxeOffscreen(bool val) { bOffscreen = val; }
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
    bool IsOffscreen(int bounds)
    {
        if (axeX < bounds)
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
        axeX += axeSpeed;
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
    void ResetPos() { axeX = ix; axeY = iy; }
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
    Color playerColor{200, 210, 190, 255};

    // axes info | 1-5 start at top of screen, 6-10 start at bottom
    int AXE_SIZE = 60; //vertical size
    int MAX_AXE_WIDTH = 120, MIN_AXE_WIDTH = 4; //horizontal size
    float MAX_AXE_SPEED = -0.3, MIN_AXE_SPEED = -0.12; //-1.0 is too fast, -0.08 feels too slow
    int MAX_AXE_DIRECTION = 10;
    Axe axe1(MAX_AXE_WIDTH-100, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH), 0, 0, 0);
    Axe axe2(MAX_AXE_WIDTH-80, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*2), 0, 0, 0);
    Axe axe3(MIN_AXE_WIDTH-60, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*3), 0, 0, 0);
    Axe axe4(MIN_AXE_WIDTH-40, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*4), 0, 0, 0);
    Axe axe5(MIN_AXE_WIDTH-20, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*5), 0, 0, 0);
    Axe axe6(MIN_AXE_WIDTH-100, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH), sizeY, 0, 0);
    Axe axe7(MIN_AXE_WIDTH-80, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*2), sizeY, 0, 0);
    Axe axe8(MIN_AXE_WIDTH-60, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*3), sizeY, 0, 0);
    Axe axe9(MIN_AXE_WIDTH-40, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*4), sizeY, 0, 0);
    Axe axe10(MIN_AXE_WIDTH-20, AXE_SIZE, int(sizeX+MIN_AXE_WIDTH*5), sizeY, 0, 0);
    Color axeColor{150, 190, 130, 255};

    // game vars | {} is called braced initialization
    Timer gameTime;    
    Phase gamePhase = Phase::ONE;
    bool bCollisionWithAxe{false};
    bool bStartNextPhase{true};
    int gameOverFontSize = 40;

    InitWindow(sizeX, sizeY, "me axe game");
    SetTargetFPS(60);
    StartTimer(&gameTime, 60.0);

    while (!WindowShouldClose())
    {
        // draw frame
        BeginDrawing();

        ClearBackground(BLUE);

        
        if(IsTimerDone(gameTime))
        {
            //the player has won!
            DrawText("DEBUG: TIMER DONE", sizeX/2 - gameOverFontSize*3, sizeY/2 - gameOverFontSize, gameOverFontSize, BLACK);

            //do something, show victory screen            
        }
        else
        {
                       


            //game proceeds until timer done
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


                //so, just realized the function TextFormat exists which functions like sprintf
                //refer to video for timer usage: https://www.youtube.com/watch?v=vGlvTWUctTQ

                //this shows time incrementing            
                //DrawText(TextFormat("%d", int(gameTime.currentTime)), int(sizeX*0.75), int(sizeY*0.9), gameOverFontSize/2, BLACK);

                UpdateTimer(&gameTime);
                DrawText(TextFormat("%d", int(gameTime.lifeTime)), int(sizeX*0.475), int(sizeY*0.045), gameOverFontSize, BLACK);



                //the player
                DrawCircle(circleX, circleY, circleSize, playerColor);

                //the axes
                DrawRectangle(axe1.GetAxeX(), axe1.GetAxeY(), axe1.GetAxeSizeX(), axe1.GetAxeSizeY(), axeColor);
                DrawRectangle(axe2.GetAxeX(), axe2.GetAxeY(), axe2.GetAxeSizeX(), axe2.GetAxeSizeY(), axeColor);
                DrawRectangle(axe3.GetAxeX(), axe3.GetAxeY(), axe3.GetAxeSizeX(), axe3.GetAxeSizeY(), axeColor);


                //check phase based off timer.lifeTime                
                if(gameTime.lifeTime < 45.0 && gamePhase == Phase::ONE)
                {
                    //update axes with new phase data
                    bStartNextPhase = true;

                    gamePhase = Phase::TWO;
                }
                else if (gameTime.lifeTime < 30.0 && gamePhase == Phase::TWO)
                {
                    gamePhase = Phase::THREE;
                }
                else if (gameTime.lifeTime < 15.0 && gamePhase == Phase::THREE)
                {
                    gamePhase = Phase::FOUR;
                }
                //may need additional tweaking to times used to properly get the look for last phase ready


                //check bOffscreen for axes to tell if they are offscreen
                //when true then if the phase has changed then it can be updated
                //once it return to the right side of the screen

                //need a horizontal direction now for moving them and the checks
                //for when they hit boundaries

                //the -x position could be based off a variable in main called
                //MAX_AXE_SIZE and if axe goes past the MAX size then it can be called
                //back to the right side 



                //check if phase timer has shifted, if so, then
                //once axe is offscreen it can be updated to match
                //size/speed for its next phase

                if(axe1.GetAxeOffscreen())
                {
                    //set size, direction, and speed for each phase
                    if(bStartNextPhase)
                    {
                        switch (gamePhase)
                        {
                            case Phase::ONE:
                                //do stuff
                                axe1.SetAxeDirection(MAX_AXE_DIRECTION*0.4);
                                axe1.SetAxeSpeed(MAX_AXE_SPEED*4);
                                bStartNextPhase = false;
                                break;
                            case Phase::TWO:
                                //do stuff
                                bStartNextPhase = false;
                                break;
                            case Phase::THREE:
                                //do stuff
                                bStartNextPhase = false;
                                break;
                            case Phase::FOUR:
                                //do stuff
                                bStartNextPhase = false;
                                break;
                            default:
                                //do stuff
                                break;
                        }   
                    }
                    //should prob reset PosY also, then set size, speed, and direction
                    //based off the phase timer
                    axe1.ResetPos();                    
                    axe1.SetAxeOffscreen(false);
                }
                
                //move the axes
                axe1.UpdatePosition();
                axe2.UpdatePosition();
                axe3.UpdatePosition();

                //determine axe directon and speed
                if(axe1.IsOutofBounds(sizeY, 0))
                {
                    axe1.SetAxeDirection(-axe1.GetAxeDirection());
                }
                if(axe1.IsOffscreen(-axe1.GetAxeSizeX()))
                {
                    axe1.SetAxeOffscreen(true);
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

                //the player
                circleLeftEdge = circleX - circleSize;
                circleRightEdge = circleX + circleSize;
                circleTopEdge = circleY - circleSize;
                circleBotEdge = circleY + circleSize;

                //the axes
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
        }
        

        

        //remove frame
        EndDrawing();
    }

}
