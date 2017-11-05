/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

import java.util.InputMismatchException;
import java.util.Scanner;

public class TicTacToe {
	private void newGame(Scanner scan) {
		// Ask player for letter
		System.out.println("Would you like to play X or O?");
		String userLetter = scan.nextLine();
		while(!userLetter.equalsIgnoreCase("X") && !userLetter.equalsIgnoreCase("O")) {
			System.out.println("Improper input, please enter \"O\" or \"X\".");
			userLetter = scan.nextLine();
		}
		
		// Initialize to empty board
		char me;
		State currentState = new State(0);
		
		// Store character that computer will use to move, and initialize variable to keep track of turn
		me = (userLetter.equalsIgnoreCase("X") ? 'o' : 'x');
		currentState.setTurn('x');
		
		// Loop while game is not in an end state
		char winner = 'n';
		while(winner == 'n') {
			// If it's my turn to move...
			if(currentState.whoseTurn() == me) {
				// Build decision tree
				Tree decisionTree = new Tree();
				decisionTree.build(currentState);
				// Use mimimax to choose best solution
				State nextState = decisionTree.getMove(me);
				// Find difference between current state and my move
				int myMove = 0;
				for(int i=0; i<9; i++) {
					if(currentState.getBoard()[i] != nextState.getBoard()[i]) {
						myMove = i+1;
					}
				}
				// Set current state to my move and print my move
				currentState = nextState;
				System.out.println("I move to " + myMove + ".");
			} else {
				// Player move
				System.out.println("The current board is:");
				currentState.print();
				System.out.println("Where would you like to move? (1-9)");
				int playerMove = 10;
				while(true) {
					// Ensure input is valid integer, otherwise loop
					try {
						playerMove = scan.nextInt();
					} catch(InputMismatchException e) {
						System.out.println("Improper input, please enter a number from 1-9.");
						continue;
					}
					playerMove--;
					// Ensure integer is valid move and break, otherwise loop
					if(playerMove >=0 && playerMove <=8 && currentState.checkMove(playerMove)) {
						break;
					} else {
						System.out.println("Improper input, please enter a number 1-9 that does not refer to an occupied square.");
					}
				}
				// Update current state with player move
				currentState.move(playerMove);
			}
			winner = currentState.getWinner();
		}
		if(winner == 't') {
			System.out.println("The game ended in a draw! The final board state is:");
		} else {
			System.out.println("Game over, " + (winner == me ? "I" : "You") + " win! The final board state is:");
		}
		currentState.print();
	}
	
	private TicTacToe() {
		System.out.println("Welcome to TicTacToe!");
		System.out.println("Move by entering a number 1-9.");
		
		// Print sample board
		System.out.println(1 + " | " + 2 + " | " + 3);
		System.out.println("---------");
		System.out.println(4 + " | " + 5 + " | " + 6);
		System.out.println("---------");
		System.out.println(7 + " | " + 8 + " | " + 9);
		
		Scanner scan = new Scanner(System.in);
		
		String input = "Y";
		
		// Loop until user doesn't want to play any more
		while(input.equalsIgnoreCase("Y")) {
			newGame(scan); // Pass scanner in to prevent creating multiple instances
			scan.nextLine();
			System.out.println("Would you like to play again? (Y/N)");
			input = scan.nextLine();
			while(!input.equalsIgnoreCase("Y") && !input.equalsIgnoreCase("N")) {
				System.out.println("Improper input, please enter \"Y\" or \"N\".");
				input = scan.nextLine();
			}
		}
		
		System.out.println("Goodbye.");
		scan.close();
	}
	
	public static void main(String[] args) {
		new TicTacToe();
	}
}
