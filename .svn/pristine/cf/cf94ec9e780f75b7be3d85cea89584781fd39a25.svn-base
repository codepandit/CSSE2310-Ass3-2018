#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <fcntl.h>

#define MINPLAYERS 2
#define MAXPLAYERS 26
#define MINCARRIAGES 3
#define CARRIAGELEVELS 2
#define MAXROUNDS 15
#define MAXHITS 3
#define ASCIIA 65

enum hubMsgType {
    GAMEOVER = 0,
    ROUND,
    YOURTURN,
    ORDERED,
    EXECUTE,
    HORIZCHOICE,
    SHORTCHOICE,
    LONGCHOICE,
    HORIZMOVE,
    VERMOVE,
    LONG,
    SHORT,
    LOOTED,
    DRIED,
};

enum playerMsgType {
    PLAY = 0,
    SIDEWAYS,
    TARGETSHORT,
    TARGETLONG,
};

typedef struct {
    int xPos;
    int yPos;
    int hits;
    int loot;
} PlayerStatus;

int digit_count(int num);
int check_valid_player(char playerID, int playerCount);
int check_valid_action(char action);
int check_illegal_horiz(PlayerStatus playerInfo[MAXPLAYERS],
        int playerID, char direction, int numCars);
int execute_horiz_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID,
        char direction, int numCarriages);
int execute_vert_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID);
int execute_loot_action(int** loot, PlayerStatus playerInfo[MAXPLAYERS], 
        int playerID);
int execute_short_action(int** loot, PlayerStatus playerInfo[MAXPLAYERS], 
        int playerID, int target);
int execute_long_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID, 
        int target);
int execute_dried_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID);
int** initial_setup(PlayerStatus playerInfo[MAXPLAYERS], 
        int* numRound, int numCars, int seed, int numPlayers);
int get_message(int maxLength, char* msg, FILE* stream);
int check_valid_number(char* argument);
