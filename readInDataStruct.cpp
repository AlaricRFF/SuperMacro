//
// Created by Yuxiang Chen on 9/18/22.
//



#include "readInDataStruct.h"

// resize has done outside the function !
void readInputMap(vector<vector<Point>> *castle,const int& roomLength,int *startRoom,int *startIdx){

    string line;
    int rCnt = 0; // start from room 0, i.e, *castle[rCnt]
    int perRoomCnt = 0; // max is roomLength
    while (getline(std::cin,line) ){
        // skip the comment
        if (line[0] == '/') {
            continue;
        }
        // at room rCnt, one line
        for (int i = 0; i < roomLength; ++i) {
            char t = line[i];
            // input error checking
            if (t != '.' &&
                t != '!' &&
                t != '#' &&
                !(t - '0' >= 0 && '9' - t >= 0) &&
                t != 'S' &&
                t != 'C'){
                throw IllegalInput;
            }
            else if (t == 'S'){
                *startIdx = i + perRoomCnt * roomLength;
                *startRoom = rCnt;
            }
            // push to Room
            Point *curPnt = &((*castle)[rCnt][( perRoomCnt % roomLength ) * roomLength + i]);
            curPnt->pnt_type = t;
            curPnt->idx = ( perRoomCnt % roomLength ) * roomLength + i;
            curPnt->roomIdx = std::to_string(rCnt)[0];
        }
        perRoomCnt ++;
        if (perRoomCnt == roomLength){
            perRoomCnt = perRoomCnt % roomLength;
            rCnt ++;
        }
    }
}

// resize has done outside the function !
void readInputList(vector<vector<Point>> *castle,const int& roomNum,const int& roomLength,int *startRoom,int *startIdx){
    string line;
    for (int i = 0; i < roomNum; ++i) {
        for (int j = 0; j < roomLength * roomLength; ++j) {
            Point *pnt = &((*castle)[i][j]);
            pnt->pnt_type = '.';
            pnt->idx = j;
            pnt->roomIdx = std::to_string(i)[0];
        }
    }
    while (getline(std::cin,line) ){
        // skip the comment
        if (line[0] == '/') {
            continue;
        }
        int rN, row, col;
        char t; // type
        bool flag = false; // if . or # or ! or C or S is met, turn to true
        sscanf(line.c_str(),"(%d,%d,%d,%c)",&rN,&row,&col,&t);

        // error checking
        if (rN < 0 or rN >= roomNum){
            throw LRoomNotLegal;
        }
        else if (row < 0 or row >= roomLength){
            throw InvalidRow;
        }
        else if (col < 0 or col >= roomLength){
            throw InvalidCol;
        }
        else if (t != '.' &&
                t != '!' &&
                t != '#' &&
                !(t - '0' >= 0 && '9' - t >= 0) &&
                t != 'S' &&
                t != 'C'){
                throw IllegalInput;
            }
        // no error, proceed
        else {
            if (t == 'S'){
                *startIdx = row * roomLength + col;
                *startRoom = rN;
            }
            Point *curPnt = &((*castle)[rN][row * roomLength + col]);
            curPnt->pnt_type = t;
        }
    }
}

void printMap(vector<vector<Point>> *castle, const int &roomNum, const int &roomLength) {
    for (int i = 0; i < roomNum; ++i) {
        printf("//castle room %d\n",i);
        for (int j = 0; j < roomLength * roomLength; ++j) {
            Point pnt = (*castle)[i][j];
            if (j % roomLength == roomLength - 1 && j != roomLength * roomLength - 1) {
                if (pnt.direction == '\0')
                    printf("%c\n",pnt.pnt_type);
                else
                    printf("%c\n",pnt.direction);
            } else {
                if (pnt.direction == '\0')
                    printf("%c",pnt.pnt_type);
                else
                    printf("%c",pnt.direction);
            }
        }
        printf("\n");
    }
}
