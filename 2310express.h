#include "helper.h"

#define MAXARGC 29

enum ecode {
    NORMAL = 0,
    WRONGARGCOUNT,
    BADARG,
    BADSTART,
    CLIENTDC,
    PROTERROR,
    ILLMOVE,
    SIGINTCAUGHT = 9,
};

typedef struct {
    int playerCount;
    int seed;
    int numCars;
    int** lootMap;
    PlayerStatus playerInfo[MAXPLAYERS];
    char* playerPrograms[MAXPLAYERS];
    int numRound;
    int cp[MAXPLAYERS][2];
    int pc[MAXPLAYERS][2];
    FILE* pcStreams[MAXPLAYERS];
    FILE* cpStreams[MAXPLAYERS];
    int playersThatStarted;
    pid_t childrenProcess[MAXPLAYERS];
    int allStarted;
    int sigint;
} Hub;

void check_argc(int argc, Hub* h);
void exit_status(enum ecode value, Hub* h);
void assign_argv_values(Hub* h, char** argv, int argc);
void read_from_child(int file);
void write_to_child(int file);
void assign_programs(Hub* h, char** argv, int argc);
void make_child_processes(Hub* h);
void make_pipes(Hub* h);
void play_game(Hub* h);
void send_message(enum hubMsgType, FILE* stream, int param1, int param2);
void handle_response(Hub* h, int playerID, int* param, 
        enum playerMsgType expectedMessage);
void update_game_state(Hub* h, int playerID, char a, int extraInfo);
void round_end_message(Hub* h, int winners);
void send_update_message(Hub* h, int j, int extraInfo, char a);
void get_additional_info(char action, int j, Hub* h, int* extraInfo);
void signals();
void ignore_pipe_signal(int sigID);
void inter(int sigID);
void dupfds(Hub* h, int i);
void setup_arg_exec(Hub* h, int i);
void close_children_players(Hub* h);
