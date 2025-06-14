#pragma once

struct Player {
    int row = 1;
    int col = 1;

    void move(int dr, int dc);
};
