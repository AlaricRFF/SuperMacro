// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#include "routingNew.h"
// NOTE: direction contains : 'S', 'n', 'e', 's', 'w', number in char, such as '0', '1', etc AND '\0' !!!
// NOTE: '\0' means this point has NOT BEEN VISITED !
// NOTE: when printing as MAP, the point with pnt_type as @'V' means on the route !!!
// The @'P' means it is a PIPE !!!


// UTILITIES

string pointInfoGen(const int &roomLength, char curDrc,
                    const uint32_t& curIdx, const uint32_t curRoomIdx) {
    if (curDrc - '0' >= 0 && '9' - curDrc >= 0){
        return "(" + std::to_string(curRoomIdx)
               + "," + std::to_string((curIdx / roomLength) )
               + "," + std::to_string( (curIdx % roomLength) )
               + ",p)";
    }else
    return "(" + std::to_string(curRoomIdx)
           + "," + std::to_string((curIdx / roomLength) )
           + "," + std::to_string( (curIdx % roomLength) )
           + "," + curDrc + ")";
}

Point* takeOneDirection(vector<vector<Point>> *castle, const int &curRoomInx, const unsigned int &curPointInx, const char &direction, const int& roomSize){

    if (direction == 'n') {
        if ((int)curPointInx > roomSize - 1) { // the point is not on the first row of the room
            Point *nextPos = &((*castle)[curRoomInx][curPointInx - roomSize]);
            return (nextPos->direction == '\0' ? nextPos : nullptr); // check whether the point has been visited
        } else
            return nullptr;
    } else if (direction == 'w') {
        if ((int)curPointInx % roomSize != 0) { // the point is not on the first column of the room
            Point *nextPos = &( (*castle)[curRoomInx][curPointInx - 1] );
            return (nextPos->direction == '\0' ? nextPos : nullptr);
        } else
            return nullptr;
    } else if (direction == 's') {
        if ((int)curPointInx < roomSize * (roomSize - 1)) { // the point is not on the last row of the room
            Point *nextPos = &( (*castle)[curRoomInx][curPointInx + roomSize] );
            return (nextPos->direction == '\0' ? nextPos : nullptr);
        } else
            return nullptr;
    } else { // direction is 'e'
        if ((int)curPointInx % roomSize != roomSize - 1) {
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

// pos return the position of the countess
Point *
findRoute_stack(stack<Point *> *visitedStack, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles, pos *CountessPosition) {
    stack<pos*> positionStack;
    int  findCountess = 0;
    // push start to visitedStack
    Point *start = &((*castle)[startRoom][startIdx]);
    // dynamic memory allocated !
    pos *startPos = new pos(startIdx, startRoom);
    positionStack.push(startPos);
    visitedStack->push( start );
    Point *curPos = start;
    start->direction = 'S';
    // Start search through the castle
    while (true){
        // current location
        pos *position = positionStack.top();
        uint32_t curRoomNum = position->roomIdx;
        uint32_t  curPntIdx = position->idx;
        curPos = visitedStack->top();
        visitedStack->pop(); // WILL the INFO of this point be DELETED after popping ???
        positionStack.pop();
        // free dynamic memory !!
        delete position;

        // first, check if Pipe
        Point *nextPosIfPipe = nullptr;
        if ( (curPos->pnt_type - '0' >= 0) && ('9' - curPos->pnt_type >= 0) ){
            int pipeTo = (int) curPos->pnt_type - '0';
            if ( pipeTo < roomNum) {
                nextPosIfPipe = &(*castle)[pipeTo][curPntIdx];
                //nextPosIfPipe = &((*castle)[pipeTo][curPos->idx]);
                if (PushDecisionStack(nextPosIfPipe, &findCountess,totalTiles,visitedStack) == 1) {
                    if (findCountess == 1) { /// find countess, return that point
                        // IMPORTANT: already updated in PushDecisionStack !
                        nextPosIfPipe->direction = std::to_string(curRoomNum)[0];
                        CountessPosition->idx = curPntIdx;
                        CountessPosition->roomIdx = pipeTo;
                        // free dynamic memory
                        while (!positionStack.empty()){
                            auto p = positionStack.top();
                            positionStack.pop();
                            delete p;
                        }
                        return nextPosIfPipe;
                    }
                    nextPosIfPipe->direction = std::to_string(curRoomNum)[0];
                    pos *newPosition = new pos(curPntIdx, pipeTo);
                    positionStack.push(newPosition);
                }
            }
        }
        else{
            // int curRoomNum = (int) curPos->roomIdx - '0';
            // unsigned int curPntIdx = curPos->idx;
            Point *NorthP = takeOneDirection(castle,curRoomNum,curPntIdx,'n',roomLength);
            Point *EastP = takeOneDirection(castle,curRoomNum,curPntIdx,'e',roomLength);
            Point *SouthP = takeOneDirection(castle,curRoomNum,curPntIdx,'s',roomLength);
            Point *WestP = takeOneDirection(castle,curRoomNum,curPntIdx,'w',roomLength);


            // North accessible
            if (PushDecisionStack(NorthP, &findCountess, totalTiles, visitedStack) == 1){
                NorthP->direction = 'n';
                pos *northP = new pos(curPntIdx - roomLength,curRoomNum);
                positionStack.push(northP);
                if (findCountess == 1){
                    CountessPosition->idx = curPntIdx - roomLength;
                    CountessPosition->roomIdx = curRoomNum;
                    while (!positionStack.empty()){
                        auto p = positionStack.top();
                        positionStack.pop();
                        delete p;
                    }
                    return NorthP;
                }

            }
            if (PushDecisionStack(EastP, &findCountess, totalTiles, visitedStack) == 1){
                EastP->direction = 'e';
                pos *eastP = new pos(curPntIdx + 1, curRoomNum);
                positionStack.push(eastP);
                if (findCountess == 1){
                    CountessPosition->roomIdx = curRoomNum;
                    CountessPosition->idx = curPntIdx + 1;
                    while (!positionStack.empty()){
                        auto p = positionStack.top();
                        positionStack.pop();
                        delete p;
                    }
                    return EastP;
                }
            }
            if (PushDecisionStack(SouthP, &findCountess, totalTiles, visitedStack) == 1){
                SouthP->direction = 's';
                pos *southP = new pos(curPntIdx + roomLength, curRoomNum);
                positionStack.push(southP);
                if (findCountess == 1){
                    CountessPosition->roomIdx = curRoomNum;
                    CountessPosition->idx = curPntIdx + roomLength;
                    while (!positionStack.empty()){
                        auto p = positionStack.top();
                        positionStack.pop();
                        delete p;
                    }
                    return SouthP;
                }
            }
            if (PushDecisionStack(WestP, &findCountess, totalTiles, visitedStack) == 1){
                WestP->direction = 'w';
                pos *westP = new pos(curPntIdx - 1, curRoomNum);
                positionStack.push(westP);
                if (findCountess == 1){
                    CountessPosition->roomIdx = curRoomNum;
                    CountessPosition->idx = curPntIdx - 1;
                    while (!positionStack.empty()){
                        auto p = positionStack.top();
                        positionStack.pop();
                        delete p;
                    }
                    return WestP;
                }
            }
        }
        if (visitedStack->empty()){
            return nullptr;
        }
    }
}


void backTrackingCastleStack(vector<vector<Point>> *castle, Point* countessLct, const int& roomLength, const pos& countessPosition){
    Point *nextPnt = nullptr;
    uint32_t curIdx = countessPosition.idx;
    uint32_t curRoomIdx = countessPosition.roomIdx;
    char nextDirection = countessLct->direction;
    char nextToNext;
    while(true){
        if (nextDirection - '0' >= 0 && '9' - nextDirection >= 0){
            int a = nextDirection  - '0';
            nextPnt = &( (*castle)[a][curIdx] );
            curRoomIdx = a;
        }
        else if(nextDirection == 'n'){
            curIdx += roomLength;
            nextPnt = & ( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 'e'){
            curIdx -= 1;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 's'){
            curIdx -= roomLength;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 'w'){
            curIdx += 1;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
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
    }
}

void printMapStack(Point* countessPnt, vector<vector<Point>> *castle, const int& roomNum, const int& startRoom,
                   const int& startIdx, const int& roomLength, const pos& countessPosition){
    backTrackingCastleStack(castle,countessPnt,roomLength,countessPosition);
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

void printListStack(Point *countessPnt, vector<vector<Point>> *castle, const int &roomLength, const pos &countessLct) {
    stack<string*> listOutput;
    Point *curPnt = countessPnt;
    Point *prevPnt  = nullptr;
    uint32_t curIdx = countessLct.idx;
    uint32_t curRoomIdx = countessLct.roomIdx;
    printf("Path taken:\n");
    while (curPnt->pnt_type != 'S'){
        if ( curPnt->direction - '0' >= 0 && '9' - curPnt->direction >= 0){
            curRoomIdx= (int) (curPnt->direction - '0');
            prevPnt = &(*castle)[curRoomIdx][curIdx];

        }
        else if (curPnt->direction == 'n'){
            curIdx += roomLength;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 'e'){
            curIdx -= 1;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 's'){
            curIdx -= roomLength;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 'w'){
            curIdx += 1;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else{
            std::cerr << "failure in back-tracking queue! error in printListQueue!\n";
            exit(1);
        }
        auto *info = new string;
        *info = pointInfoGen(roomLength, curPnt->direction,curIdx,curRoomIdx);
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

// BEGIN QUEUE
int PushDecisionQueue(Point *nextPossiblePos, int *foundCountess, int *tilesDiscovered, queue<Point *> *visitedQueue) {
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
        visitedQueue->push(nextPossiblePos);
        (*tilesDiscovered) ++;
        return 1;
    }
    else
        return 0;
}

Point *
findRoute_queue(queue<Point *> *visitedQueue, vector<vector<Point>> *castle, const int &startRoom, const int &startIdx,
                const int &roomLength, const int &roomNum, int *totalTiles, pos *countessLct) {
    queue<pos*> positionQueue;
    int  findCountess = 0;
    // push start to visitedStack
    Point *start = &((*castle)[startRoom][startIdx]);
    // dynamic memory allocated !
    pos *startPos = new pos(startIdx, startRoom);
    positionQueue.push(startPos);
    visitedQueue->push( start );
    Point *curPos = start;
    start->direction = 'S';
    // Start search through the castle
    while (true){
        // current location
        pos *position = positionQueue.front();
        uint32_t curRoomNum = position->roomIdx;
        uint32_t  curPntIdx = position->idx;
        curPos = visitedQueue->front();
        visitedQueue->pop(); // WILL the INFO of this point be DELETED after popping ???
        positionQueue.pop();
        // free dynamic memory !!
        delete position;

        // first, check if Pipe
        Point *nextPosIfPipe = nullptr;
        if ( (curPos->pnt_type - '0' >= 0) && ('9' - curPos->pnt_type >= 0) ){
            int pipeTo = (int) curPos->pnt_type - '0';
            if ( pipeTo < roomNum) {
                nextPosIfPipe = &(*castle)[pipeTo][curPntIdx];
                //nextPosIfPipe = &((*castle)[pipeTo][curPos->idx]);
                if (PushDecisionQueue(nextPosIfPipe, &findCountess,totalTiles,visitedQueue) == 1) {
                    if (findCountess == 1) { /// find countess, return that point
                        // IMPORTANT: already updated in PushDecisionStack !
                        nextPosIfPipe->direction = std::to_string(curRoomNum)[0];
                        countessLct->idx = curPntIdx;
                        countessLct->roomIdx = pipeTo;
                        // free dynamic memory
                        while (!positionQueue.empty()){
                            auto p = positionQueue.front();
                            positionQueue.pop();
                            delete p;
                        }
                        return nextPosIfPipe;
                    }
                    nextPosIfPipe->direction = std::to_string(curRoomNum)[0];
                    pos *newPosition = new pos(curPntIdx, pipeTo);
                    positionQueue.push(newPosition);
                }
            }
        }
        else{
            // int curRoomNum = (int) curPos->roomIdx - '0';
            // unsigned int curPntIdx = curPos->idx;
            Point *NorthP = takeOneDirection(castle,(int)curRoomNum,curPntIdx,'n',roomLength);
            Point *EastP = takeOneDirection(castle,(int)curRoomNum,curPntIdx,'e',roomLength);
            Point *SouthP = takeOneDirection(castle,(int)curRoomNum,curPntIdx,'s',roomLength);
            Point *WestP = takeOneDirection(castle,(int)curRoomNum,curPntIdx,'w',roomLength);


            // North accessible
            if (PushDecisionQueue(NorthP, &findCountess, totalTiles, visitedQueue) == 1){
                NorthP->direction = 'n';
                pos *northP = new pos(curPntIdx - roomLength,curRoomNum);
                positionQueue.push(northP);
                if (findCountess == 1){
                    countessLct->idx = curPntIdx - roomLength;
                    countessLct->roomIdx = curRoomNum;
                    while (!positionQueue.empty()){
                        auto p = positionQueue.front();
                        positionQueue.pop();
                        delete p;
                    }
                    return NorthP;
                }

            }
            if (PushDecisionQueue(EastP, &findCountess, totalTiles, visitedQueue) == 1){
                EastP->direction = 'e';
                pos *eastP = new pos(curPntIdx + 1, curRoomNum);
                positionQueue.push(eastP);
                if (findCountess == 1){
                    countessLct->roomIdx = curRoomNum;
                    countessLct->idx = curPntIdx + 1;
                    while (!positionQueue.empty()){
                        auto p = positionQueue.front();
                        positionQueue.pop();
                        delete p;
                    }
                    return EastP;
                }
            }
            if (PushDecisionQueue(SouthP, &findCountess, totalTiles, visitedQueue) == 1){
                SouthP->direction = 's';
                pos *southP = new pos(curPntIdx + roomLength, curRoomNum);
                positionQueue.push(southP);
                if (findCountess == 1){
                    countessLct->roomIdx = curRoomNum;
                    countessLct->idx = curPntIdx + roomLength;
                    while (!positionQueue.empty()){
                        auto p = positionQueue.front();
                        positionQueue.pop();
                        delete p;
                    }
                    return SouthP;
                }
            }
            if (PushDecisionQueue(WestP, &findCountess, totalTiles, visitedQueue) == 1){
                WestP->direction = 'w';
                pos *westP = new pos(curPntIdx - 1, curRoomNum);
                positionQueue.push(westP);
                if (findCountess == 1){
                    countessLct->roomIdx = curRoomNum;
                    countessLct->idx = curPntIdx - 1;
                    while (!positionQueue.empty()){
                        auto p = positionQueue.front();
                        positionQueue.pop();
                        delete p;
                    }
                    return WestP;
                }
            }
        }
        if (visitedQueue->empty()){
            return nullptr;
        }
    }
}

void backTrackingCastleQueue(vector<vector<Point>> *castle, Point* countessLct,
                             const int& roomLength, const pos& countessPosition){
    Point *nextPnt = nullptr;
    uint32_t curIdx = countessPosition.idx;
    uint32_t curRoomIdx = countessPosition.roomIdx;
    char nextDirection = countessLct->direction;
    char nextToNext;
    while(true){
        if (nextDirection - '0' >= 0 && '9' - nextDirection >= 0){
            int a = nextDirection  - '0';
            nextPnt = &( (*castle)[a][curIdx] );
            curRoomIdx = a;
        }
        else if(nextDirection == 'n'){
            curIdx += roomLength;
            nextPnt = & ( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 'e'){
            curIdx -= 1;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 's'){
            curIdx -= roomLength;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
        }
        else if (nextDirection == 'w'){
            curIdx += 1;
            nextPnt = &( (*castle)[ curRoomIdx ][curIdx] );
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
    }
}

void printMapQueue(Point* countessPnt, vector<vector<Point>> *castle, const int& roomNum,
                   const int& startRoom, const int& startIdx, const int& roomLength,const pos& countessPosition){
    backTrackingCastleQueue(castle,countessPnt,roomLength,countessPosition);
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

void printListQueue(Point* countessPnt, vector<vector<Point>> *castle,
                    const int& roomLength, const pos& countessLct){
    stack<string*> listOutput;
    Point *curPnt = countessPnt;
    Point *prevPnt  = nullptr;
    uint32_t curIdx = countessLct.idx;
    uint32_t curRoomIdx = countessLct.roomIdx;
    printf("Path taken:\n");
    while (curPnt->pnt_type != 'S'){
        if ( curPnt->direction - '0' >= 0 && '9' - curPnt->direction >= 0){
            curRoomIdx= (int) (curPnt->direction - '0');
            prevPnt = &(*castle)[curRoomIdx][curIdx];

        }
        else if (curPnt->direction == 'n'){
            curIdx += roomLength;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 'e'){
            curIdx -= 1;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 's'){
            curIdx -= roomLength;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else if (curPnt->direction == 'w'){
            curIdx += 1;
            prevPnt = &(*castle)[curRoomIdx][curIdx];
        }
        else{
            std::cerr << "failure in back-tracking queue! error in printListQueue!\n";
            exit(1);
        }
        auto *info = new string;
        *info = pointInfoGen(roomLength, curPnt->direction,curIdx,curRoomIdx);
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
// END QUEUE