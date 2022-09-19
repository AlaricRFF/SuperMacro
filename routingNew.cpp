//
// Created by Yuxiang Chen on 9/18/22.
//

#include "routingNew.h"
// NOTE: direction contains : 'S', 'n', 'e', 's', 'w', number in char, such as '0', '1', etc AND '\0' !!!
// NOTE: '\0' means this point has NOT BEEN VISITED !
// NOTE: when printing as MAP, the point with pnt_type as @'V' means on the route !!!
// The @'P' means it is a PIPE !!!


// UTILITIES

string pointInfoGen(Point *pnt, const int &roomLength, char curDrc) {
    if (curDrc - '0' >= 0 && '9' - curDrc >= 0){
        return "(" + std::to_string((int) (pnt->roomIdx - '0'))
               + "," + std::to_string((pnt->idx / roomLength) )
               + "," + std::to_string( (pnt->idx % roomLength) )
               + ",p)";
    }else
    return "(" + std::to_string((int) (pnt->roomIdx - '0'))
           + "," + std::to_string((pnt->idx / roomLength) )
           + "," + std::to_string( (pnt->idx % roomLength) )
           + "," + curDrc + ")";
}

Point* takeOneDirection(vector<vector<Point>> *castle, const int &curRoomInx, const unsigned int &curPointInx, const char &direction, const int& roomSize){

    if (direction == 'n') {
        if (curPointInx > roomSize - 1) { // the point is not on the first row of the room
            Point *nextPos = &((*castle)[curRoomInx][curPointInx - roomSize]);
            return (nextPos->direction == '\0' ? nextPos : nullptr); // check whether the point has been visited
        } else
            return nullptr;
    } else if (direction == 'w') {
        if (curPointInx % roomSize != 0) { // the point is not on the first column of the room
            Point *nextPos = &( (*castle)[curRoomInx][curPointInx - 1] );
            return (nextPos->direction == '\0' ? nextPos : nullptr);
        } else
            return nullptr;
    } else if (direction == 's') {
        if (curPointInx < roomSize * (roomSize - 1)) { // the point is not on the last row of the room
            Point *nextPos = &( (*castle)[curRoomInx][curPointInx + roomSize] );
            return (nextPos->direction == '\0' ? nextPos : nullptr);
        } else
            return nullptr;
    } else { // direction is 'e'
        if (curPointInx % roomSize != roomSize - 1) {
            Point *nextPos = &( (*castle)[curRoomInx][curPointInx + 1] );
            return (nextPos->direction == '\0' ? nextPos : nullptr);
        } else
            return nullptr;
    }
}
// WARNING: NOP LONGER UPDATE DIRECTION !!!

// UTILITIES



// BEGIN STACK
int PushDecisionStack(Point *nextPossiblePos, int *foundCountess, int *tilesDiscovered, stack<Point *> *visitedStack) {
    if (nextPossiblePos == nullptr){
        return -1;
    }
    if (nextPossiblePos->pnt_type == 'C'){
        *foundCountess = 1;
        return 1;
    }
    else if (nextPossiblePos->direction == '\0' &&
             nextPossiblePos->pnt_type != '#' &&
             nextPossiblePos->pnt_type != '!'){
        visitedStack->push(nextPossiblePos);
        (*tilesDiscovered) ++;
        return 1;
    }
    else
        return 0;
}
Point *
findRoute_stack(stack<Point *> *visitedStack, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles) {
    int  findCountess = 0;
    // push start to visitedStack
    Point *start = &((*castle)[startRoom][startIdx]);
    visitedStack->push( start );
    Point *curPos = start;
    start->direction = 'S';
    // Start search through the castle
    while (true){
        curPos = visitedStack->top();
        visitedStack->pop();         // WILL the INFO of this point be DELETED after popping ???
        // first, check if Pipe
        Point *nextPosIfPipe = nullptr;
        if ( (curPos->pnt_type - '0' >= 0) && ('9' - curPos->pnt_type >= 0) ){
            int pipeTo = (int) curPos->pnt_type - '0';
            if ( pipeTo < roomNum) {
                nextPosIfPipe = &((*castle)[pipeTo][curPos->idx]);
                if (PushDecisionStack(nextPosIfPipe, &findCountess,totalTiles,visitedStack) == 1) {
                    nextPosIfPipe->direction = curPos->roomIdx;
                }
                if (findCountess == 1) { /// find countess, return that point
                    // IMPORTANT: already updated in PushDecisionStack !
                    return nextPosIfPipe;
                }
            }
        }
        else{
            int curRoomNum = (int) curPos->roomIdx - '0';
            unsigned int curPntIdx = curPos->idx;
            Point *NorthP = takeOneDirection(castle,curRoomNum,curPntIdx,'n',roomLength);
            Point *EastP = takeOneDirection(castle,curRoomNum,curPntIdx,'e',roomLength);
            Point *SouthP = takeOneDirection(castle,curRoomNum,curPntIdx,'s',roomLength);
            Point *WestP = takeOneDirection(castle,curRoomNum,curPntIdx,'w',roomLength);


            // North accessible
            if (PushDecisionStack(NorthP, &findCountess, totalTiles, visitedStack) == 1){
                NorthP->direction = 'n';
                if (findCountess == 1){
                    return NorthP;
                }

            }
            if (PushDecisionStack(EastP, &findCountess, totalTiles, visitedStack) == 1){
                EastP->direction = 'e';
                if (findCountess == 1){
                    return EastP;
                }
            }
            if (PushDecisionStack(SouthP, &findCountess, totalTiles, visitedStack) == 1){
                SouthP->direction = 's';
                if (findCountess == 1){
                    return SouthP;
                }
            }
            if (PushDecisionStack(WestP, &findCountess, totalTiles, visitedStack) == 1){
                WestP->direction = 'w';
                if (findCountess == 1){
                    return WestP;
                }
            }
        }
        if (visitedStack->empty()){
            return nullptr;
        }
    }
}


void backTrackingCastleStack(vector<vector<Point>> *castle, Point* countessLct, const int& roomLength){
    Point *curPnt = countessLct;
    Point *nextPnt = nullptr;

    char nextDirection = countessLct->direction;
    char nextToNext;
    while(true){
        if (nextDirection - '0' >= 0 && '9' - nextDirection >= 0){
            int a = nextDirection  - '0';
            nextPnt = &( (*castle)[a][curPnt->idx] );
        }
        else if(nextDirection == 'n'){
            int a = curPnt->roomIdx - '0';
            nextPnt = & ( (*castle)[ a ][curPnt->idx + roomLength] );
        }
        else if (nextDirection == 'e'){
            int a = (int) (curPnt->roomIdx - '0');
            nextPnt = &( (*castle)[ a ][curPnt->idx - 1] );
        }
        else if (nextDirection == 's'){
            int a = (int) (curPnt->roomIdx - '0');
            nextPnt = &( (*castle)[ a ][curPnt->idx - roomLength] );
        }
        else if (nextDirection == 'w'){
            int a = (int) (curPnt->roomIdx - '0');
            nextPnt = &( (*castle)[ a ][curPnt->idx + 1] );
        }
        else{
            std::cerr << "failure in Stack back-tracking! Point from no where !\n";
            exit(1);
        }
        if (nextPnt->pnt_type == 'S'){
            nextPnt->direction = nextDirection;
            break;
        }
        nextPnt->pnt_type = 'V';

        nextToNext = nextPnt->direction;  // the direction after nextPnt should take
        nextPnt->direction = nextDirection; // this is the direction this prev point should take (reason this)
        nextDirection = nextToNext;
        curPnt = nextPnt; /// update curPnt to the next loop
    }
}

void printMapStack(Point* countessPnt, vector<vector<Point>> *castle, const int& roomNum, const int& startRoom, const int& startIdx, const int& roomLength){
    backTrackingCastleStack(castle,countessPnt,roomLength);
    printf("Start in room %d, row %d, column %d\n", startRoom, (startIdx / roomLength), (startIdx % roomLength));
    for (int rIdx = 0; rIdx < roomNum; ++rIdx) {
        printf("//castle room %d\n",rIdx);
        for (int pIdx = 0; pIdx < roomLength * roomLength; ++pIdx) {
            Point *curPoint = &( (*castle)[rIdx][pIdx] );
            if (curPoint->pnt_type == 'C'){
                printf("C");
            }
            // point on the route !!!
            else if ( curPoint->pnt_type == 'V' || curPoint->pnt_type == 'S'){
                // from Pipe or it itself is a warp pipe will engage a "p" CHECK!!!
                if ( curPoint->direction - '0' >= 0 && '9' - curPoint->direction >= 0 ){
                    printf("p");
                }
                else{
                    printf("%c", curPoint->direction);
                }
            }
            // point not on the route !!
            else{
                printf("%c",curPoint->pnt_type);
            }
            if (pIdx % roomLength == roomLength - 1)
                printf("\n");
        }
    }
}

void printListStack(Point* countessPnt, vector<vector<Point>> *castle, const int& roomLength){
    stack<string*> listOutput;
    Point *curPnt = countessPnt;
    Point *prevPnt  = nullptr;
    printf("Path taken:\n");
    while (curPnt->pnt_type != 'S'){
        if ( curPnt->direction - '0' >= 0 && '9' - curPnt->direction >= 0){
            int a = (int) (curPnt->direction - '0');
            prevPnt = &(*castle)[a][curPnt->idx];
        }
        else if (curPnt->direction == 'n'){
            int a = (int) (curPnt->roomIdx - '0');
            prevPnt = &(*castle)[a][curPnt->idx + roomLength];
        }
        else if (curPnt->direction == 'e'){
            int a = (int) (curPnt->roomIdx - '0');
            prevPnt = &(*castle)[a][curPnt->idx - 1];
        }
        else if (curPnt->direction == 's'){
            int a = (int) (curPnt->roomIdx - '0');
            prevPnt = &(*castle)[a][curPnt->idx - roomLength];
        }
        else if (curPnt->direction == 'w'){
            int a = (int) (curPnt->roomIdx - '0');
            prevPnt = &(*castle)[a][curPnt->idx + 1];
        }
        else{
            std::cerr << "failure in back-tracking queue! error in printListQueue!\n";
            exit(1);
        }
        auto *info = new string;
        *info = pointInfoGen(prevPnt, roomLength, curPnt->direction);
        // debug
        // printf("%s\n",info->c_str());
        listOutput.push(info);
        curPnt = prevPnt; /// update to continue back-tracking
    }
    while(!listOutput.empty()){
        string *e = listOutput.top();
        printf("%s\n",e->c_str());
        delete e;
        listOutput.pop();
    }

}

// END STACK