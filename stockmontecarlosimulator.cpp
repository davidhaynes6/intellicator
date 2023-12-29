#include "stockmontecarlosimulator.h"
#include <cmath>

stockmontecarlosimulator::stockmontecarlosimulator(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations) 
:S0(initialPrice), mu(expectedReturn), sigma(volatility), timeHorizon(timeHorizon), numSimulations(numSimulations) {
}

double stockmontecarlosimulator::simulateStockPrice(double S0, double mu, double sigma, double timeHorizon, std::mt19937& generator) const {
    std::normal_distribution<double> distribution(0, 1);
    double S = S0;

    for (double t = 0; t < timeHorizon; t += 1.0 / 252) {
        S += mu * S * (1.0 / 252) + sigma * S * sqrt(1.0 / 252) * distribution(generator);
    }

    return S;
}

void stockmontecarlosimulator::setParameters(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations) {
    this->S0 = initialPrice;
    this->mu = expectedReturn;
    this->sigma = volatility;
    this->timeHorizon = timeHorizon;
    this->numSimulations = numSimulations;
}
