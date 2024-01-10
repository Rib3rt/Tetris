#include <time.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "tetris.h"

//extern declarations
extern int stage[];

//function declarations
int calcPoints(int linesCombo, int currentLevel, int* comboCounter);

//Main function
int main(int argc, char** argv, char** environ)
{
    //inizialize random seed
    time_t unixTime;
    time(&unixTime);
    SetRandomSeed(unixTime);

    //inizialize TETROMINO MOVE DOWN variables
    float moveTetrominoDownTimer = 0.3f;
    const float tetrominoDownTimerMod = 0.02f;
    float timeToMoveTetrominoDown = moveTetrominoDownTimer;
    int gameplayTick = 0;
    int keyDownPressedCount = 0;

    //inizialize Raylib window and framerate
    const int windowWidth = 600; 
    const int windowHeight = 700;
    InitWindow(windowWidth, windowHeight, "Tetris"); 
    SetTargetFPS(60);

    //inizialize stage offset variables
    const int startOffsetX = (windowWidth / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
    const int startOffsetY = (windowHeight / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);

    //inizialize tetromino starting location variables
    const int tetrominoStartX = STAGE_WIDTH / 2;
    const int tetrominoStartY = 0;

    //inizialize 2 tetromini, current and next
    Tetromino currentTetromino = *getNewRandomTetromino();
    currentTetromino.posX = tetrominoStartX;
    currentTetromino.posY = tetrominoStartY;

    Tetromino nextTetromino = *getNewRandomTetromino();
    nextTetromino.posX = (tetrominoStartX - 10);
    nextTetromino.posY = tetrominoStartY;

    //inizialize gameplay variables
    int gameOver = 0;
    int points = 0;
    int lines = 0;
    int level = 1;
    int deletingLines = 0;
    int pointLineCounter = 0;
    int levelForPointsCalc = 0;
    int comboCounter = 0;

    //inizialize Raylib sounds system and sounds
    InitAudioDevice();
    Sound rotateTetraminoSFX = LoadSound("Sounds/Jump199.wav");
    Sound collisionTetraminoSFX = LoadSound("Sounds/Damage184.wav");

    //main loop
    while(!WindowShouldClose())
    {
        if(gameOver == 0)
        {
            timeToMoveTetrominoDown -= GetFrameTime();
            
            //TICK
            gameplayTick++;
            if (gameplayTick == TICK)
            {
                gameplayTick = 0;
                keyDownPressedCount = 0;
                
                if(deletingLines > 0)
                {
                    //T-Spin points check here, 7 is tetromino_T
                    if(currentTetromino.tetrominoID == 7)
                    {
                        //maybe in future
                        printf("%s\n","check T-Spin");
                    }

                    clearLines();
                    PlaySound(collisionTetraminoSFX);

                    //increment line counter for points combo
                    pointLineCounter++;
                    
                    //save current level BEFORE new lines count for points calc
                    levelForPointsCalc = level;

                    //increment delete lines counter
                    lines++;

                    //every 10 lines level up
                    int checkLevelUp = lines % LINES2LEVELUP;
                    if(checkLevelUp==0)
                    {
                        level++;
                        moveTetrominoDownTimer -= tetrominoDownTimerMod;
                        timeToMoveTetrominoDown = moveTetrominoDownTimer;
                    }

                    if(!checkLinesToDelete())
                    {
                        points += calcPoints(pointLineCounter, levelForPointsCalc, &comboCounter);

                        //printf("%d\n",comboCounter);
                        
                        deletingLines = 0;
                        
                        pointLineCounter = 0;

                        currentTetromino = nextTetromino;
                        currentTetromino.posX = tetrominoStartX;
                        currentTetromino.posY = tetrominoStartY;

                        nextTetromino = *getNewRandomTetromino();
                        nextTetromino.posX = (tetrominoStartX - 10);
                        nextTetromino.posY = tetrominoStartY;
                    }
                }
            }

            //Tetromino move logic
            if (IsKeyPressed(KEY_UP) && deletingLines == 0)
            {
                const int lastRotation = currentTetromino.currentRotation;

                currentTetromino.currentRotation++;

                if (currentTetromino.currentRotation > 3)
                {
                    currentTetromino.currentRotation = 0;
                }

                //if collision after tetromino rotation go back to last rotation else make rotation and play a sound
                if (CheckCollision(currentTetromino.posX, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.currentRotation = lastRotation;
                }
                else
                {
                    PlaySound(rotateTetraminoSFX);
                }
            }

            if (IsKeyPressed(KEY_LEFT_CONTROL) && deletingLines == 0)
            {
                const int lastRotation = currentTetromino.currentRotation;

                currentTetromino.currentRotation--;

                if (currentTetromino.currentRotation < 0)
                {
                    currentTetromino.currentRotation = 3;
                }

                //if collision after tetromino rotation go back to last rotation else make rotation and play a sound
                if (CheckCollision(currentTetromino.posX, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.currentRotation = lastRotation;
                }
                else
                {
                    PlaySound(rotateTetraminoSFX);
                }
            }

            if (IsKeyPressed(KEY_RIGHT) && deletingLines == 0)
            {
                // No need to check overflow, wall is your protector
                if (!CheckCollision(currentTetromino.posX+1, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.posX++;
                }
            }
            
            if (IsKeyPressed(KEY_LEFT) && deletingLines == 0)
            {
                // No need to check overflow, wall is your protector
                if (!CheckCollision(currentTetromino.posX-1, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.posX--;
                }
            }
            
            if (IsKeyPressed(KEY_SPACE) && deletingLines == 0)
            {
                while(!CheckCollision(currentTetromino.posX, currentTetromino.posY+1, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.posY++;
                    timeToMoveTetrominoDown = moveTetrominoDownTimer;
                }

                //tetromino lockdown event
                transformTetrominoToStage(&currentTetromino);
                
                if (deletingLines < 1)
                {
                    PlaySound(collisionTetraminoSFX);
                }

                if(checkLinesToDelete())
                {
                    deletingLines = 1;
                }
                else
                {

                    //no new lines to delete, clear combo counter
                    comboCounter = 0;

                    currentTetromino = nextTetromino;
                    currentTetromino.posX = tetrominoStartX;
                    currentTetromino.posY = tetrominoStartY;

                    nextTetromino = *getNewRandomTetromino();
                    nextTetromino.posX = (tetrominoStartX - 10);
                    nextTetromino.posY = tetrominoStartY;

                    if(CheckCollision(currentTetromino.posX, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                    {
                        gameOver = 1;
                    }
                }
            }
        
            if(timeToMoveTetrominoDown <= 0 || IsKeyPressed(KEY_DOWN) || deletingLines != 0)
            {      
                
                if(!CheckCollision(currentTetromino.posX, currentTetromino.posY+1, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                {
                    currentTetromino.posY++;
                    timeToMoveTetrominoDown = moveTetrominoDownTimer;
                }

                else
                {   
                    //tetromino lockdown event
                    transformTetrominoToStage(&currentTetromino);
                    
                    if (deletingLines < 1)
                    {
                        PlaySound(collisionTetraminoSFX);
                    }

                    if(checkLinesToDelete())
                    {
                        deletingLines = 1;
                    }
                    else
                    {
                        //no new lines to delete, clear combo counter
                        comboCounter = 0;

                        currentTetromino = nextTetromino;
                        currentTetromino.posX = tetrominoStartX;
                        currentTetromino.posY = tetrominoStartY;

                        nextTetromino = *getNewRandomTetromino();
                        nextTetromino.posX = (tetrominoStartX - 10);
                        nextTetromino.posY = tetrominoStartY;

                        if(CheckCollision(currentTetromino.posX, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation)))
                        {
                            gameOver = 1;
                        }
                    }                 
                }
            }
        }

        BeginDrawing();
        
        ClearBackground(BLACK);

        DrawText("Points:", 480, 100, 20, LIGHTGRAY);
        DrawText(TextFormat("%d", points), 480, 130, 20, WHITE);

        DrawText("Level:", 480, 160, 20, LIGHTGRAY);
        DrawText(TextFormat("%d", level), 480, 190, 20, WHITE);

        DrawText("Lines:", 480, 220, 20, LIGHTGRAY);
        DrawText(TextFormat("%d", lines), 480, 250, 20, WHITE);

        drawStage(startOffsetX, startOffsetY, LIGHTGRAY);

        //draw Next tetromino
        drawTetromino(nextTetromino.tetrominoColor, startOffsetX, startOffsetY, nextTetromino.posX, nextTetromino.posY, getTetrominoAtRotation(&nextTetromino, nextTetromino.currentRotation));
        
        if(deletingLines < 1)
        {
            //draw Current tetromino
            drawTetromino(currentTetromino.tetrominoColor, startOffsetX, startOffsetY, currentTetromino.posX, currentTetromino.posY, getTetrominoAtRotation(&currentTetromino, currentTetromino.currentRotation));
        }

        EndDrawing();
    }

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

//points by https://tetris.wiki/Scoring and https://web.archive.org/web/20140701182459/http://zone.tetris.com/page/manual
int calcPoints(int linesCombo, int levelMultiplier, int* comboCounter)
{
    int actionTotalPoints = 0;
    
    //single
    if(linesCombo==1)
    {
        actionTotalPoints = 100 * levelMultiplier;
    }

    //double
    if(linesCombo==2)
    {
        actionTotalPoints = 300 * levelMultiplier;
    }

    //triple
    if(linesCombo==3)
    {
        actionTotalPoints = 500 * levelMultiplier;
    }

    //tetris
    if(linesCombo==4)
    {
        actionTotalPoints = 800 * levelMultiplier;
    }

    //add combo bonus
    if(!comboCounter)
    {
        *comboCounter += 1;
    }
    else
    {   
        *comboCounter += 1;
        
        if(*comboCounter == 2)
        {
            actionTotalPoints += 50 * levelMultiplier;
        }
        else if(*comboCounter == 3)
        {
            actionTotalPoints += 100 * levelMultiplier;
        }
        else if(*comboCounter == 4)
        {
            actionTotalPoints += 150 * levelMultiplier;
        }
        else if(*comboCounter == 5)
        {
            actionTotalPoints += 200 * levelMultiplier;
        }
        else if(*comboCounter == 6)
        {
            actionTotalPoints += 250 * levelMultiplier;
        }
        else if(*comboCounter == 7)
        {
            actionTotalPoints += 300 * levelMultiplier;
        }
        else if(*comboCounter == 8)
        {
            actionTotalPoints += 350 * levelMultiplier;
        }
        else if(*comboCounter == 9)
        {
            actionTotalPoints += 400 * levelMultiplier;
        }
        else if(*comboCounter == 10)
        {
            actionTotalPoints += 450 * levelMultiplier;
        }
        else if(*comboCounter == 11)
        {
            actionTotalPoints += 500 * levelMultiplier;
        }
        else if(*comboCounter == 12)
        {
            actionTotalPoints += 550 * levelMultiplier;
        }
        else if(*comboCounter == 13)
        {
            actionTotalPoints += 600 * levelMultiplier;
        }
        else if(*comboCounter == 14)
        {
            actionTotalPoints += 650 * levelMultiplier;
        }
        else if(*comboCounter == 15)
        {
            actionTotalPoints += 700 * levelMultiplier;
        }
        else if(*comboCounter == 16)
        {
            actionTotalPoints += 750 * levelMultiplier;
        }
        else if(*comboCounter == 17)
        {
            actionTotalPoints += 800 * levelMultiplier;
        }
        else if(*comboCounter == 18)
        {
            actionTotalPoints += 850 * levelMultiplier;
        }
        else if(*comboCounter == 19)
        {
            actionTotalPoints += 900 * levelMultiplier;
        }
        else if(*comboCounter == 20)
        {
            actionTotalPoints += 950 * levelMultiplier;
        }
        else
        {
            actionTotalPoints += 1000 * levelMultiplier;   
        }
    }

    return actionTotalPoints;
}
