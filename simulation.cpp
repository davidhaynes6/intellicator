#include "simulation.h"
#include <QThread>
#include <cmath>

Simulation::Simulation(QObject* parent, int numSimulations)
    : QObject(parent), numSimulations(numSimulations) 
{
}

void Simulation::doWork(Simulator* s) {

    double averagePercentage = 0.0;
    std::mt19937 generator(std::random_device{}());
    for (int i = 0; i < s->getNumSimulations(); ++i) {
        double simulatedPrice = s->simulateStockPrice(s->getS0(), s->getMu(), s->getSimga(), s->getTimeHorizon(), generator);

        // Calculate the percentage progress and emit it as the progress value
        int progressPercentage = static_cast<int>((i + 1) * 100.0 / s->getNumSimulations());
        emit progressUpdated(progressPercentage);
        averagePercentage += simulatedPrice;
    }

    averagePercentage = averagePercentage / s->getNumSimulations();
    emit finished(averagePercentage);
}




