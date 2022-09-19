//
// Created by Yuxiang Chen on 9/18/22.
//

#ifndef SUPERMACRO_ROUTINGNEW_H
#define SUPERMACRO_ROUTINGNEW_H
#include "stack"
#include "queue"
#include "readInDataStruct.h"
using std::stack;
using std::queue;


// UTILITIES
Point* takeOneDirection(vector<vector<Point>> *castle, const int &curRoomInx, const unsigned int &curPointInx, const char &direction, const int& roomSize);
// UTILITIES

// BEGIN STACK
int PushDecisionStack(Point *nextPossiblePos, int *foundCountess, int *tilesDiscovered, stack<Point *> *visitedStack);
Point *
findRoute_stack(stack<Point *> *visitedStack, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles);
void backTrackingCastleStack(vector<vector<Point>> *castle, Point* countessLct, const int& roomLength);
void printMapStack(Point* countessPnt, vector<vector<Point>> *castle,
                   const int& roomNum, const int& startRoom, const int& startIdx, const int& roomLength);
// END STACK

#endif //SUPERMACRO_ROUTINGNEW_H
