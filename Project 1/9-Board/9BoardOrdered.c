/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

#include "State.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define max(x,y) (x>y) ? x : y

// chars are used for most values instead of ints for smaller memory footprint
// (not sure how much of a difference this actually makes)
char fullyExplored;
// Counters for # of states explored, not expected to overflow a single byte
int states;

// TODO Add some randomness to moves - shuffle moves for getBestMove?

char alphabeta(State* state, char me, char depth, char alpha, char beta, char color, char heuristic) {
    char winner = State_winner(state);
    if(depth == 0 || winner != -1) {
        if(winner == -1) {
            fullyExplored = 0;
            return 5 * color * heuristic;
        }
        return -100 * winner;
    }
    State** moves = malloc(sizeof(State)*9);
    char val[9];
    int j;
    for(j=0; j<9; j++) {
        if(State_isLegalMove(state, j)) {
            moves[j] = State_next(state, state->nextBoard, j);
            val[j] = State_heuristic(moves[j], me);
        } else {
            moves[j] = NULL;
            val[j] = -127;
        }
    }
    for(j=1; j<9; j++) {
        int i = j;
        while(i>0) {
            if(moves[i-1] == NULL) {
                moves[i-1] = moves[i];
                moves[i] = NULL;
                val[i-1] = val[i];
                val[1] = -1;
            } else if(moves[i] != NULL && val[i-1] > val[i]) {
                State* temp = moves[i];
                moves[i] = moves[i-1];
                moves[i-1] = temp;
                char tempVal = val[i];
                val[i] = val[i-1];
                val[i-1] = tempVal;
            }
            i--;
        }
    }
    char bestValue = -127;
    for(j=0; j<9; j++) {
        if(moves[j] != NULL) {
            states++;
            char v = -alphabeta(moves[j], me, depth-1, -beta, -alpha, -color, val[j]);
            free(moves[j]);
            bestValue = max(bestValue, v);
            alpha = max(alpha, v);
            if(alpha >= beta) break;
        } else break;
    }
    free(moves);
    return bestValue;
}

char getBestMove(State* state, char me, char depth) {
    State** moves = malloc(sizeof(State)*9);
    char val[9];
    int j;
    for(j=0; j<9; j++) {
        if(State_isLegalMove(state, j)) {
            moves[j] = State_next(state, state->nextBoard, j);
            val[j] = State_heuristic(moves[j], me);
        } else {
            moves[j] = NULL;
            val[j] = -127;
        }
    }
    for(j=1; j<9; j++) {
        int i = j;
        while(i>0) {
            if(moves[i-1] == NULL) {
                moves[i-1] = moves[i];
                moves[i] = NULL;
                val[i-1] = val[i];
                val[1] = -127;
            } else if(moves[i] != NULL && val[i-1] > val[i]) {
                State* temp = moves[i];
                moves[i] = moves[i-1];
                moves[i-1] = temp;
                char tempVal = val[i];
                val[i] = val[i-1];
                val[i-1] = tempVal;
            }
            i--;
        }
    }
    char alpha = -127;
    char beta = 127;
    char bestValue = -127;
    char move = -1;
    for(j=0; j<9; j++) {
        if(moves[j] != NULL) {
            states++;
            char v = -alphabeta(moves[j], me, depth-1, -beta, -alpha, -1, val[j]);
            if(v > bestValue) {
                move = moves[j]->nextBoard;
                bestValue = v;
            }
            free(moves[j]);
            bestValue = max(bestValue, v);
            alpha = max(alpha, v);
            if(alpha >= beta) break;
        } else break;
    }
    free(moves);
    return move;
}

void newGame(char me) {
    // Initialize state to empty board
    State* currentState = State_init(me);
    char winner = -1;
    // If game is not yet over
    while(winner == -1) {
        if(currentState->player == me) {
            // Calculate best move and apply
            struct timespec start, end;
            char depth = 9; // Starting depth 10
            fullyExplored = 0;
            // Reset counters
            clock_gettime(CLOCK_MONOTONIC, &start);
            end = start;
            char j = -1;
            // Time-limited iterative deepening - run at higher depths until time limit reached
            // This may take more than one second e.g. if previous call finished in .99 seconds
            // It also sometimes takes less than a second although I couldn't figure out why
            while((end.tv_sec - start.tv_sec) < 1 && !fullyExplored) {
                // If the heuristic is ever called, this will be set to 0 and the loop will continue
                fullyExplored = 1;
                states = 0;
                depth++;
                // Get best move (space on current board)
                j = getBestMove(currentState, me, depth);
                // Check current time
                clock_gettime(CLOCK_MONOTONIC, &end);
            }
            // Print statistics
            printf("Plies explored: %d\n", depth);
            printf("States explored: %d\n", states);
            printf("Search time: %lu ms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
            printf("I move to board %d space %d.\n", currentState->nextBoard+1, j+1);
            // Update and print currentState
            currentState = State_next(currentState, currentState->nextBoard, j);
            State_print(currentState, me);
        } else {
            // Ask for player input and apply move
            int i, j;
            if(currentState->lastBoard == -1) {
                // For initial move, ask for both board number and space number
                printf("Which board (1-9) and space (1-9) would you like to move to?\n");
                char input[64];
                while(fgets(input, 64, stdin) == NULL || input[1] != ' ' || input[3] != '\n' 
                    || (input[0] < '1' && input[0] > '9' && input[2] < '1' && input[2] > '9')) {
                    printf("Incorrent input, please enter two numbers from 1-9.\n");
                }
                // Converts ASCII representation to 0-indexed integer
                i = input[0] - '1';
                j = input[2] - '1';
            } else {
                // Otherwise just ask for state number
                i = currentState->nextBoard;
                printf("Where would you like to move on board %d?\n", i+1);
                char input[64];
                // Some error checking - ensure single integer entered between 1-9 that corresponds to empty square
                while(fgets(input, 64, stdin) == NULL || input[1] != '\n' || (input[0] < '1' && input[0] > '9') 
                    || !State_isLegalMove(currentState,  input[0] - '1')) {
                    printf("Improper input, please enter a number 1-9 that correspons to an unoccupied square on board %d.\n", i+1);
                }
                j = input[0] - '1';
            }
            // Update currentState
            currentState = State_next(currentState, i, j);
        }
        // Check if state is winning condition
        winner = State_winner(currentState);
    }
    if(winner) {
        // If it's my turn it means I just lost
        printf("%s won!\n", currentState->player == me ? "You" : "I");
    } else {
        // If game over but no winner (very rare!) declare a draw
        printf("Draw!\n");
    }
}

int main(int argc, char* argv[]) {
    // Seed random number generator (used for deciding board for first move) based on current time
    srand(time(NULL));
    printf("Welcome to 9-Board!\n");
    char again[64] = { 'y' };
    // Keep going until player doesn't want to play any more
    while(again[0] == 'y' || again[0] == 'Y') {
        printf("Would you like to play X or O?\n");
        char player[64];
        while(fgets(player, 64, stdin) == NULL || player[1] != '\n' 
            || (player[0] != 'x' && player[0] != 'X' && player[0] != 'o' && player[0] != 'O')) {
            printf("Improper input, please enter either \"X\" or \"O\".\n");
        }
        newGame(player[0] == 'x' || player[0] == 'X');
        printf("Would you like to play again? (Y/N)\n");
        while(fgets(again, 64, stdin) == NULL || again[1] != '\n' 
            || (again[0] != 'y' && again[0] != 'Y' && again[0] != 'n' && again[0] != 'N')) {
            printf("Improper input, please enter either \"Y\" or \"N\".\n");
        }
    }
    printf("Goodbye.\n");
    return 0;
}
