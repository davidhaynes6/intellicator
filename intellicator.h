#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_intellicator.h"
#include "simulation.h"
#include "simulator.h"

class Intellicator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Intellicator(QWidget *parent = nullptr);
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

    Ui::intellicatorClass ui;

    QThread* simThread;

    std::unique_ptr<Simulation> sim;
    std::unique_ptr<Simulator> simulator;
};