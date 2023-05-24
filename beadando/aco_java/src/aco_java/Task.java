package aco_java;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Scanner;

public class Task {
	private int numAnts;
	private int numCities;
	private double[][] cityDistances;
	private double[][] pheromones;
	private int[][] antTours;
	private double[] antLengths;
	private int[] bestTour;
	private double bestLength;
	final static double ALPHA = 1.0;
	final static double BETA = 2.0;
	final static double RHO = 0.5;
	final static double Q = 100.0;

	public Task(int numAnts, int numCities, String filePath) {
		super();
		this.numAnts = numAnts;
		this.numCities = numCities;
		initializeCityDistances(filePath);
		initializePheromones();
		initializeAnts();
		this.bestLength = Double.POSITIVE_INFINITY;
		this.bestTour = new int[numCities];
	}

	private void initializeCityDistances(String filePath) {
		try {
			File file = new File(filePath);
			Scanner scanner = new Scanner(file);

			cityDistances = new double[numCities][numCities];

			int currentRow = 0;
			while (scanner.hasNextLine()) {
				String line = scanner.nextLine();
				String[] values = line.split(" ");

				for (int i = 0; i < values.length; i++) {
					cityDistances[currentRow][i] = Integer.parseInt(values[i]);
				}

				currentRow++;
			}

			scanner.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	private void initializePheromones() {
		pheromones = new double[numCities][numCities];
		for (int i = 0; i < numCities; i++) {
			for (int j = 0; j < numCities; j++) {
				pheromones[i][j] = 100;
			}
		}
	}

	private void initializeAnts() {
		antTours = new int[numAnts][numCities];
		antLengths = new double[numAnts];
		for (int i = 0; i < numAnts; i++) {
			antTours[i][0] = (int) (Math.random() * numCities);
			for (int j = 1; j < numCities; j++) {
				antTours[i][j] = -1;
			}
			antLengths[i] = 0;
		}
	}
	
	public void constructAntToursWithThreads() {
        Thread[] threads = new Thread[numAnts];
        for (int i = 0; i < numAnts; i++) {
            threads[i] = new AntThread(this, i);
            threads[i].start();
        }

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
	
	public void updatePheromones() {
		int city1, city2;
	    // Evaporating pheromones
	    for (int i = 0; i < numCities; i++)
	    {
	        for (int j = 0; j < numCities; j++)
	        {
	            pheromones[i][j] *= (1.0 - RHO);
	        }
	    }
	    // Depositing pheromones
	    for (int i = 0; i < numAnts; i++)
	    {
	        for (int j = 0; j < numCities - 1; j++)
	        {

	            city1 = antTours[i][j];
	            city2 = antTours[i][j + 1];
	            pheromones[city1][city2] += Q / antLengths[i];
	            pheromones[city2][city1] += Q / antLengths[i];
	        }
	        city1 = antTours[i][numCities - 1];
	        city2 = antTours[i][0];
	        pheromones[city1][city2] += Q / antLengths[i];
	        pheromones[city2][city1] += Q / antLengths[i];
	    }
	}
	
	public void findBestTour() {
		for (int i = 0; i < numAnts; i++)
	    {
	        if (antLengths[i] < bestLength)
	        {
	            setBestLength(antLengths[i]);
	            for (int j = 0; j < numCities; j++)
	            {
	                bestTour[j] = antTours[i][j];
	            }
	        }
	    }
	}

	public int getNumAnts() {
		return numAnts;
	}

	public void setNumAnts(int numAnts) {
		this.numAnts = numAnts;
	}

	public int getNumCities() {
		return numCities;
	}

	public void setNumCities(int numCities) {
		this.numCities = numCities;
	}

	public double[][] getCityDistances() {
		return cityDistances;
	}

	public void setCityDistances(double[][] cityDistances) {
		this.cityDistances = cityDistances;
	}

	public double[][] getPheromones() {
		return pheromones;
	}

	public void setPheromones(double[][] pheromones) {
		this.pheromones = pheromones;
	}

	public int[][] getAntTours() {
		return antTours;
	}

	public void setAntTours(int[][] antTours) {
		this.antTours = antTours;
	}

	public double[] getAntLengths() {
		return antLengths;
	}

	public void setAntLengths(double[] antLengths) {
		this.antLengths = antLengths;
	}

	public static double getAlpha() {
		return ALPHA;
	}

	public static double getBeta() {
		return BETA;
	}

	public static double getRho() {
		return RHO;
	}

	public static double getQ() {
		return Q;
	}

	public int[] getBestTour() {
		return bestTour;
	}

	public void setBestTour(int[] bestTour) {
		this.bestTour = bestTour;
	}

	public double getBestLength() {
		return bestLength;
	}

	public void setBestLength(double bestLength) {
		this.bestLength = bestLength;
	}

	@Override
	public String toString() {
		return "Task [numAnts=" + numAnts + ", numCities=" + numCities + ", cityDistances="
				+ Arrays.toString(cityDistances) + ", pheromones=" + Arrays.toString(pheromones) + ", antTours="
				+ Arrays.toString(antTours) + ", antLengths=" + Arrays.toString(antLengths) + ", bestTour="
				+ Arrays.toString(bestTour) + ", bestLength=" + bestLength + "]";
	}
	
	
	
	

}
