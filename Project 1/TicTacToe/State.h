/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#ifndef STATE_H_
#define STATE_H_

typedef struct State {
    char board[9];
    char player;
    char moveNumber;
} State;

extern State* State_init();

extern State* State_next(State* state, int i);

extern void State_free(State* state);

extern char State_winner(State* state);

extern void State_print(State* state);

#endif
