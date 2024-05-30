#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>

class GrayWolfOptimizer {
private:
    int numWolves; // Number of wolves in the pack
    int dim; // Dimension of the problem
    int maxIter; // Maximum number of iterations
    std::vector<std::vector<double>> wolves; // Positions of the wolves
    std::vector<double> alphaWolf; // Best solution found (Alpha wolf)
    double alphaScore; // Score of the Alpha wolf
    std::vector<double> betaWolf; // Second best solution (Beta wolf)
    double betaScore; // Score of the Beta wolf
    std::vector<double> deltaWolf; // Third best solution (Delta wolf)
    double deltaScore; // Score of the Delta wolf
    double lowerBound; // Lower bound of search space
    double upperBound; // Upper bound of search space
    std::mt19937 rng; // Random number generator

    double objectiveFunction(const std::vector<double>& position) {
        double sum = 0.0;
        for (double val : position) {
            sum += val * val;
        }
        return sum;
    }

    void initializeWolves() {
        std::uniform_real_distribution<double> dist(lowerBound, upperBound);
        for (int i = 0; i < numWolves; i++) {
            for (int j = 0; j < dim; j++) {
                wolves[i][j] = dist(rng);
            }
            double score = objectiveFunction(wolves[i]);
            updateAlphaBetaDelta(wolves[i], score);
        }
    }

    void updateAlphaBetaDelta(const std::vector<double>& position, double score) {
        if (score < alphaScore) {
            alphaScore = score;
            alphaWolf = position;
        }
        else if (score < betaScore) {
            betaScore = score;
            betaWolf = position;
        }
        else if (score < deltaScore) {
            deltaScore = score;
            deltaWolf = position;
        }
    }

public:
    GrayWolfOptimizer(int numWolves, int dim, int maxIter, double lowerBound, double upperBound)
        : numWolves(numWolves), dim(dim), maxIter(maxIter), lowerBound(lowerBound), upperBound(upperBound),
        wolves(numWolves, std::vector<double>(dim)), alphaWolf(dim), betaWolf(dim), deltaWolf(dim),
        alphaScore(std::numeric_limits<double>::infinity()), betaScore(std::numeric_limits<double>::infinity()),
        deltaScore(std::numeric_limits<double>::infinity()), rng(std::random_device{}()) {}

    void optimize() {
        initializeWolves();

        for (int iter = 0; iter < maxIter; iter++) {
            double a = 2.0 - iter * (2.0 / maxIter); // Decrease linearly from 2 to 0

            for (int i = 0; i < numWolves; i++) {
                for (int j = 0; j < dim; j++) {
                    double r1 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);
                    double r2 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);

                    double A1 = 2 * a * r1 - a;
                    double C1 = 2 * r2;

                    double D_alpha = std::abs(C1 * alphaWolf[j] - wolves[i][j]);
                    double X1 = alphaWolf[j] - A1 * D_alpha;

                    r1 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);
                    r2 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);

                    double A2 = 2 * a * r1 - a;
                    double C2 = 2 * r2;

                    double D_beta = std::abs(C2 * betaWolf[j] - wolves[i][j]);
                    double X2 = betaWolf[j] - A2 * D_beta;

                    r1 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);
                    r2 = std::uniform_real_distribution<double>(0.0, 1.0)(rng);

                    double A3 = 2 * a * r1 - a;
                    double C3 = 2 * r2;

                    double D_delta = std::abs(C3 * deltaWolf[j] - wolves[i][j]);
                    double X3 = deltaWolf[j] - A3 * D_delta;

                    wolves[i][j] = (X1 + X2 + X3) / 3.0;
                }

                // Check if the new position is better
                double score = objectiveFunction(wolves[i]);
                updateAlphaBetaDelta(wolves[i], score);
            }

            std::cout << "Iteration: " << iter << " Best score: " << alphaScore << std::endl;
        }
    }
};