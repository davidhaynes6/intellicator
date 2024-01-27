#include "simulator.h"
#include <cmath>

Simulator::Simulator(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations) 
:S0(initialPrice), mu(expectedReturn), sigma(volatility), timeHorizon(timeHorizon), numSimulations(numSimulations) {
}

double Simulator::simulateStockPrice(double S0, double mu, double sigma, double timeHorizon, std::mt19937& generator) const {
    std::normal_distribution<double> distribution(0, 1);
    double price = S0;

    for (double t = 0; t < timeHorizon; t += 1.0 / ONE_YEAR_TRADING_DAYS) {
        price += mu * price * (1.0 / ONE_YEAR_TRADING_DAYS) + sigma * price * sqrt(1.0 / ONE_YEAR_TRADING_DAYS) * distribution(generator);
    }

    return price;
}

void Simulator::setParameters(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations) {
    this->S0 = initialPrice;
    this->mu = expectedReturn;
    this->sigma = volatility;
    this->timeHorizon = timeHorizon;
    this->numSimulations = numSimulations;
}
