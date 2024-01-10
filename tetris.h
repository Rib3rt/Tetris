#define STAGE_WIDTH 12
#define STAGE_HEIGHT 22
#define TILE_SIZE 24
#define TETROMINO_SIZE 4
#define LINES2LEVELUP 10
#define TICK 30

#include "raylib.h"

typedef struct tetromino
{
    int rot_0[(TETROMINO_SIZE*TETROMINO_SIZE)];
    int rot_90[(TETROMINO_SIZE*TETROMINO_SIZE)];
    int rot_180[(TETROMINO_SIZE*TETROMINO_SIZE)];
    int rot_270[(TETROMINO_SIZE*TETROMINO_SIZE)];
    Color tetrominoColor;
    int posX;
    int posY;
    int currentRotation;
    int tetrominoID;

} Tetromino;

int CheckCollision(const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);

void clearLines();

int checkLinesToDelete();

const int* getTetrominoAtRotation(const Tetromino *tetromino, const int tetrominoRotation);

Tetromino* getNewRandomTetromino();

void drawStage(const int startOffsetX, const int startOffsetY, Color stageColor);

void drawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);

void transformTetrominoToStage(const Tetromino *currentTetromino);