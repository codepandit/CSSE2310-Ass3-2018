#include "helper.h"

/* Player exit codes */
enum ecode {
    OK = 0,
    BADARG,
    BADNUMPLAY,
    BADID,
    BADNUMCAR,
    BADSEED,
    BADCOM,
};

/* Player Info */
typedef struct {
    int numPlayers;
    int playerIndex;
    int seed;
    int numCars;
    int** lootMap;
    int numRound;
    PlayerStatus playerInfo[MAXPLAYERS];
    int lastAction;
    int lastDirection;
} Player;

void exit_status(enum ecode e);
void check_argc(int argc);
void assign_argv_values(Player* p, char** argv);
enum hubMsgType process_msg(char* msg, Player* p);
void send_message(enum playerMsgType msgType, int param);
int rob_train(Player* p);
void print_stderr_message(enum hubMsgType, char one, char two, char three);
void ignore_sigpipe(int sigID);
void sigpipe_signal();
