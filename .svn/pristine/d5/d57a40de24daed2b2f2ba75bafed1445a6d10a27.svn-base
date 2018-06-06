#include "player.h"

char select_action(Player* p);
char select_horiz_direction(Player* p);
char select_short_target(Player* p);
char select_long_target(Player* p);
char choose_lowestid_long(Player* p);

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
            p.lastAction = act;
            send_message(PLAY, act);
        } else if (curMsgType == HORIZCHOICE) {
            char dir = select_horiz_direction(&p);
            p.lastDirection = dir;
            send_message(SIDEWAYS, dir);
        } else if (curMsgType == SHORTCHOICE) {
            char target = select_short_target(&p);
            send_message(TARGETSHORT, target);
        } else if (curMsgType == LONGCHOICE) {
            char target = select_long_target(&p);
            send_message(TARGETLONG, target);
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
    } else if (p->lastAction != 's' && p->lastAction != 'l') {
        for (int i = 0; i < p->numPlayers; ++i) {
            int x = p->playerInfo[i].xPos;
            int y = p->playerInfo[i].yPos;

            if (xPos == x && yPos == y && i != p->playerIndex) {
                return 's';
            }
        }
    }
    
    int top = 0;
    int bottom = 0;
    for (int i = 0; i < p->numCars; ++i) {
        top += p->lootMap[i][1];
        bottom += p->lootMap[i][0];
    }
    
    if ((yPos == 1 && top < bottom) || (yPos == 0 && bottom < top)) {
        return 'v';
    } else {
        int left = 0;
        int right = 0;
        for (int i = 0; i < xPos; ++i) {
            left += p->lootMap[i][0];
            left += p->lootMap[i][1];
        }

        for (int i = xPos + 1; i < p->numCars; ++i) {
            right += p->lootMap[i][0];
            right += p->lootMap[i][1];
        }

        if (left != right) {
            return 'h';
        }
    }

    for (int i = 0; i < p->numPlayers; ++i) {
        int x = p->playerInfo[i].xPos;
        int y = p->playerInfo[i].yPos;
        
        if (i == p->playerIndex) {
            continue;
        }        

        if (yPos == 0) {
            if (y == 0 && (x == xPos - 1 || x == xPos + 1)) {
                return 'l';
            } 
        } else if (yPos == 1) {
            if (y == 1 && x != xPos) {
                return 'l';
            }
        }
    }

    return 'v';
}

char select_horiz_direction(Player* p) {
    int xPos = p->playerInfo[p->playerIndex].xPos;

    int left = 0;
    int right = 0;
    for (int i = 0; i < xPos; ++i) {
        left += p->lootMap[i][0];
        left += p->lootMap[i][1];
    }

    for (int i = xPos + 1; i < p->numCars; ++i) {
        right += p->lootMap[i][0];
        right += p->lootMap[i][1];
    }

    if (left < right) {
        if (xPos != p->numCars - 1) {
            p->lastDirection = '+';
        } else {
            p->lastDirection = '-';
        }
    } else if (right < left) {
        if (xPos != 0) {
            p->lastDirection = '-';
        } else {
            p->lastDirection = '+';
        }
    } else if (right == left) {
        if (xPos != 0) {
            p->lastDirection = '-';
        } else {
            p->lastDirection = '+';
        }
    }
    return p->lastDirection;
}

char select_short_target(Player* p) {
    int xPos = p->playerInfo[p->playerIndex].xPos;
    int yPos = p->playerInfo[p->playerIndex].yPos;
    int highest = -1;
    for (int i = 0; i < p->numPlayers; ++i) {
        if (i != p->playerIndex) {
            if (xPos == p->playerInfo[i].xPos 
                && yPos == p->playerInfo[i].yPos) {
                highest = i;
            }
        } 
    }

    if (highest == -1) {
        return '-';
    } else {
        return (highest + ASCIIA);
    }
}

char select_long_target(Player* p) {
    return choose_lowestid_long(p);
}

char choose_lowestid_long(Player* p) {
    int shortestLeft = p->numCars;
    int shortestRight = p->numCars;
    int lowestidLeft = p->numPlayers;
    int lowestidRight = p->numPlayers;
    int ourID = p->playerIndex;
    int ourX = p->playerInfo[ourID].xPos;
    int ourY = p->playerInfo[ourID].yPos;
    for (int i = 0; i < p->numPlayers; ++i) {
        if (i != ourID) {
            int pX = p->playerInfo[i].xPos;
            int pY = p->playerInfo[i].yPos;
            if (ourY == 0 && pY == 0) {
                // bottom level
                if (pX == ourX - 1 || pX == ourX + 1) {
                    return (i + ASCIIA);
                }
            } else if (ourY == 1 && pY == 1){
                // top level
                if (ourX < pX) {
                    // to our right
                    if (pX - ourX < shortestRight) {
                        shortestRight = pX - ourX;
                        lowestidRight = i;
                    }
                } else if (pX < ourX) {
                    // to our left
                    if (ourX - pX < shortestLeft) {
                        shortestLeft = ourX - pX;
                        lowestidLeft = i; 
                    }
                }
            }
        } 
    }

    if (lowestidRight == lowestidLeft) {
        return '-';
    } else if (lowestidRight < lowestidLeft) {
        return (lowestidRight + ASCIIA);
    } else if (lowestidLeft < lowestidRight) {
        return (lowestidLeft + ASCIIA);
    }

    return '-';
}
