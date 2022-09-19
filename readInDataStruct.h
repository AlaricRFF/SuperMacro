//
// Created by Yuxiang Chen on 9/18/22.
//

#ifndef SUPERMACRO_READINDATASTRUCT_H
#define SUPERMACRO_READINDATASTRUCT_H
#include "vector"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctype.h>
using std::string;
using std::vector;
enum Err_t {IllegalInput, LRoomNotLegal, InvalidRow, InvalidCol};
struct Point{
    char pnt_type = '\0';
    char direction = '\0';
    char roomIdx = '\0';
    unsigned int idx = -1;
};

void readInputMap(vector<vector<Point>> *castle,const int& roomLength,int *startRoom,int *startIdx);

void readInputList(vector<vector<Point>> *castle,const int& roomNum,const int& roomLength,int *startRoom,int *startIdx);

void printMap(vector<vector<Point>> *castle, const int& roomNum, const int& roomLength);

#endif //SUPERMACRO_READINDATASTRUCT_H
