package aco_java;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Task {
	private int numAnts;
	private int numCities;
	private double[][] cityDistances;
	private double[][] pheromones;
	private int[][] antTours;
	private double[] antLengths;

	public Task(int numAnts, int numCities, String filePath) {
		super();
		this.numAnts = numAnts;
		this.numCities = numCities;
		initializeCityDistances(filePath);
		initializePheromones();
		initializeAnts();
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
		for (int i = 0; i < numAnts; i++) {
			antTours[i][0] = (int)(Math.random() * numCities);
			for (int j = 1; j < numCities; j++) {
				antTours[i][j] = -1;
			}
			antLengths[i] = 0;
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

}
