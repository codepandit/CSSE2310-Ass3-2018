#include "player.h"

int rob_train(Player* p) {
    char msg[10];
    memset(msg, 0, 10 * sizeof(char));
    int ret = get_message(9, msg, stdin); 
    if (ret < 0) {
        exit_status(BADCOM);
    }
    
    enum hubMsgType got = process_msg(msg, p);
    
    return got;
}

void check_argc(int argc) {
    if (argc < 5 || argc > 5) {
        exit_status(BADARG);
    }
}

void assign_argv_values(Player* p, char** argv) {
 
    long numofplayers, playernum, numofcars, seed;
    char* ptr;

    numofplayers = strtol(argv[1], &ptr, 10);
    playernum = strtol(argv[2], &ptr, 10);
    numofcars = strtol(argv[3], &ptr, 10);
    seed = strtol(argv[4], &ptr, 10);

    // check valid player count
    if (!check_valid_number(argv[1]) || 
            numofplayers < MINPLAYERS || 
            numofplayers > MAXPLAYERS) {
        exit_status(BADNUMPLAY);
    }
   
    // check valid player id
    if (!check_valid_number(argv[2])) {
        exit_status(BADID);
    } else if (playernum < 0 || playernum >= numofplayers) {
        exit_status(BADID);
    }

    // check valid carriages
    if (!check_valid_number(argv[3]) || numofcars < MINCARRIAGES) {
        exit_status(BADNUMCAR);
    }

    // check valid seed
    if (!check_valid_number(argv[4])) {
        exit_status(BADSEED);  
    } else if (seed < 0 || seed > UINT_MAX) {
        exit_status(BADSEED);
    }
 
    p->numPlayers = numofplayers;
    p->playerIndex = playernum;
    p->numCars = numofcars;
    p->seed = seed;
}

enum hubMsgType process_msg(char* msg, Player* p){
    if (strncmp(msg, "yourturn", 8) == 0 && strlen(msg) == 8) {
        return YOURTURN;
    }
    if (strncmp(msg, "round", 5) == 0 && strlen(msg) == 5) {
        if (p->numRound < MAXROUNDS) {
            p->numRound++;//or maybe use increment function
        }
        return ROUND;
    }
    if (strncmp(msg, "ordered", 7) == 0 && strlen(msg) == 9) {
        /* X - the player who submitted the action
         * V - the action they chose
         * */
        char X = msg[7];
        char V = msg[8];
        
        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        } else if (!check_valid_action(V)) {
            exit_status(BADCOM);
        }
        print_stderr_message(ORDERED, X, V, 0);
        return ORDERED;
    }
    if (strncmp(msg, "hmove", 5) == 0 && strlen(msg) == 7) {
        /* X - the player who moved
         * Y - move left or right
         * */
        char X = msg[5];
        char Y = msg[6];

        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        } else if (check_illegal_horiz(p->playerInfo, X - 65, Y, 
                p->numCars) < 0) {
            exit_status(BADCOM);
        }
        

        execute_horiz_action(p->playerInfo, X - 65, Y, p->numCars);
        print_stderr_message(HORIZMOVE, X, p->playerInfo[X - 65].xPos, 
                p->playerInfo[X - 65].yPos);
        return HORIZMOVE;
    }
    if (strncmp(msg, "vmove", 5) == 0 && strlen(msg) == 6) {
        /* X - the player who moved * */
        char X = msg[5];

        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        }

        execute_vert_action(p->playerInfo, X - 65);
        print_stderr_message(VERMOVE, X, p->playerInfo[X - 65].xPos, 
                p->playerInfo[X - 65].yPos);
        return VERMOVE;
    }
    if (strncmp(msg, "long", 4) == 0 && strlen(msg) == 6) {
        /* X - the player who targeted
         * T - the player who was targeted
         * */
        char X = msg[4];
        char T = msg[5];

        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        } else if (!check_valid_player(T, p->numPlayers) && 
                T != '-') {
            exit_status(BADCOM);
        }
 
        execute_long_action(p->playerInfo, X - 65, T - 65);

        print_stderr_message(LONG, X, T, p->playerInfo[T - 65].hits);
        return LONG;
    }
    if (strncmp(msg, "short", 5) == 0 && strlen(msg) == 7) {
        /* X - the player who targeted
         * T - the player who was targeted
         * */
        char X = msg[5];
        char T = msg[6];
        
        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        } else if (!check_valid_player(T, p->numPlayers) && 
                T != '-') {
            exit_status(BADCOM);
        }

        if (p->playerInfo[T - 65].loot == 0) {
            print_stderr_message(SHORT, X, T, 0);
        } else {
            print_stderr_message(SHORT, X, T, 1);
        }

        execute_short_action(p->lootMap, p->playerInfo, X - 65, 
                T - 65);
        
       

        return SHORT;
    } 
    if (strncmp(msg, "looted", 6) == 0 && strlen(msg) == 7) {
        /* X - the player who was looted or who looted * */
        char X = msg[6];

        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        }

        int xPos = p->playerInfo[X - 65].xPos;
        int yPos = p->playerInfo[X - 65].yPos;

        if (p->lootMap[xPos][yPos] == 0) {
            print_stderr_message(LOOTED, X, p->playerInfo[X - 65].loot, 0);
        } else {
            print_stderr_message(LOOTED, X, p->playerInfo[X - 65].loot + 1, 1);
        }
        execute_loot_action(p->lootMap, p->playerInfo, X - 65);
        
        return LOOTED;
    }
    if (strncmp(msg, "driedout", 8) == 0 && strlen(msg) == 9) {
        /* X - the player who took >= 3 hits and dried off * */
        char X = msg[8];

        if (!check_valid_player(X, p->numPlayers)) {
            exit_status(BADCOM);
        }

        execute_dried_action(p->playerInfo, X - 65);
        if (X - 65 == p->playerIndex) {
            p->lastAction = 'd';
        }
        print_stderr_message(DRIED, X, 0, 0);
        return DRIED;
    }
    if (strncmp(msg, "game_over", 9) == 0 && strlen(msg) == 9) {
        // game over
        exit_status(OK);
        return GAMEOVER;
    }
    if (strncmp(msg, "execute", 7) == 0 && strlen(msg) == 7) {
        // execute phase
        return EXECUTE;
    }
    if (strncmp(msg, "h?", 2) == 0 && strlen(msg) == 2) {
        // decide horiz movement
        return HORIZCHOICE;
    }
    if (strncmp(msg, "l?", 2) == 0 && strlen(msg) == 2) {
        // decide long target
        return LONGCHOICE;
    }
    if (strncmp(msg, "s?", 2) == 0 && strlen(msg) == 2) {
        // decide short target
        return SHORTCHOICE;
    }
    exit_status(BADCOM);
    return GAMEOVER;
}

void exit_status(enum ecode e) { 
    switch (e) {
        case OK:
            break;
        case BADARG:
            fprintf(stderr, "Usage: player pcount myid width seed\n");
            break;
        case BADNUMPLAY:
            fprintf(stderr, "Invalid player count\n");
            break;
        case BADID:
            fprintf(stderr, "Invalid player ID\n");
            break;
        case BADNUMCAR:
            fprintf(stderr, "Invalid width\n");
            break;
        case BADSEED:
            fprintf(stderr, "Invalid seed\n");
            break;
        case BADCOM:
            fprintf(stderr, "Communication Error\n");
            break;
    }

    exit(e);
}

void send_message(enum playerMsgType msgType, int param) {
    if (msgType == PLAY) {
        fprintf(stdout, "play%c\n", param);
    } else if (msgType == SIDEWAYS) {
        fprintf(stdout, "sideways%c\n", param);
    } else if (msgType == TARGETSHORT) {
        fprintf(stdout, "target_short%c\n", param);
    } else if (msgType == TARGETLONG) {
        fprintf(stdout, "target_long%c\n", param);
    }
    fflush(stdout);
}


void print_stderr_message(enum hubMsgType recvMsg, char one, char two, char three) {
    if (recvMsg == LOOTED) {
        if (three != 0) {
            fprintf(stderr, "%c picks up loot (they now have %d)\n", one, two);
        } else {
            fprintf(stderr, "%c tries to pick up loot but there isn't any\n", one);
        }
    } else if (recvMsg == ORDERED) {
        fprintf(stderr, "%c ordered %c\n", one, two);
    } else if (recvMsg == LONG) {
        if (two != '-') {
            fprintf(stderr, "%c targets %c who has %d hits\n", one, two, three);
        } else {
            fprintf(stderr, "%c has no target\n", one);
        }
    } else if (recvMsg == SHORT) {
        if (two != '-' && three != 0) {
            fprintf(stderr, "%c makes %c drop loot\n", one, two);
        } else if (two != '-' && three == 0) {
            fprintf(stderr, "%c tries to make %c drop loot they don't have\n", one, two);
        } else {
            fprintf(stderr, "%c has no target\n", one);
        }
    } else if (recvMsg == DRIED) {
        fprintf(stderr, "%c dries off\n", one);
    } else if (recvMsg == HORIZMOVE || recvMsg == VERMOVE) {
        fprintf(stderr, "%c moved to %d/%d\n", one, two, three);
    }
}

void sigpipe_signal() {
    struct sigaction pipe;
    pipe.sa_handler = ignore_sigpipe;
    sigaction(SIGPIPE, &pipe, NULL);
}

void ignore_sigpipe(int sigID) {
}
