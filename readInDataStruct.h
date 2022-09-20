// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

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
    unsigned int idx = 0;
};
struct pos{
    uint32_t idx = 0;
    uint32_t roomIdx = 0;
    pos(uint32_t idx, uint32_t roomIdx){
        this->idx = idx;
        this->roomIdx = roomIdx;
    }
};

void readInputMap(vector<vector<Point>> *castle,const int& roomLength,int *startRoom,int *startIdx);

void readInputList(vector<vector<Point>> *castle,const int& roomNum,const int& roomLength,int *startRoom,int *startIdx);

void printMap(vector<vector<Point>> *castle, const int& roomNum, const int& roomLength);

#endif //SUPERMACRO_READINDATASTRUCT_H
