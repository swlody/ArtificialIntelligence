import java.util.LinkedList;

/*
 * Sam Wlody
 * CSC 242 - Project 1
 * I affirm that I have not given or received any unauthorized
 * help on this assignment, and that this work is my own.
 */

public class Queue<T> {
	LinkedList<T> queueList;
	
	public Queue() {
		queueList = new LinkedList<T>();
	}
	
	public boolean isEmpty() {
		return queueList.isEmpty();
	}
	
	public void enqueue(T x) {
		queueList.add(x);
	}
	
	public T dequeue() {
		return queueList.removeFirst();
	}
	
	public T peek() {
		return queueList.getFirst();
	}
}
