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

/* 
 * chars are used for most values instead of ints for smaller memory footprint
 * (not sure how much of a difference this actually makes)
 */
char fullyExplored;
// Counters for # of states explored, not expected to overflow a single byte
int states, winStates, leafStates;
/*
 * Increasing this value will allow for slightly deeper searches in the early-game
 * at the expense of exponential search time increase and negligible difference in
 * late-game performance and search time. Probably best to leave at default (10).
 */
const char searchDepth = 10;

// TODO Get rid of inefficient State struct
// TODO Add some randomness to moves - shuffle moves for getBestMove?
// TODO Improve ordering in 9BoardOrdered.c - might make a performance difference?
// TODO ++

char evaluate(State* state, char winner, char me, char color) {
    if(winner == -1) {
        // Game not over -> call heuristic & mark as not fully explored so iterative deepening (106) continues
        fullyExplored = 0;
        leafStates++;
        // Heuristic returns number of maximizing player's 2-in-a-rows subtracted by opponent's 2-in-a-rows
        return 5 * color * State_heuristic(state, me);
    }
    // Increment win counter
    if(color == -1 && winner) 
        winStates++;
    // If winner is currently true, it means the current maximizing player made the final move, so they get the value of 100
    // Since -alphabeta is called (result of negamax), return -100 so the negative is negated
    return -100 * winner;
}

char alphabeta(State* state, char me, char depth, char alpha, char beta, char color) {
    // Negamax variation of alpha-beta search - same output and runtime as minimax with less code
    char winner = State_winner(state);
    // If the game is over (-1 returns if not over) or depth limit is reached
    if(depth == 0 || winner != -1)
        return evaluate(state, winner, me, color);
    char bestValue = -127;
    int j;
    // For all potential moves
    for(j=0; j<9; j++) {
        // If the move is legal
        if(state->board[(int)state->nextBoard][j] == -1) {
            // Increment state counter
            states++;
            // Generate the next state based on previous move
            State* explore = State_new(state, j);
            // Run alpha-beta on state
            char v = -alphabeta(explore, me, depth-1, -beta, -alpha, -color);
            // Free state from memory when no longer needed
            free(explore);
            // Get max of alpha-beta and current best value
            bestValue = max(bestValue, v);
            // Search cutoff test
            alpha = max(alpha, v);
            if(alpha >= beta) 
                break;
        }
    }
    return bestValue;
}

char getBestMove(State* state, char me, char depth) {
    // Returns a move rather than a bestValue, otherwise same as above with some intialization
    char move = -1;
    char bestValue = -127;
    char alpha = -127;
    char beta = 127;
    int j;
    for(j=0; j<9; j++) {
        if(state->board[(int)state->nextBoard][j] == -1) {
            states++;
            State* explore = State_new(state, j);
            char v = -alphabeta(explore, me, depth-1, -beta, -alpha, -1);
            free(explore);
            if(v > bestValue) {
                move = j;
                bestValue = v;
            }
            alpha = max(alpha, v);
            if(alpha >= beta) 
                break;
        }
    }
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
            char depth = searchDepth - 1;
            fullyExplored = 0;
            // Reset counters
            winStates = 0;
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
            printf("Leaf nodes: %d\n", leafStates);
            printf("Winning states: %d (%05.2f%%)\n", winStates, ((double)winStates / (double)leafStates)*100);
            printf("Search time: %I64d ms\n", (end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000);
            printf("I move to board %d space %d.\n", currentState->nextBoard+1, j+1);
            // Update and print currentState
            State_next(currentState, currentState->nextBoard, j);
            State_print(currentState, me);
        } else {
            // Ask for player input and apply move
            int i, j;
            if(currentState->lastBoard == -1) {
                // For initial move, ask for both board number and space number
                printf("Which board (1-9) and space (1-9) would you like to move to?\n");
                char input[64];
                while(fgets(input, 64, stdin) == NULL || input[1] != ' ' || input[3] != '\n' 
                        || (input[0] < '1' && input[0] > '9' && input[2] < '1' && input[2] > '9'))
                    printf("Incorrent input, please enter two numbers from 1-9.\n");
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
                        || currentState->board[(int)currentState->nextBoard][input[0] - '1'] != -1)
                    printf("Improper input, please enter a number 1-9 that correspons to an unoccupied square on board %d.\n", i+1);
                j = input[0] - '1';
            }
            // Update currentState
            State_next(currentState, i, j);
        }
        // Check if state is winning condition
        winner = State_winner(currentState);
    }
    // If it's my turn it means I just lost
    // If game over but no winner (very rare!) declare a draw
    if(winner)
        printf("%s won!\n", currentState->player == me ? "You" : "I");
    else
        printf("Draw!\n");
    free(currentState);
}

int main(int argc, char* argv[]) {
    // Seed random number generator (used for deciding board for first move) based on current time
    srand(time(NULL));
    printf("Welcome to 9-Board!\n");
    char again[64] = "y";
    // Keep going until player doesn't want to play any more
    while(again[0] == 'y' || again[0] == 'Y') {
        printf("Would you like to play X or O?\n");
        char player[64];
        while(fgets(player, 64, stdin) == NULL || player[1] != '\n' 
                || (player[0] != 'x' && player[0] != 'X' && player[0] != 'o' && player[0] != 'O'))
            printf("Improper input, please enter either \"X\" or \"O\".\n");
        newGame(player[0] == 'x' || player[0] == 'X');
        printf("Would you like to play again? (Y/N)\n");
        while(fgets(again, 64, stdin) == NULL || again[1] != '\n' 
                || (again[0] != 'y' && again[0] != 'Y' && again[0] != 'n' && again[0] != 'N'))
            printf("Improper input, please enter either \"Y\" or \"N\".\n");
    }
    printf("Goodbye.\n");
    return 0;
}
