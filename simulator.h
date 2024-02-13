#pragma once

#include <vector>
#include <random>

class Simulator {
public:
    static int constexpr ONE_YEAR_TRADING_DAYS = 252;
    Simulator(double S0=0.0, double mu=0.0, double sigma=0.0, double timeHorizon=0.0, int numSimulations=0);
    double simulateStockPrice(double S0, double mu, double sigma, double timeHorizon, std::mt19937& generator) const;

    // This variable represents the initial price of the stock.
    // This is the price at the beginning of the simulation.
    double getS0() 
    { 
        return S0; 
    }

    // This variable represents the expected return of the stock. 
    // This is the average rate of return that an investor anticipates receiving from an investment.
    double getMu() 
    { 
        return mu; 
    }

    // This variable represents the volatility of the stock.
    // This is the degree of variation of a trading price series over time. Often used to indicate the risk of the stock.
    double getSimga() 
    { 
        return sigma; 
    }

    // This variable represents the time horizon of the simulation.
    // This is the duration over which the simulation is performed.
    double getTimeHorizon() 
    { 
        return timeHorizon; 
    }

    // Number of simulations to run.
    int getNumSimulations() 
    { 
        return numSimulations; 
    }

    // Add member functions for setting parameters and performing simulations.
    void setParameters(double initialPrice, double expectedReturn, double volatility, double timeHorizon, int numSimulations);

private:
    double S0;          // Initial stock price
    double mu;          // Expected return
    double sigma;       // Volatility
    double timeHorizon; // Time horizon in years
    int numSimulations; // Number of Monte Carlo simulations

    std::vector<double> finalPrices;
};