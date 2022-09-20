// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include "routingNew.h"
#include "readInDataStruct.h"
#include <cstring>
using std::vector;
using namespace std;

struct myOpt{
    bool withArg = false;
    char optName = '\0';
    char* optArg = nullptr ;
};

int main(int argc, char *argv[]) {
    /// get opt
    int gotopt;
    int optCnt = 0;
    vector<myOpt> commandCache;
    int optionIdx = 0;
    option long_opts[] = {
            {"stack",  no_argument,       nullptr, 's'},
            {"queue",  no_argument,       nullptr, 'q'},
            {"output", required_argument, nullptr, 'o'},
            {"help",   no_argument,       nullptr, 'h'},
            {nullptr, 0,                  nullptr, '\0'}
    };
    while ((gotopt = getopt_long(argc, argv, "sqo:h", long_opts, &optionIdx)) != -1) {
        myOpt thisArg;
        optCnt ++;
        switch (gotopt) {
            case 's': {
                thisArg.optName = 's';
                break;
            }
            case 'q': {
                thisArg.optName = 'q';
                break;
            }
            case 'o': {
                thisArg.optName = 'o';
                thisArg.withArg = true;
                thisArg.optArg = optarg;
                break;
            }
            case 'h': {
                printf("Lets rescue the Countess!\n[--stack | -s]\n[--queue | -q]\n"
                       "[--output | -o (M|L)]\n[--help | -h]\n");
                return 0;
            }
            default: {
                printf("Unknown argument !\n");
                exit(1);
            }
        }
        commandCache.push_back(thisArg);
    }
    // no argument
    if (optCnt == 0){
        cerr << "No argument!\n";
        exit(1);
    }
    // error checking: stack and queue together
    int flag = 0;
    bool isStackNotQueue = false,
            listOutputMode = false;
    for (auto e:commandCache){
        if (e.optName == 's'){
            isStackNotQueue = true;
            flag++;
        }
        if (e.optName == 'q'){
            flag ++;
        }
        if (e.optName == 'o' && strcmp("L",e.optArg) == 0){
            listOutputMode = true;
        }
    }
    if (flag == 2){
        cerr << "Stack or queue can only be specified once\n";
        exit(1);
    }
    if (flag == 0){
        cerr << "Stack or queue must be specified\n";
        exit(1);
    }

    // Read mode, roomNum, roomLength from input file
    string mode;
    int roomNum, roomLength;
    string temp;
    getline(cin,mode);
    getline(cin,temp);
    roomNum = stoi(temp);
    getline(cin,temp);
    roomLength = stoi(temp);
    // start point
    int startRoom = -1, startIdx = -1;
    // create rooms and castle
    vector<vector<Point>> castle;
    castle.resize(roomNum);
    for (int i = 0; i < roomNum; ++i) {
        castle[i].resize(roomLength * roomLength);
    }

    // READ INPUT
    try {
        // map reading
        if (mode == "M") {
            readInputMap(&castle, roomLength, &startRoom, &startIdx);
            //printMap(&castle,roomNum,roomLength);
        }
            // list reading
        else if (mode == "L"){
            readInputList(&castle, roomNum, roomLength, &startRoom, &startIdx);
            //printMap(&castle,roomNum,roomLength);
        }
    }
        /// error checking
    catch(Err_t err){

        if (err == IllegalInput){
            cerr << "Unknown map character\n";
            exit(1);
        }
        if (err == LRoomNotLegal){
            cerr << "Invalid room number\n";
            exit(1);
        }
        if (err == InvalidCol){
            cerr << "Invalid column number\n";
            exit(1);
        }
        if (err == InvalidRow){
            cerr << "Invalid row number\n";
            exit(1);
        }

    }
//     NO ERROR, PROCEED
//     go to stack mode
    if (isStackNotQueue) {
        int totalTiles = 1;
        stack<Point *> visitedStack;
        pos CountessPosition(0,0);
        Point *findCountess = findRoute_stack(&visitedStack, &castle, startRoom, startIdx, roomLength, roomNum,
                                              &totalTiles, &CountessPosition);
        if (findCountess == nullptr) {
            //printMap(&castle, roomNum, roomLength);
            printf("No solution, %d tiles discovered.\n", totalTiles);
            return 0;
        }
        if (!listOutputMode) {
            printMap(&castle, roomNum, roomLength);
            //printMapStack(findCountess, &castle, roomNum, startRoom, startIdx, roomLength);
        } else{
            printListStack(findCountess,&castle,roomLength);
        }
    }
    else { // queue mode
        int totalTiles = 1;
        queue<Point *> visitedQueue;

        Point *findCountess = findRoute_queue(&visitedQueue, &castle, startRoom, startIdx, roomLength, roomNum, &totalTiles);
        if (findCountess == nullptr) {
            printf("No solution, %d tiles discovered.\n", totalTiles);
            return 0;
        }
        if (!listOutputMode) {
            //printMap(&castle, roomNum, roomLength);
            printMapQueue(findCountess, &castle, roomNum, startRoom, startIdx, roomLength);
        } else{
            printListQueue(findCountess,&castle,roomLength);
        }
    }



    return 0;
}
