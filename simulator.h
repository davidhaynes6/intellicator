#pragma once

#include <vector>
#include <random>

class Simulator {
public:
    static int constexpr ONE_YEAR_TRADING_DAYS = 252;
    Simulator(double S0=0.0, double mu=0.0, double sigma=0.0, double timeHorizon=0.0, int numSimulations=0);
    double simulateStockPrice(double S0, double mu, double sigma, double timeHorizon, std::mt19937& generator) const;

    double getS0() { return S0; }
    double getMu() { return mu; }
    double getSimga() { return sigma; }
    double getTimeHorizon() { return timeHorizon; }
    int getNumSimulations() { return numSimulations; }
    
    // Add member functions for setting parameters and performing simulations.
    void setParameters(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations);

private:
    double S0; // Initial stock price
    double mu; // Expected return
    double sigma; // Volatility
    double timeHorizon; // Time horizon in years
    int numSimulations; // Number of Monte Carlo simulations

    std::vector<double> finalPrices;
};


