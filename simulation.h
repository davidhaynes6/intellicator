#pragma once

#include <QObject>
#include <vector>
#include <random>
#include "simulator.h"

struct Point
{
    int simulation;
    float price;

    Point(int simulation, float price) : simulation(simulation), price(price) {}
};

class Simulation : public QObject {
    Q_OBJECT

public:
    explicit Simulation(QObject* parent=nullptr, int numSimulations=10000);

signals:
    void progressUpdated(int value);
    void finished(double value);
    void priceDataReady(std::vector<Point> value);

public slots:
    void doWork(std::shared_ptr<Simulator> simulator);

private:
    int numSimulations;
};