#include "helper.h"

/**
 * Count the number of digit
 * Parameter: Integer number
 * Return: count
 * */
int digit_count(int num) {
    int count = 0;
    
    while (num != 0) {
        num /= 10;
        count++;
    }

    return count;
}

/**
 * Check the valid player
 * Parameters: Player Id and Player Count
 * */
int check_valid_player(char playerID, int playerCount) {
    if (playerID - 65 < 0 || (playerID - 65) >= playerCount) {
        // invalid player
        return 0;
    } else {
        return 1;
    }
}

/**
 * Check the valid action send by player
 * Parameter: char action
 * */
int check_valid_action(char action) {
    if (action != 'l' && action != 's' && action != 'h'
            && action != '$' && action != 'v') {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Check if the horizontal move action is illegal
 * Parameters: player status, info, ID, direction and number of carriages
 * */
int check_illegal_horiz(PlayerStatus playerInfo[MAXPLAYERS], 
        int playerID, char direction, int numCars) {
    int xPos = playerInfo[playerID].xPos;
    
    if (direction != '+' && direction != '-') {
        return -1;
    } else if (xPos == 0 && direction == '-') {
        return -2;
    } else if (xPos == numCars - 1 && direction == '+') {
        return -2;
    } else {
        return 1;
    }
}

/**
 * This function execute the loot if any loot available
 * Parameters: player status, Info, ID and loot
 * */
int execute_loot_action(int** loot, PlayerStatus playerInfo[MAXPLAYERS],
        int playerID) {
    int xPos = playerInfo[playerID].xPos;
    int yPos = playerInfo[playerID].yPos;
    
    if (loot[xPos][yPos] > 0) {
        loot[xPos][yPos]--;
        playerInfo[playerID].loot++;
    }

    return 1;
}

/**
 * Execute the dried out action, if player took 3 hits
 * Parametes: Player status, Info and ID
 * */
int execute_dried_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID) {
    playerInfo[playerID].hits = 0;
    return 1; 
}

/**
 * excute the horizontal move action, If player is the either end of the
 * carriage it will move opposite direction other wise keep moving at 
 * previous direction
 * Parameters: Player status, ID, direction and number of carriages
 * */
int execute_horiz_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID,
        char direction, int numCarriages) {
    int xPos = playerInfo[playerID].xPos;

    if (xPos == 0 && direction == '-') {
        return 0;
    } else if (xPos == numCarriages - 1 && direction == '+') {
        return 0;
    } else if (direction == '+') {
        xPos++;
        playerInfo[playerID].xPos = xPos;
    } else if (direction == '-') {
        xPos--;
        playerInfo[playerID].xPos = xPos;
    }

    return 1;
}

/**
 * Execute the vertical move action, if player at 0 level move to 1
 * if on level stays there
 * Parmaters: Player status and ID
 * */
int execute_vert_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID) 
{ 
    int yPos = playerInfo[playerID].yPos;
    yPos = !yPos;
    playerInfo[playerID].yPos = yPos;

    return 1;
}

/**
 * Execute the long action, target player will take the hit
 * Parameters: player status, ID and target
 * */
int execute_long_action(PlayerStatus playerInfo[MAXPLAYERS], int playerID,
        int target) {
    
    if (target + 65 != '-') {
        playerInfo[target].hits++; 
    }

    return 1;
}

/**
 * execute short action, if the target is on the same level and have some loot
 * it will drop 1 loot. If target player doesn't have any loot nothing will 
 * happen
 * Parameters: loot, player status, ID and target
 * */
int execute_short_action(int** loot, PlayerStatus playerInfo[MAXPLAYERS], 
        int playerID, int target) {
    int xPos = playerInfo[playerID].xPos;
    int yPos = playerInfo[playerID].yPos;
    if (target + 65 == '-') {

    } else if (playerInfo[target].loot > 0) {
        playerInfo[target].loot--;
        loot[xPos][yPos]++; 
    }

    return 1;
}

/**
 * This function will initialise the player and game setup
 * */
int** initial_setup(PlayerStatus playerInfo[MAXPLAYERS], 
        int* numRound, int numCars, int seed, int numPlayers) {
    int totalLoot = 0, lootPosX = 0, lootPosY = 0;
    int** loot;

    // initialise the rounds
    *numRound = 0;

    // initialise the 2D array with all 0's
    memset(playerInfo, 0, sizeof(PlayerStatus) * MAXPLAYERS);

    // allocate the memory to store the position of loot
    loot = malloc(sizeof(int*) * numCars);
    for (int i = 0; i < numCars; ++i) {
        loot[i] = malloc(sizeof(int) * CARRIAGELEVELS);
    }

    for (int i = 0; i < numCars; ++i) {
        for (int j = 0; j < CARRIAGELEVELS; ++j) {
            loot[i][j] = 0;
        }
    }

    // initialise the loot
    totalLoot = ((seed % 4) + 1) * numCars;

    // distribute the loot
    for (int i = 0; i < totalLoot; ++i) {
        if (i == 0) {
            lootPosX = ceil((numCars / 2.0));
            lootPosY = 0;
        } else {
            lootPosX = (lootPosX + (seed % 101)) % numCars;
            lootPosY = (lootPosY + (seed % 2)) % 2;
        }
        loot[lootPosX][lootPosY]++;
    }

    // initialise player positions 
    for (int i = 0; i < numPlayers; ++i) {
        int xPos = (int)(i % numCars);
        int yPos = 0;

        playerInfo[i].xPos = xPos;
        playerInfo[i].yPos = yPos;
        playerInfo[i].loot = 0;
        playerInfo[i].hits = 0;
    }


    return loot;
}

/**
 * This function will get the message from the stream
 * Parameters: max lenght, msg and stream
 * */
int get_message(int maxLength, char* msg, FILE* stream) {
    char c;
    int i = 0;
    while (((c = fgetc(stream)) != '\n') && (c != EOF)) {
        if (i < maxLength) {
            msg[i] = c;
            msg[i + 1] = '\0';
        } else {
            return -1;
        }
        i++;
    }

    if (c == EOF) {
        return -2;
    }

    return 0;
}

/**
 * Functionn will check the valid number
 * */
int check_valid_number(char* argument) {
    int argLength = strlen(argument);

    for (int i = 0; i < argLength; ++i) {
        if (argument[i] < 48 || argument[i] > 57) {
            return 0;
        }
    }
    return 1;
}
