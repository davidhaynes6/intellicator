#pragma once

#include <QObject>
#include <vector>
#include <random>
#include "stockmontecarlosimulator.h"

class simulation : public QObject {
    Q_OBJECT

public:
    explicit simulation(QObject* parent=nullptr, int numSimulations=10000);

signals:
    void progressUpdated(int value);
    void finished(double value);

public slots:
    void doWork(stockmontecarlosimulator* s);

private:   
    int numSimulations;
};