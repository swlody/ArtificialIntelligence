/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#include "State.h"
#include <stdio.h>
#include <time.h>

#define max(x,y) (x>y) ? x : y

// TODO Add some randomness to moves of equivalent value - shuffle move first?

// chars are used for most values instead of ints for smaller memory footprint and potentially faster computation
// (not sure how much of a difference this actually makes)

int states;

char alphabeta(State* state, char alpha, char beta, char color) {
    char winner = State_winner(state);
    if(winner != -1) return -winner;
    char bestValue = -127;
    int i;
    // For all possible moves, if the move is legal (not in an occupied space), explore it with negamax
    for(i=0; i<9; i++) {
        // Check move is legal (square unoccupied)
        if(state->board[i] == -1) {
            states++;
            State* explore = State_next(state, i);
            // Swap color for next call
            char v = -alphabeta(explore, -beta, -alpha, -color);
            // Free memory once state is no longer needed
            State_free(explore);
            // Set bestValue to v if v > bestValue
            bestValue = max(bestValue, v);
            alpha = max(alpha, v);
            if(alpha >= beta) break;
        }
    }
    return bestValue;
}

char getBestMove(State* state) {
    char move = -1;
    char bestValue = -127;
    char alpha = -127;
    char beta = 127;
    int i;
    for(i=0; i<9; i++) {
        if(state->board[i] == -1) {
            states++;
            State* explore = State_next(state, i);
            char v = -alphabeta(explore, -beta, -alpha, -1);
            State_free(explore);
            if(v > bestValue) {
                move = i;
                bestValue = v;
            }
            alpha = max(alpha, v);
            if(alpha >= beta) break;
        }
    }
    return move;
}

void newGame(char me) {
	// Initialize current state to empty board
    State* currentState = State_init();
    char winner = -1;
    // If State_winner() returns -1, the game is not yet over
    while(winner == -1) {
        if(currentState->player == me) {
            struct timespec start, end;
            clock_gettime(CLOCK_REALTIME, &start);
            char i = getBestMove(currentState);
            clock_gettime(CLOCK_REALTIME, &end);
            printf("%lu us\n", (end.tv_nsec - start.tv_nsec)/1000);
            printf("%d states explored.\n", states);
            printf("%s%d%c\n", "I move to space ", i+1, '.');
            currentState = State_next(currentState, i);
        } else {
            State_print(currentState);
            printf("Where would you like to move?\n");
            int i;
            // Ensure an integer is entered, then ensure integer is between 1 and 9, then ensure move is legal (space is empty)
            while(scanf(" %d", &i) == EOF || i<1 || i>9 || currentState->board[i-1] != -1) {
                printf("Improper input, please enter a number 1-9 that corresponds to an empty space.\n");
            }
            currentState = State_next(currentState, i-1);
        }
        // Check if game is over
        winner = State_winner(currentState);
    }
    if(winner) {
        // Whoever's turn it is lost
        printf("%s won!\n", currentState->player == me ? "You" : "I");
    } else {
        printf("Draw!\n");
    }
}

int main(int argc, char* argv[]) {
    printf("Welcome to TicTacToe!\n");
    printf("Move by entering a number 1-9.\n");
    char input = 'y';
    while(input == 'y' || input == 'Y') {
        printf("Would you like to play X or O?\n");
        char player;
        // Ensure a character is entered, then ensure it is equal to x or y
        while(scanf(" %c", &player) == EOF || (player != 'x' && player != 'X' && player != 'o' && player != 'O')) {
            printf("Improper input, please enter either \"X\" or \"O\".\n");
        }
        newGame(player == 'x' || player == 'X');
        printf("Would you like to play again? (Y/N)\n");
        // Ensure a character is entered, then ensure it is equal to y or n
        while(scanf(" %c", &input) == EOF || (input != 'y' && input != 'Y' && input != 'n' && input != 'N')) {
            printf("Improper input, please enter either \"Y\" or \"N\".\n");
        }
    }
    printf("Goodbye.\n");
    return 0;
}
