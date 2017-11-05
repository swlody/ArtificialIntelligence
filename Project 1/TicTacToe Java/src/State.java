/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

public class State {
	// Board represented by an array - 0 is empty, 1 is "x", 2 is "o"
	private char board[];
	private char player;
	private int moveNumber;
	
	public void setTurn(char player) {
		this.player = player;
	}
	
	public char whoseTurn() {
		return this.player;
	}
	
	public State(int moveNumber) {
		this.board = new char[9];
		this.moveNumber = moveNumber;
	}
	
	public int getMoveNumber() {
		return moveNumber;
	}
	
	public char[] getBoard() {
		return board;
	}
	
	public void setBoard(char[] board) {
		this.board = board;
	}
	
	public void move(int square) {
		board[square] = player;
		player = (player == 'x' ? 'o' : 'x');
		moveNumber++;
	}
	
	public boolean checkMove(int move) {
		return board[move] != 'x' && board[move] != 'o';
	}
	
	public void print() {
		System.out.println((board[0]==0 ? '1' : board[0]) + " | " + (board[1]==0 ? '2' : board[1]) + " | " + (board[2]==0 ? '3' : board[2]));
		System.out.println("---------");
		System.out.println((board[3]==0 ? '4' : board[3]) + " | " + (board[4]==0 ? '5' : board[4]) + " | " + (board[5]==0 ? '6' : board[5]));
		System.out.println("---------");
		System.out.println((board[6]==0 ? '7' : board[6]) + " | " + (board[7]==0 ? '8' : board[7]) + " | " + (board[8]==0 ? '9' : board[8]));
	}
	
	public char getWinner() {
		if(board[0] != 0 && board[0] == board[1] && board[1] == board[2]) return board[0];
		else if(board[3] != 0 && board[3] == board[4] && board[4] == board[5]) return board[3];
		else if(board[6] != 0 && board[6] == board[7] && board[7] == board[8]) return board[6];
		else if(board[0] != 0 && board[0] == board[3] && board[3] == board[6]) return board[0];
		else if(board[1] != 0 && board[1] == board[4] && board[4] == board[7]) return board[1];
		else if(board[2] != 0 && board[2] == board[5] && board[5] == board[8]) return board[2];
		else if(board[0] != 0 && board[0] == board[4] && board[4] == board[8]) return board[0];
		else if(board[2] != 0 && board[2] == board[4] && board[4] == board[6]) return board[2];
		else if(moveNumber == 9) return 't';
		else return 'n';
	}
}
