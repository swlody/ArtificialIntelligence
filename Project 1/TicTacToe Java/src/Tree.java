/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

import java.util.LinkedList;

public class Tree {
	private int statesBFS, statesMinimax;
	
	private class Node {
		private State state;
		private LinkedList<Node> children;
		private int value;
		
		// Expand children as part of breadth-first search
		private void expand(int moveNumber) {
			children = new LinkedList<>();
			// For each possible move
			for(int i=0; i<=8; i++) {
				// If square isn't occupied...
				if(state.getBoard()[i] != 'x' && state.getBoard()[i] != 'o') {
					// Create new node and set as a child of current node
					Node newNode = new Node();
					State newState = new State(moveNumber+1);
					/*
					 * Using:
					 * char[] newBoard = new char[9];
					 * newBoard = state.getBoard();
					 * 
					 * Makes a reference to the current board, instead of making a copy of it.
					 * This means that any changes made to the new board will also be made to the current board.
					 * Instead a deep copy is made to copy each value from the current board onto the new board.
					 * */
					char[] newBoard = new char[9];
					for(int j = 0; j<=8; j++) {
						newBoard[j] = state.getBoard()[j];
					}
					newBoard[i] = state.whoseTurn();
					newState.setBoard(newBoard);
					newState.setTurn(state.whoseTurn() == 'x' ? 'o' : 'x');
					newNode.state = newState;
					children.add(newNode);
				}
			}
		}
		
		private int minimax(boolean max, char me) {
			// Standard minimax algorithm
			// If in end state (always true when leaf node, i.e. no children of node
			if(children == null) {
				// There might be an easier way to check win state - ~6% cost
				char win = state.getWinner();
				// Much prefer winning to tieing, but still prefer tieing to losing
				if(win == me) return 100;
				else if(win == 't') return 0;
				else return -1;
			}
			// Else run minimax
			if(max) {
				// If max (me), minimize possible score
				int bestValue = Integer.MIN_VALUE;
				// Recursively run minimax on children
				for(Node node : children) {
					statesMinimax++;
					int v = node.minimax(false, me);
					bestValue = Math.max(bestValue, v);
				}
				value = bestValue;
				return bestValue;
			} else {
				// If min (not me), maximize possible score
				int bestValue = Integer.MAX_VALUE;
				for(Node node : children) {
					statesMinimax++;
					int v = node.minimax(true, me);
					bestValue = Math.min(bestValue, v);
				}
				// Store value in node - space inefficient but easy to code, won't use this method for 9-board
				value = bestValue;
				return bestValue;
			}
		}
	}
	
	private Node root;
	
	public void build(State state) {
		long startTime = System.nanoTime();
		// Standard breadth-first search
		Queue<Node> frontier = new Queue<>();
		root = new Node();
		root.state = state;
		frontier.enqueue(root);
		statesBFS = 0;
		while(!frontier.isEmpty()) {
			if(frontier.peek().state.getWinner() == 'n') {
				frontier.peek().expand(frontier.peek().state.getMoveNumber());
				for(Node node : frontier.dequeue().children) {
					frontier.enqueue(node);
				}
			} else {
				frontier.dequeue();
			}
			statesBFS++;
		}
		long endTime = System.nanoTime();
		System.out.println("BFS: " + (endTime - startTime) / 1000 + "us");
	}
	
	public State getMove(char me) {
		// Run minimax on entire tree, storing minimax value in each node
		long startTime = System.nanoTime();
		root.minimax(true, me);
		long endTime = System.nanoTime();
		System.out.println("Minimax: " + (endTime - startTime) / 1000 + "us");
		System.out.println("BFS explored: " + statesBFS);
		System.out.println("Minimax explored:" + statesMinimax);
		// Find best score of possible current moves
		State choice = root.children.getFirst().state;
		int bestValue = Integer.MIN_VALUE;
		for(Node node : root.children) {
			if(node.value > bestValue) {
				choice = node.state;
				bestValue = node.value;
			}
		}
		return choice;
	}
}
