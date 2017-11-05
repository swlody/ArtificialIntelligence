/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#ifndef STATE_H_
#define STATE_H_

typedef struct State {
    char board[9][9];
    char player;
    char nextBoard;
    char lastBoard;
} State;

extern State* State_init(char me);

extern State* State_new(State* state, int j);

extern void State_next(State* state, int i, int j);

extern char State_winner(State* state);

extern char State_isLegalMove(State* state, int j);

extern char State_heuristic(State* state, char me);

extern void State_print(State* state, char me);

#endif
