#include "player.h"

char select_action(Player* p);
char select_horiz_direction(Player* p);

int main(int argc, char** argv) {
    Player p;
    sigpipe_signal();
    check_argc(argc); 
    assign_argv_values(&p, argv);
    fprintf(stdout, "!");
    fflush(stdout);
    p.lootMap = initial_setup(p.playerInfo, &(p.numRound),
            p.numCars, p.seed, p.numPlayers);
    p.lastDirection = '-'; 

    while(1) {
        enum hubMsgType curMsgType = rob_train(&p);
        if (curMsgType == YOURTURN) {
            char act = select_action(&p);
            send_message(PLAY, act);
        } else if (curMsgType == HORIZCHOICE) {
            char dir = select_horiz_direction(&p);
            send_message(SIDEWAYS, dir);
        }
    }
   
    return 0;
}

char select_action(Player* p) {
    int pID = p->playerIndex;
    int xPos = p->playerInfo[pID].xPos;
    int yPos = p->playerInfo[pID].yPos;
   
    if (p->lootMap[xPos][yPos] > 0) {
        // loot here
        return '$';
    } else {
        return 'h';
    }
}

char select_horiz_direction(Player* p) {
    int xPos = p->playerInfo[p->playerIndex].xPos;

    if (xPos == 0) {
        p->lastDirection = '+';
    } else if (xPos == p->numCars - 1) {
        p->lastDirection = '-';
    }

    return p->lastDirection;
}
