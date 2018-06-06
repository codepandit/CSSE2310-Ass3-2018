#include "player.h"

char select_action(Player* p);
char select_horiz_direction(Player* p);
char select_short_target(Player* p);
char select_long_target(Player* p);
char choose_highestid_long(Player* p);

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

/**
 * Select the player action according to the input
 * Parameters: Player
 * */
char select_action(Player* p) {
    int pID = p->playerIndex;
    int xPos = p->playerInfo[pID].xPos;
    int yPos = p->playerInfo[pID].yPos;

    if (p->lastAction != 's' && p->lastAction != 'l') {
        // try short action
        for (int i = 0; i < p->numPlayers; ++i) {
            if (i != p->playerIndex) {
                int x = p->playerInfo[i].xPos;
                int y = p->playerInfo[i].yPos;
                if (xPos == x && yPos == y) {
                    return 's';
                }
            }
        }
        // try long action
        for (int i = 0; i < p->numPlayers; ++i) {
            if (i != p->playerIndex) {
                int x = p->playerInfo[i].xPos;
                int y = p->playerInfo[i].yPos;
                if (y == 0 && yPos == 0) {
                    if (x == xPos + 1 || x == xPos - 1) {
                        return 'l';
                    }
                } else if (y == 1 && yPos == 1) {
                    if (x != xPos) {
                        return 'l';
                    }
                }
            }
        }
    }
    // try vertical action
    for (int i = 0; i < p->numPlayers; ++i) {
        if (i != p->playerIndex) {
            int x = p->playerInfo[i].xPos;
            int y = p->playerInfo[i].yPos;
            if (y != yPos && x == xPos) {
                return 'v';
            }
        }
    }
    // try loot action
    if (p->lootMap[xPos][yPos] > 0) {
        return '$';
    } 
    // default to horizontal action
    return 'h';
}

/**
 * Move the player in horizontal direction, if the player is at the either
 * end of the carriage it will move to an opposite direction
 * */
char select_horiz_direction(Player* p) {
    int xPos = p->playerInfo[p->playerIndex].xPos;

    int left = 0;
    int right = 0;
    for (int i = 0; i < p->numPlayers; ++i) {
        int x = p->playerInfo[i].xPos;
        
        if (xPos < x) {
            right++;
        } else if (x < xPos) {
            left++;
        }
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

/**
 * Select the short action on player, if the player is on same level
 * */
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

/**
 * Select the long target according to the highest ID
 * */
char select_long_target(Player* p) {
    return choose_highestid_long(p);
}

/**
 * Chose the target with the highest ID
 * */
char choose_highestid_long(Player* p) {
    int shortestLeft = p->numCars;
    int shortestRight = p->numCars;
    int highestidLeft = 0;
    int highestidRight = 0;
    int ourID = p->playerIndex;
    int ourX = p->playerInfo[ourID].xPos;
    int ourY = p->playerInfo[ourID].yPos;
    for (int i = p->numPlayers - 1; i >= 0; --i) {
        if (i != ourID) {
            int pX = p->playerInfo[i].xPos;
            int pY = p->playerInfo[i].yPos;
            if (ourY == 0 && pY == 0) {
                // bottom level
                if (pX == ourX - 1 || pX == ourX + 1) {
                    return (i + ASCIIA);
                }
            } else if (ourY == 1 && pY == 1) {
                // top level
                if (ourX < pX) {
                    // to our right
                    if (pX - ourX < shortestRight) {
                        shortestRight = pX - ourX;
                        highestidRight = i;
                    }
                } else if (pX < ourX) {
                    // to our left
                    if (ourX - pX < shortestLeft) {
                        shortestLeft = ourX - pX;
                        highestidLeft = i; 
                    }
                }
            }
        } 
    }

    if (highestidRight == highestidLeft) {
        return '-';
    } else if (highestidRight < highestidLeft) {
        return (highestidLeft + ASCIIA);
    } else if (highestidLeft < highestidRight) {
        return (highestidRight + ASCIIA);
    }

    return '-';
}
