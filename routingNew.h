// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#ifndef SUPERMACRO_ROUTINGNEW_H
#define SUPERMACRO_ROUTINGNEW_H
#include "stack"
#include "queue"
#include "readInDataStruct.h"
using std::stack;
using std::queue;


// UTILITIES
string pointInfoGen(const int &roomLength, char curDrc,const uint32_t& curIdx, const uint32_t curRoomIdx);
Point* takeOneDirection(vector<vector<Point>> *castle, const int &curRoomInx, const unsigned int &curPointInx, const char &direction, const int& roomSize);
// UTILITIES

// BEGIN STACK
int PushDecisionStack(Point *nextPossiblePos, int *foundCountess, int *tilesDiscovered, stack<Point *> *visitedStack);
Point *
findRoute_stack(stack<Point *> *visitedStack, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles, pos *CountessPosition);
void backTrackingCastleStack(vector<vector<Point>> *castle, Point* countessLct, const int& roomLength,const pos& countessPosition);
void printMapStack(Point* countessPnt, vector<vector<Point>> *castle,
                   const int& roomNum, const int& startRoom,
                   const int& startIdx, const int& roomLength, const pos& countessPosition);
void printListStack(Point *countessPnt, vector<vector<Point>> *castle, const int &roomLength, const pos &countessLct);
// END STACK

// BEGIN QUEUE
int PushDecisionQueue(Point *nextPossiblePos, int *foundCountess, int *tilesDiscovered, queue<Point *> *visitedQueue);
Point *
findRoute_queue(queue<Point *> *visitedQueue, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles, pos *countessLct);
void backTrackingCastleQueue(vector<vector<Point>> *castle, Point* countessLct, const int& roomLength, const pos& countessPosition);
void printMapQueue(Point* countessPnt, vector<vector<Point>> *castle, const int& roomNum,
                   const int& startRoom, const int& startIdx, const int& roomLength, const pos& countessPosition);
void printListQueue(Point* countessPnt, vector<vector<Point>> *castle, const int& roomLength,const pos& countessLct);
// END QUEUE

#endif //SUPERMACRO_ROUTINGNEW_H
