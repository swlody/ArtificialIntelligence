/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#include "State.h"
#include <stdlib.h>
#include <stdio.h>

#define ANSI_COLOR_BRIGHT_RED "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_BLUE "\x1b[32;1m"
#define ANSI_COLOR_RESET "\x1b[0m"

State* State_init(char me) {
    State* state = (State*)malloc(sizeof(State));
    // First player is X
    state->player = 0;
    // The board that was just moved in, used for win function to avoid looping through every board
    state->lastBoard = -1;
    // If it's my turn, choose a random board to move in, otherwise the player will choose
    state->nextBoard = me ? -1 : rand() % 9;
    int i;
    // Initialize board to -1 (empty)
    for(i=0; i<9; i++) {
        int j;
        for(j=0; j<9; j++)
            state->board[i][j] = -1;
    }
    return state;
}

State* State_new(State* state, int space) {
    State* nextState = (State*)malloc(sizeof(State));
    // Swap players
    nextState->player = !state->player;
    // Board to be moved into in next move is space that was moved into for this move
    nextState->lastBoard = state->nextBoard;
    nextState->nextBoard = space;
    int m;
    // Copy board to new state
    for(m=0; m<9; m++) {
        int n;
        for(n=0; n<9; n++)
            nextState->board[m][n] = state->board[m][n];
    }
    // Replace move space with the player - error checking (making sure space is empty) done outside this method
    nextState->board[state->nextBoard][space] = state->player;
    return nextState;
}

void State_next(State* state, int board, int space) {
    state->lastBoard = state->nextBoard;
    state->nextBoard = space;
    // Swap players
    state->board[board][space] = state->player;
    state->player = !state->player;
}

char State_winner(State* state) {
    // lastBoard == -1 means first move
    if(state->lastBoard == -1)
        return -1;
    // Returns 1 if the state is a winning one
    // If 1 is not empty, and 1 == 2, and 2 == 3, the spaces are filled by the same letter in a row, so someone won
    if(state->board[(int)state->lastBoard][0] != -1 
            && state->board[(int)state->lastBoard][0] == state->board[(int)state->lastBoard][1] 
            && state->board[(int)state->lastBoard][1] == state->board[(int)state->lastBoard][2]) 
        return 1;
    if(state->board[(int)state->lastBoard][3] != -1 
            && state->board[(int)state->lastBoard][3] == state->board[(int)state->lastBoard][4] 
            && state->board[(int)state->lastBoard][4] == state->board[(int)state->lastBoard][5]) 
        return 1;
    if(state->board[(int)state->lastBoard][6] != -1 
            && state->board[(int)state->lastBoard][6] == state->board[(int)state->lastBoard][7] 
            && state->board[(int)state->lastBoard][7] == state->board[(int)state->lastBoard][8]) 
        return 1;
    if(state->board[(int)state->lastBoard][0] != -1 
            && state->board[(int)state->lastBoard][0] == state->board[(int)state->lastBoard][3] 
            && state->board[(int)state->lastBoard][3] == state->board[(int)state->lastBoard][6]) 
        return 1;
    if(state->board[(int)state->lastBoard][1] != -1 
            && state->board[(int)state->lastBoard][1] == state->board[(int)state->lastBoard][4] 
            && state->board[(int)state->lastBoard][4] == state->board[(int)state->lastBoard][7]) 
        return 1;
    if(state->board[(int)state->lastBoard][2] != -1 
            && state->board[(int)state->lastBoard][2] == state->board[(int)state->lastBoard][5] 
            && state->board[(int)state->lastBoard][5] == state->board[(int)state->lastBoard][8]) 
        return 1;
    if(state->board[(int)state->lastBoard][0] != -1 
            && state->board[(int)state->lastBoard][0] == state->board[(int)state->lastBoard][4] 
            && state->board[(int)state->lastBoard][4] == state->board[(int)state->lastBoard][8]) 
        return 1;
    if(state->board[(int)state->lastBoard][2] != -1 
            && state->board[(int)state->lastBoard][2] == state->board[(int)state->lastBoard][4] 
            && state->board[(int)state->lastBoard][4] == state->board[(int)state->lastBoard][6]) 
        return 1;
    // Return 0 for tie
    int i;
    for(i=0; i<9; i++) {
        if(state->board[(int)state->lastBoard][i] != -1) 
            break;
    }
    // 0 for tie, else -1
    return -(i!=9);
}

char State_heuristic(State* state, char me) {
    // Count unblocked 2-in-a-rows for player being maximized
    char count = 0;
    int i;
    for(i=0; i<9; i++) {
        if(state->board[i][0] != -1 
                && state->board[i][2] == -1 
                && state->board[i][0] == state->board[i][1]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][0] != -1
                && state->board[i][1] == -1
                && state->board[i][0] == state->board[i][2]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][0] != -1
                && state->board[i][6] == -1
                && state->board[i][0] == state->board[i][3]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][0] != -1
                && state->board[i][8] == -1
                && state->board[i][0] == state->board[i][4]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][0] != -1
                && state->board[i][3] == -1
                && state->board[i][0] == state->board[i][6]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][0] != -1
                && state->board[i][4] == -1
                && state->board[i][0] == state->board[i][8]) 
            count+= state->board[i][0] == me ? 1 : -1;
        if(state->board[i][1] != -1
                && state->board[i][0] == -1
                && state->board[i][1] == state->board[i][2]) 
            count+= state->board[i][1] == me ? 1 : -1;
        if(state->board[i][1] != -1
                && state->board[i][7] == -1
                && state->board[i][1] == state->board[i][4]) 
            count+= state->board[i][1] == me ? 1 : -1;
        if(state->board[i][1] != -1
                && state->board[i][4] == -1
                && state->board[i][1] == state->board[i][7]) 
            count+= state->board[i][1] == me ? 1 : -1;
        if(state->board[i][2] != -1
                && state->board[i][6] == -1
                && state->board[i][2] == state->board[i][4]) 
            count+= state->board[i][2] == me ? 1 : -1;
        if(state->board[i][2] != -1
                && state->board[i][8] == -1
                && state->board[i][2] == state->board[i][5]) 
            count+= state->board[i][2] == me ? 1 : -1;
        if(state->board[i][2] != -1
                && state->board[i][4] == -1
                && state->board[i][2] == state->board[i][6]) 
            count+= state->board[i][2] == me ? 1 : -1;
        if(state->board[i][2] != -1
                && state->board[i][5] == -1
                && state->board[i][2] == state->board[i][8]) 
            count+= state->board[i][2] == me ? 1 : -1;
        if(state->board[i][3] != -1
                && state->board[i][5] == -1
                && state->board[i][3] == state->board[i][4]) 
            count+= state->board[i][3] == me ? 1 : -1;
        if(state->board[i][3] != -1
                && state->board[i][4] == -1
                && state->board[i][3] == state->board[i][5]) 
            count+= state->board[i][3] == me ? 1 : -1;
        if(state->board[i][3] != -1
                && state->board[i][0] == -1
                && state->board[i][3] == state->board[i][6]) 
            count+= state->board[i][3] == me ? 1 : -1;
        if(state->board[i][4] != -1
                && state->board[i][3] == -1
                && state->board[i][4] == state->board[i][5]) 
            count+= state->board[i][4] == me ? 1 : -1;
        if(state->board[i][4] != -1
                && state->board[i][2] == -1
                && state->board[i][4] == state->board[i][6]) 
            count+= state->board[i][4] == me ? 1 : -1;
        if(state->board[i][4] != -1
                && state->board[i][1] == -1
                && state->board[i][4] == state->board[i][7]) 
            count+= state->board[i][4] == me ? 1 : -1;
        if(state->board[i][4] != -1
                && state->board[i][0] == -1
                && state->board[i][4] == state->board[i][8]) 
            count+= state->board[i][4] == me ? 1 : -1;
        if(state->board[i][5] != -1
                && state->board[i][2] == -1
                && state->board[i][5] == state->board[i][8]) 
            count+= state->board[i][5] == me ? 1 : -1;
        if(state->board[i][6] != -1
                && state->board[i][8] == -1
                && state->board[i][6] == state->board[i][7]) 
            count+= state->board[i][6] == me ? 1 : -1;
        if(state->board[i][6] != -1
                && state->board[i][7] == -1
                && state->board[i][6] == state->board[i][8]) 
            count+= state->board[i][6] == me ? 1 : -1;
        if(state->board[i][7] != -1
                && state->board[i][6] == -1
                && state->board[i][7] == state->board[i][8]) 
            count+= state->board[i][7] == me ? 1 : -1;
    }
    return count;
}

void State_print(State* state, char me) {
    // This loop is weird
    int q;
    for(q=0; q<9; q+=3) {
        int j;
        for(j=0; j<9; j+=3) {
            int i;
            for(i=0; i<3; i++) {
                printf(" %s%c%s|%s%c%s|%s%c%s", 
                    // Print empty string (no color) if space is empty (== -1), else use red for me (computer) or blue for player
                    state->board[q+i][j+0] == -1 ? "" : (state->board[q+i][j+0] == me ? ANSI_COLOR_BRIGHT_RED : ANSI_COLOR_BRIGHT_BLUE), 
                        // If space is empty, print whatever number represents the square, otherwise print X or O
                        state->board[q+i][j+0] == -1 ? '0'+j+1 : (state->board[q+i][j+0] == 0 ? 'X' : 'O'), 
                        // Reset color
                        ANSI_COLOR_RESET, 
                    state->board[q+i][j+1] == -1 ? "" : (state->board[q+i][j+1] == me ? ANSI_COLOR_BRIGHT_RED : ANSI_COLOR_BRIGHT_BLUE), 
                        state->board[q+i][j+1] == -1 ? '0'+j+2 : (state->board[q+i][j+1] == 0 ? 'X' : 'O'), 
                        ANSI_COLOR_RESET,
                    state->board[q+i][j+2] == -1 ? "" : (state->board[q+i][j+2] == me ? ANSI_COLOR_BRIGHT_RED : ANSI_COLOR_BRIGHT_BLUE), 
                        state->board[q+i][j+2] == -1 ? '0'+j+3 : (state->board[q+i][j+2] == 0 ? 'X' : 'O'), 
                        ANSI_COLOR_RESET);
                if(i%3 != 2) 
                    printf(" ||");
            }
            printf("\n");
            if(j!=6) 
                printf("-------------------------\n");
        }
        if(q!=6) 
            printf("=========================\n");
    }
}
