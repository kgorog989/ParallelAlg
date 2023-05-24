package aco_java;

import java.util.Random;

public class AntThread extends Thread {
	private Task task;
	private final int antId;

	public AntThread(Task task, int antId) {
		super();
		this.task = task;
		this.antId = antId;
	}

	@Override
	public void run() {
		Random random = new Random();
		double totalProbabilities, randomNum, probabilitySum, probabilityToMove;
		int currentCity, nextCity = 0, nextCityFound;
		int visitedCities[] = new int[task.getNumCities()];

		visitedCities[task.getAntTours()[antId][0]] = 1;
		currentCity = task.getAntTours()[antId][0];
		
		task.getAntLengths()[antId] = 0;
		for (int k = 1; k < task.getNumCities(); k++) {
			totalProbabilities = 0;
			randomNum = random.nextDouble();
			probabilitySum = 0;
			for (int l = 0; l < task.getNumCities(); l++) {
				if (visitedCities[l] == 0) {
					totalProbabilities += Math.pow(task.getPheromones()[currentCity][l], Task.getAlpha())
							* Math.pow(1.0 / task.getCityDistances()[currentCity][l], Task.getBeta());
				}
			}

			nextCityFound = 0;
			for (int m = 0; m < task.getNumCities(); m++) {
				if (visitedCities[m] == 0) {
					probabilitySum += Math.pow(task.getPheromones()[currentCity][m], Task.getAlpha())
							* Math.pow(1.0 / task.getCityDistances()[currentCity][m], Task.getBeta());
					probabilityToMove = probabilitySum / totalProbabilities;
					if (probabilityToMove >= randomNum) {
						nextCityFound = 1;
						nextCity = m;
						break;
					}
				}
			}

			if (nextCityFound == 0) {
				for (int n = task.getNumCities() - 1; n >= 0; n--) {
					if (visitedCities[n] == 0) {
						nextCity = n;
						break;
					}
				}
			}

			task.getAntTours()[antId][k] = nextCity;
			task.getAntLengths()[antId] += task.getCityDistances()[currentCity][nextCity];

			visitedCities[nextCity] = 1;
			currentCity = nextCity;
		}

		// Adding the distance between the starting and ending city to the length of the
		// tour
		int lastNum = task.getAntTours()[antId][task.getNumCities() - 1];
		int firstNum = task.getAntTours()[antId][0];
		task.getAntLengths()[antId] += task.getCityDistances()[lastNum][firstNum];

	}

	public Task getTask() {
		return task;
	}

	public void setTask(Task task) {
		this.task = task;
	}

	public int getAntId() {
		return antId;
	}

}
