#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_intellicator.h"
#include "simulation.h"
#include "stockmontecarlosimulator.h"

class intellicator : public QMainWindow
{
    Q_OBJECT

public:
    intellicator(QWidget *parent = nullptr);
    ~intellicator();

    void initializeThread();

public slots:   
    void onButtonClicked();
    void startTask();
    void startSimulation();
    void updateProgress(int value);
    void finishedTask(int value);

private:

    double initialPrice = 100;
    double expectedReturn = 0.05;
    double volatility = 0.2;
    double timeHorizon = 1;
    int numSimulations = 10000;
    stockmontecarlosimulator* s;

    Ui::intellicatorClass ui;

    QProgressBar* progressBar;
    QPushButton* startButton;
    simulation* sim;
    QThread* simThread;
};
