#include "simulation.h"
#include <QThread>
#include <QPointF>
#include <QVector>
#include <cmath>

Simulation::Simulation(QObject* parent, int numSimulations)
    : QObject(parent), numSimulations(numSimulations) 
{
}

/*
void Simulation::doWork(Simulator* simulator) {

    double averagePercentage = 0.0;
    std::mt19937 generator(std::random_device{}());
    for (auto simulation = 0; simulation < simulator->getNumSimulations(); ++simulation) {
        double simulatedPrice = simulator->simulateStockPrice(simulator->getS0(), simulator->getMu(), simulator->getSimga(), simulator->getTimeHorizon(), generator);

        // Calculate the percentage progress and emit it as the progress value
        int progressPercentage = static_cast<int>((simulation + 1) * 100.0 / simulator->getNumSimulations());
        emit progressUpdated(progressPercentage);
        averagePercentage += simulatedPrice;
    }

    averagePercentage = averagePercentage / simulator->getNumSimulations();
    emit finished(averagePercentage);
}
*/

void Simulation::doWork(Simulator* simulator) {

    std::vector<Point> priceData;
    auto averagePercentage = 0.0;
    std::mt19937 generator(std::random_device{}());
    for (auto simulation = 0; simulation < simulator->getNumSimulations(); ++simulation) {
        double simulatedPrice = simulator->simulateStockPrice(simulator->getS0(), simulator->getMu(), simulator->getSimga(), simulator->getTimeHorizon(), generator);

        // Store the simulated price with the corresponding time point
        priceData.push_back(Point(simulation, simulatedPrice));

        // Calculate the percentage progress and emit it as the progress value
        int progressPercentage = static_cast<int>((simulation + 1) * 100.0 / simulator->getNumSimulations());
        emit progressUpdated(progressPercentage);
        averagePercentage += simulatedPrice;
    }
    emit priceDataReady(priceData);   // Emit the price data
    emit finished(averagePercentage); // Emit finished

}



