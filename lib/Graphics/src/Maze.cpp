#include "Maze.h"
#include <iostream>

Maze::Maze() {}

bool Maze::forwardBlock(char playerType, int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2) {
    // model length and width
    double length;
    double modelHalfWidth;
    

    if (playerType == Cat) {
        length = 0.25; // estimate cat length
        modelHalfWidth = .1; // just approximating
    }
    else {
        length = .15; // estimate mouse length
        modelHalfWidth = .05;
       
    }

    // cardinal directions
    bool headingN = mod2 >= 0;
    bool headingS = mod2 < 0;
    bool headingW = mod1 >= 0;
    bool headingE = mod1 < 0;
    
    return isObstacle(playerID, currPlayerXLoc, currPlayerZLoc, mod1, mod2, length, modelHalfWidth, headingN, headingS, headingE, headingW);
}

bool Maze::backwardsBlock(char playerType, int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2) {
    // model length and width
    double length; 
    double modelHalfWidth; 

    if (playerType == Cat) {
        length = 0.25; // estimate dino length
        modelHalfWidth = .08; // just approximating
    }
    else {
        length = .15; // estimate rabbit/teapot length
        modelHalfWidth = .05;
    }

      // cardinal directions
    bool headingN = mod2 < 0;
    bool headingS = mod2 >= 0;
    bool headingW = mod1 < 0;
    bool headingE = mod1 >= 0;

    // rotate perceived orientation of maze for backwards motion
    int ID = (playerID + 2) % 4;

    return isObstacle(playerID, currPlayerXLoc, currPlayerZLoc, mod1, mod2, length, modelHalfWidth, headingN, headingS, headingE, headingW);
}

bool Maze::leftBlock(char playerType, int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2) {
    // model length and width
    double length; 
    double modelHalfWidth; 

    if (playerType == Cat) {
        length = 0.1; // estimate dino length
        modelHalfWidth = .23; // just approximating
    }
    else {
        length = .1; // estimate mouse length
        modelHalfWidth = .15;
    }
    
    // cardinal directions
    bool headingN = mod2 >= 0;
    bool headingS = mod2 < 0;
    bool headingW = mod1 >= 0;
    bool headingE = mod1 < 0;
  
    // rotate perceived orientation of maze for strafing
    int ID = (playerID + 1) % 4;

    return isObstacle(ID, currPlayerXLoc, currPlayerZLoc, mod1, mod2, length, modelHalfWidth, headingN, headingS, headingE, headingW);
}

bool Maze::rightBlock(char playerType, int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2) {
    // model length and width
    double length; 
    double modelHalfWidth; 

    if (playerType == Cat) {
        length = 0.1; // estimate dino length
        modelHalfWidth = .23; // just approximating
    }
    else {
        length = .1; // estimate rabbit/teapot length
        modelHalfWidth = .15;
    }

    // cardinal directions
    bool headingN = mod2 >= 0;
    bool headingS = mod2 < 0;
    bool headingW = mod1 >= 0;
    bool headingE = mod1 < 0;

    // rotate perceived orientation of maze for strafing
    int ID = (playerID +3) % 4;

    return isObstacle(ID, currPlayerXLoc, currPlayerZLoc, mod1, mod2, length, modelHalfWidth, headingN, headingS, headingE, headingW);
}





bool Maze::isObstacle(int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2, double length, double modelHalfWidth,
    bool headingN, bool headingS, bool headingE, bool headingW) {

    bool obstacle = false;

    // location in maze
    currPlayerXLoc = abs((currPlayerXLoc - 150) / 10);
    currPlayerZLoc = abs(currPlayerZLoc / 10);

    //printf("x %lf z %lf\n", currPlayerXLoc, currPlayerZLoc);
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
        //printf("North angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length)
            return true;
        else if (abs(angle) > 1.22) {
            if (distToWall - .3 <= length)
                return true;
            else if (abs(angle) > 1.39 && distToWall - .6 <= length)
                return true;
            else if (abs(angle) > 1.48 && distToWall - 1 <= length)
                return true;

        }
    }


    if (left && headingW) {
        double angle = atan(mod2 / mod1);
        double distToWall = abs(distLeft / cos(angle)) - modelHalfWidth * abs(sin(angle)); 
        //printf("West angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length)
            return true;
        else if (abs(angle) > 1.22) {
            if (distToWall - .3 <= length)
                return true;
            else if (abs(angle) > 1.39 && distToWall - .6 <= length)
                return true;
            else if (abs(angle) > 1.48 && distToWall - 1 <= length)
                return true;

        }
    }


    if (down && headingS) {
        double angle = atan(mod1 / mod2);

        double distToWall = abs(distDown / cos(angle)) - modelHalfWidth * abs(sin(angle));
        //printf("South angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length)
            return true;
        else if (abs(angle) > 1.22) {
            if (distToWall - .3 <= length)
                return true;
            else if (abs(angle) > 1.39 && distToWall - .6 <= length)
                return true;
            else if (abs(angle) > 1.48 && distToWall - 1 <= length)
                return true;

        }
    }


    if (right && headingE) {
        double angle = atan(mod2 / mod1);

        double distToWall = abs(distRight / cos(angle)) - modelHalfWidth * abs(sin(angle));
        //printf("East angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length)
            return true;
        else if (abs(angle) > 1.22) {
            if (distToWall - .3 <= length)
                return true;
            else if (abs(angle) > 1.39 && distToWall - .6 <= length)
                return true;
            else if (abs(angle) > 1.48 && distToWall - 1 <= length)
                return true;

        }
    }

    return obstacle;
}

bool Maze::rotateBlock(char playerType, int playerID, double currPlayerXLoc, double currPlayerZLoc, double mod1, double mod2, double dir) {

    // model length and width
    double length; 
    double modelHalfWidth; 

    if (playerType == Cat) {
        length = 0.16; // estimate dino length
        modelHalfWidth = .5; // just approximating
    }
    else {
        length = .15; // estimate rabbit/teapot length
        modelHalfWidth = .1;
    }


    // cardinal directions
    bool headingN = mod2 >= 0;
    bool headingS = mod2 < 0;
    bool headingW = mod1 >= 0;
    bool headingE = mod1 < 0;

    bool obstacle = false;

    // location in maze
    currPlayerXLoc = abs((currPlayerXLoc - 150) / 10);
    currPlayerZLoc = abs(currPlayerZLoc / 10);

    //printf("x %lf z %lf\n", currPlayerXLoc, currPlayerZLoc);
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

        //if (headingW)
        //printf("North angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length) {
            if (headingW && dir < 0)
                return true;
            else if (headingE && dir > 0)
                return true;
        }

    }


    if (left && headingW) {
        double angle = atan(mod2 / mod1);
        double distToWall = abs(distLeft / cos(angle)) - modelHalfWidth * abs(sin(angle));
        //printf("West angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length) {
            if (headingN && dir > 0)
                return true;
            else if (headingS && dir < 0)
                return true;
        }

    }


    if (down && headingS) {
        double angle = atan(mod1 / mod2);

        double distToWall = abs(distDown / cos(angle)) - modelHalfWidth * abs(sin(angle));
        // printf("South angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length) {
            if (headingE && dir < 0)
                return true;
            else if (headingW && dir > 0)
                return true;
        }

    }


    if (right && headingE) {
        double angle = atan(mod2 / mod1);

        double distToWall = abs(distRight / cos(angle)) - modelHalfWidth * abs(sin(angle));
        //printf("East angle %lf distToWall %lf\n", angle * 180 / pi, distToWall);
        if (distToWall <= length) {
            if (headingN && dir < 0)
                return true;
            else if (headingS && dir > 0)
                return true;
        }

    }

    return obstacle;
}
