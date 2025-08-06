#pragma once

#define TURTLE_HEIGHT 6
#define TURTLE_WIDTH 24
#define TURTLE_HP 30

static const char* turtleGraphic[2][TURTLE_HEIGHT][TURTLE_WIDTH] = {
    {
    "        ______ ",
    "  ___ _/ \\__/ \\_   /|",
    " (_o / \\ /  \\ / \\_/ |",
    "  \\__ -----------__/",
    "      \\_|_|_|_|_/ ",
    "     /_|_|  /_|_|"
    },
    {
        "       ______"
        "|\\   _/ \\__/ \\_ ___"
        "| \\_/ \\ /  \\ / \\ o_)"
        " \\__----------- __/"
        "    \\_|_|_|_|_/"
        "    |_|_\\  |_|_\\"
    }
};

void DrawTurtle(int dir, int posX, int posY);