#include "player.h"
#include "map_data.h"

void Player::move(int dr, int dc) {
    int newRow = row + dr;
    int newCol = col + dc;
    if (newRow >= 0 && newRow < MAP_ROWS && newCol >= 0 && newCol < MAP_COLS) {
        map[row][col] = mapOriginal[row][col];
        row = newRow;
        col = newCol;
        map[row][col] = 6;
    }
}
