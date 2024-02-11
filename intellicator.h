#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_intellicator.h"
#include "simulation.h"
#include "simulator.h"

class Intellicator : public QMainWindow
{
    Q_OBJECT

public:
    Intellicator(QWidget *parent = nullptr);
    ~Intellicator();

    void initializeThread();

public slots:
    void onButtonClicked();
    void startTask();
    void startSimulation();
    void updateProgress(int value);
    void finishedTask(double value);
    void createChart(const std::vector<Point>& data);

private:

    double initialPrice;
    double expectedReturn;
    double volatility;
    double timeHorizon;
    int numSimulations;
    Simulator* simulator;

    Ui::intellicatorClass ui;

    QProgressBar* progressBar;
    QPushButton* startButton;
    Simulation* sim;
    QThread* simThread;
};