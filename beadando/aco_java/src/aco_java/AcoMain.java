package aco_java;

import java.io.FileWriter;
import java.io.IOException;

public class AcoMain {

    public static void main(String[] args) {
        System.out.println("ACO implementation");
        long startTime, endTime, executionTime;
        double executionTimeInSecs;
        int numIterations = 100;
        int numAnts;
        int numCities = 312;
        String filePath = "data/usca312.txt";
        int maxAnts = 100;
        String outputPath = "data/times_usca312.txt";

        try (FileWriter writer = new FileWriter(outputPath)) {
            for (numAnts = 2; numAnts <= maxAnts; numAnts++) {
                startTime = System.nanoTime();
                Task task = new Task(numAnts, numCities, filePath);
                for (int k = 0; k < numIterations; k++) {
                    task.constructAntToursWithThreads();
                    task.findBestTour();
                    task.updatePheromones();
                }

                System.out.println("Best tour:");
                for (int i = 0; i < task.getBestTour().length; i++) {
                    System.out.print(task.getBestTour()[i] + " ");
                }
                System.out.println();

                System.out.println("Best length: " + task.getBestLength());
                endTime = System.nanoTime();
                executionTime = endTime - startTime;
                executionTimeInSecs = (double) executionTime / 1_000_000_000.0;
                writer.write(String.valueOf(numAnts)+ " " + String.valueOf(executionTimeInSecs) + " " + String.valueOf(task.getBestLength()));
                writer.write(System.lineSeparator());
            }
        } catch (IOException e) {
            System.out.println("An error occurred while writing to the file: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
