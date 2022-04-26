#include "Maze.h"

Maze::Maze() { turn = 0; }

bool Maze::isObstacle(int playerID, double currPlayerXLoc, double currPlayerZLoc) {
	bool obstacle = false;
    bool headingN = (turn >= -90 && turn <= 90) || turn <= -270 || turn >= 270;
    bool headingE = (turn >= 0 && turn <= 180) || (turn <= -180);
    bool headingS = (turn >= 90 && turn <= 270) || (turn <= -90 && turn >= -270);
    bool headingW = (turn >= 180 && turn <= 360) || (turn <= 0 && turn >= -180);

    currPlayerXLoc = abs((currPlayerXLoc - 150) / 10);
    currPlayerZLoc = abs(currPlayerZLoc / 10);
    double turnR = (turn * pi) / 180;
    int currZIndexN;
    int currXIndexN;
    int currXIndexW;
    int currZIndexW;
    int currZIndexS;
    int currXIndexS;
    int currZIndexE;
    int currXIndexE;
    bool up = true;
    bool down = true;
    bool right = true;
    bool left = true;
    double distUp = 0.0;
    double distDown = 0.0;
    double distLeft = 0.0;
    double distRight = 0.0;


    switch (playerID) {
    case 1:
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
    case 2:
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
    case 3:
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
    case 4:
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

    if (up && headingN) {

        //spdlog::info("distX {}", distX);
        distUp = distUp + modelHalfWidth * sin(turnR); // where .1 is 1/2 width of dino
        //spdlog::info("distX {}", distX);
        double distToWall = abs(distUp / cos(turnR));
        //spdlog::info("distToWall {}", distToWall);

        if (distToWall <= .4)
            obstacle = true;

    }


    if (left && headingW) {
        distLeft = distLeft + modelHalfWidth * sin(turnR + (pi / 2));
        double distToWall = abs(distLeft / cos(turnR + (pi / 2)));

        if (distToWall <= .4)
            obstacle = true;
    }


    if (down && headingS) {
        distDown = distDown + modelHalfWidth * sin(turnR + pi);
        double distToWall = abs(distDown / cos(turnR + pi));

        if (distToWall <= .4)
            obstacle = true;
    }


    if (right && headingE) {

        distRight = distRight + modelHalfWidth * sin(turnR + ((3 * pi) / 2));
        double distToWall = abs(distRight / cos(turnR + ((3 * pi) / 2)));


        if (distToWall <= .4)
            obstacle = true;
    }

	return obstacle;
}