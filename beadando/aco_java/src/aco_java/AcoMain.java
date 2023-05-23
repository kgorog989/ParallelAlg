package aco_java;


public class AcoMain {

	public static void main(String[] args) {
		System.out.println("ACO implementation");

		Task task = new Task(20, 7, "data/sh07.txt");

		for (int row = 0; row < task.getNumAnts(); row++) {
	        for (int col = 0; col < task.getAntTours()[row].length; col++) {
	            System.out.printf(task.getAntTours()[row][col] + " ");
	        }
	        System.out.println();
	    }

	}

}
