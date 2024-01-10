#include "tetris.h"
#include "raylib.h"

extern Tetromino tetromino_I;
extern Tetromino tetromino_J;
extern Tetromino tetromino_L;
extern Tetromino tetromino_O;
extern Tetromino tetromino_S;
extern Tetromino tetromino_T;
extern Tetromino tetromino_Z;

const int* getTetrominoAtRotation(const Tetromino *tetromino, const int tetrominoRotation)
{
    
    if (tetrominoRotation == 0)
    {
        return tetromino->rot_0;
    }

    if (tetrominoRotation == 1)
    {
        return tetromino->rot_90;
    }

    if (tetrominoRotation == 2)
    {
        return tetromino->rot_180;
    }

    if (tetrominoRotation == 3)
    {
        return tetromino->rot_270;
    }

    return tetromino->rot_0;
    
}

Tetromino* getNewRandomTetromino()
{
    const int tetroType = GetRandomValue(0, 6);

    if (tetroType == 0)
    {
        return &tetromino_I;
    }

    if (tetroType == 1)
    {
        return &tetromino_J;
    }

    if (tetroType == 2)
    {
        return &tetromino_L;
    }

    if (tetroType == 3)
    {
        return &tetromino_O;
    }

    if (tetroType == 4)
    {
        return &tetromino_S;
    }

    if (tetroType == 5)
    {
        return &tetromino_T;
    }

    if (tetroType == 6)
    {
        return &tetromino_Z;
    }

    return &tetromino_I;
}

void drawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawRectangle((x + tetrominoStartX) * TILE_SIZE + startOffsetX, (y + tetrominoStartY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, currentColor);
            }
        }
    }
}
