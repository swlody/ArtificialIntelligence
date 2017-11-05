/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#include "State.h"
#include <stdlib.h>
#include <stdio.h>

State* State_init() {
    // Initial state with empty board, moveNumber 0, player 0 (x)
    State* state = (State*)malloc(sizeof(State));
    state->player = 0;
    state->moveNumber = 0;
    int i;
    // Initialize each space to -1 (representing empty)
    for(i=0; i<9; i++) {
        state->board[i] = -1;
    }
    return state;
}

State* State_next(State* state, int i) {
    // Return subsequent state of input state given a legal move
    State* nextState = (State*)malloc(sizeof(State));
    // Swap current player from input state
    nextState->player = !state->player;
    // Increment move number from input state
    nextState->moveNumber = state->moveNumber + 1;
    int m;
    // Copy board from input state
    for(m=0; m<9; m++) {
        nextState->board[m] = state->board[m];
    }
    // Assume move is legal and move input state player to space on board
    nextState->board[i] = state->player;
    return nextState;
}

void State_free(State* state) {
    free(state);
}

char State_winner(State* state) {
    // Return 1 if winning state, 0 for tie, -1 for game not yet over
    // First check to make sure first of checked squares is not empty (board[i] != -1)
    // Then check to make sure 3 in a row are the same (still not negative one)
    if(state->board[0] != -1 && state->board[0] == state->board[1] && state->board[1] == state->board[2]) return 1;
    if(state->board[3] != -1 && state->board[3] == state->board[4] && state->board[4] == state->board[5]) return 1;
    if(state->board[6] != -1 && state->board[6] == state->board[7] && state->board[7] == state->board[8]) return 1;
    if(state->board[0] != -1 && state->board[0] == state->board[3] && state->board[3] == state->board[6]) return 1;
    if(state->board[1] != -1 && state->board[1] == state->board[4] && state->board[4] == state->board[7]) return 1;
    if(state->board[2] != -1 && state->board[2] == state->board[5] && state->board[5] == state->board[8]) return 1;
    if(state->board[0] != -1 && state->board[0] == state->board[4] && state->board[4] == state->board[8]) return 1;
    if(state->board[2] != -1 && state->board[2] == state->board[4] && state->board[4] == state->board[6]) return 1;
    // If all the checks failed and is the final move, declare a tie
    if(state->moveNumber == 9) return 0;
    return -1;
}

void State_print(State* state) {
    // state->board[0] == -1 ? '1' : (state->board[0] ? 'o' : 'x')
    // If board is empty (print number) else (if x print x else print o)
    // Relies on state->board[0] return 1 (true) for o and 0 (false) for x
    printf("%c%s%c%s%c\n", state->board[0] == -1 ? '1' : (state->board[0] ? 'o' : 'x'), " | ", state->board[1] == -1 ? '2' : (state->board[1] ? 'o' : 'x'), " | ", state->board[2] == -1 ? '3' : (state->board[2] ? 'o' : 'x'));
    printf("---------\n");
    printf("%c%s%c%s%c\n", state->board[3] == -1 ? '4' : (state->board[3] ? 'o' : 'x'), " | ", state->board[4] == -1 ? '5' : (state->board[4] ? 'o' : 'x'), " | ", state->board[5] == -1 ? '6' : (state->board[5] ? 'o' : 'x'));
    printf("---------\n");
    printf("%c%s%c%s%c\n", state->board[6] == -1 ? '7' : (state->board[6] ? 'o' : 'x'), " | ", state->board[7] == -1 ? '8' : (state->board[7] ? 'o' : 'x'), " | ", state->board[8] == -1 ? '9' : (state->board[8] ? 'o' : 'x'));
}
