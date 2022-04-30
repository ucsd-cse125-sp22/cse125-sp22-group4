#include "Maze.h"
#include <iostream>

Maze::Maze() {}

bool Maze::isObstacle(int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2) {

    bool obstacle = false;

    // cardinal directions
    bool headingN = mod2 >= 0;
    bool headingS = mod2 < 0;
    bool headingW = mod1 >= 0;
    bool headingE = mod1 < 0;

    // location in maze
    currPlayerXLoc = abs((currPlayerXLoc - 150) / 10);
    currPlayerZLoc = abs(currPlayerZLoc / 10);

    printf("x %lf y %lf\n", currPlayerXLoc, currPlayerZLoc);
    // used for checking NS/EW walls 
    int currZIndexN;
    int currXIndexN;
    int currXIndexW;
    int currZIndexW;
    int currZIndexS;
    int currXIndexS;
    int currZIndexE;
    int currXIndexE;

    // is there a wall
    bool up = true;
    bool down = true;
    bool right = true;
    bool left = true;

    // what direction is model heading
    double distUp = 0.0;
    double distDown = 0.0;
    double distLeft = 0.0;
    double distRight = 0.0;

    // model length and width
    double length = 0.23; // estimate dino length
    double modelHalfWidth = .08; // just approximating

    // cases are needed since players start with different orientation
    switch (playerID) {
    case 0:
        currXIndexN = floor(currPlayerXLoc);
        currZIndexN = ceil(currPlayerZLoc);
        currXIndexE = floor(currPlayerXLoc);
        currZIndexE = floor(currPlayerZLoc);
        currXIndexS = floor(currPlayerXLoc);
        currZIndexS = floor(currPlayerZLoc);
        currXIndexW = ceil(currPlayerXLoc);
        currZIndexW = floor(currPlayerZLoc);
        distUp = currZIndexN - currPlayerZLoc;
        distLeft = currXIndexW - currPlayerXLoc;
        distDown = currPlayerZLoc - currZIndexS;
        distRight = currPlayerXLoc - currXIndexE;
        up = mazeVerticals[currXIndexN][currZIndexN];
        right = mazeHorizontals[currXIndexE][currZIndexE];
        down = mazeVerticals[currXIndexS][currZIndexS];
        left = mazeHorizontals[currXIndexW][currZIndexW];
        break;
    case 1:
        currZIndexN = floor(currPlayerZLoc);
        currXIndexN = ceil(currPlayerXLoc);
        currXIndexE = floor(currPlayerXLoc);
        currZIndexE = ceil(currPlayerZLoc);
        currZIndexS = floor(currPlayerZLoc);
        currXIndexS = floor(currPlayerXLoc);
        currZIndexW = floor(currPlayerZLoc);
        currXIndexW = floor(currPlayerXLoc);
        distUp = currXIndexN - currPlayerXLoc;
        distLeft = currZIndexW - currPlayerZLoc;
        distDown = currPlayerXLoc - currXIndexS;
        distRight = currPlayerZLoc - currZIndexE;
        up = mazeHorizontals[currXIndexN][currZIndexN];
        left = mazeVerticals[currXIndexW][currZIndexW];
        down = mazeHorizontals[currXIndexS][currZIndexS];
        right = mazeVerticals[currXIndexE][currZIndexE];
        break;
    case 2:
        currZIndexN = floor(currPlayerZLoc);
        currXIndexN = floor(currPlayerXLoc);
        currZIndexE = floor(currPlayerZLoc);
        currXIndexE = ceil(currPlayerXLoc);
        currZIndexS = ceil(currPlayerZLoc);
        currXIndexS = floor(currPlayerXLoc);
        currZIndexW = floor(currPlayerZLoc);
        currXIndexW = floor(currPlayerXLoc);
        distUp = currPlayerZLoc - currZIndexN;
        distLeft = currPlayerXLoc - currXIndexW;
        distDown = currZIndexS - currPlayerZLoc;
        distRight = currXIndexE - currPlayerXLoc;
        up = mazeVerticals[currXIndexN][currZIndexN];
        right = mazeHorizontals[currXIndexE][currZIndexE];
        down = mazeVerticals[currXIndexS][currZIndexS];
        left = mazeHorizontals[currXIndexW][currZIndexW];
        break;
    case 3:
        currZIndexN = floor(currPlayerZLoc);
        currXIndexN = floor(currPlayerXLoc);
        currZIndexE = floor(currPlayerZLoc);
        currXIndexE = floor(currPlayerXLoc);
        currZIndexS = floor(currPlayerZLoc);
        currXIndexS = ceil(currPlayerXLoc);
        currZIndexW = ceil(currPlayerZLoc);
        currXIndexW = floor(currPlayerXLoc);
        distUp = currPlayerXLoc - currXIndexN;
        distLeft = currZIndexW - currPlayerZLoc;
        distDown = currXIndexS - currPlayerXLoc;
        distRight = currPlayerZLoc - currZIndexE;
        up = mazeHorizontals[currXIndexN][currZIndexN];
        left = mazeVerticals[currXIndexW][currZIndexW];
        down = mazeHorizontals[currXIndexS][currZIndexS];
        right = mazeVerticals[currXIndexE][currZIndexE];
        break;
    }

    // only check for obstacles if there is a wall in the direction model is heading
    if (up && headingN) {
        double angle = atan(mod1 / mod2);
        double distToWall = abs(distUp / cos(angle)) - modelHalfWidth * abs(sin(angle));

        if (distToWall <= length)
            obstacle = true;
    }


    if (left && headingW) {
        double angle = atan(mod2 / mod1);
        double distToWall = abs(distLeft / cos(angle)) - modelHalfWidth * abs(sin(angle));

        if (distToWall <= length)
            obstacle = true;
    }


    if (down && headingS) {
        double angle = atan(mod1 / mod2);

        double distToWall = abs(distDown / cos(angle)) - modelHalfWidth * abs(sin(angle));

        if (distToWall <= length)
            obstacle = true;
    }


    if (right && headingE) {
        double angle = atan(mod2 / mod1);

        double distToWall = abs(distRight / cos(angle)) - modelHalfWidth * abs(sin(angle));

        if (distToWall <= length)
            obstacle = true;
    }

    return obstacle;
}
