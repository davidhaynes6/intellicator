#include "intellicator.h"
#include "simulation.h"
#include <QThread>
#include <sstream>
#include <ctime>

intellicator::intellicator(QWidget* parent) : QMainWindow(parent)
{
    sim = nullptr;
    s = nullptr;
    simThread = nullptr;

    ui.setupUi(this);
    ui.progressBar->setValue(0);
    connect(ui.buttonExecute, &QPushButton::clicked, this, &intellicator::onButtonClicked);

    initialPrice = 100.0;
    expectedReturn = 0.05;
    volatility = 0.2;
    timeHorizon = 1.0;
    numSimulations = 10000;

    ui.editInitialPrice->setText(QString::number(initialPrice));
    ui.editExpectedReturn->setText(QString::number(expectedReturn));
    ui.editVolatility->setText(QString::number(volatility));
    ui.editTimeHorizon->setText(QString::number(timeHorizon));
    ui.editNumSimulations->setText(QString::number(numSimulations));
    ui.progressBar->setRange(0, 100);
}

intellicator::~intellicator()
{
}

void intellicator::initializeThread() {

    if (sim == nullptr && s == nullptr && simThread == nullptr) {
        sim = new simulation(this, numSimulations);
        s = new stockmontecarlosimulator();
        simThread = new QThread(this);
        sim->moveToThread(simThread);
        connect(simThread, &QThread::started, this, &intellicator::startSimulation);
        connect(sim, &simulation::finished, this, &intellicator::finishedTask);
        connect(sim, &simulation::progressUpdated, this, &intellicator::updateProgress);
        connect(sim, &simulation::finished, simThread, &QThread::quit);
        connect(sim, &simulation::finished, sim, &simulation::deleteLater);
        connect(simThread, &QThread::finished, simThread, &QThread::deleteLater);
    } 
}

void intellicator::onButtonClicked()
{
    ui.progressBar->setValue(0);
    ui.buttonExecute->setDisabled(true);

    // Retrieve user input and update member variables.
    initialPrice = ui.editInitialPrice->text().toDouble();
    expectedReturn = ui.editExpectedReturn->text().toDouble();
    volatility = ui.editVolatility->text().toDouble();
    timeHorizon = ui.editTimeHorizon->text().toDouble();
    numSimulations = ui.editNumSimulations->text().toInt();
    
    startTask();
}

void intellicator::startTask()
{
    initializeThread();
    simThread->start();
}

void intellicator::startSimulation()
{
    // Pass the updated values to the simulation object.
    s->setParameters(initialPrice, expectedReturn, volatility, timeHorizon, numSimulations);
    sim->doWork(s);
}

void intellicator::updateProgress(int value)
{
    ui.progressBar->setValue(value);
}

void intellicator::finishedTask(int value)
{
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the time to a tm structure for extracting date and time components
    std::tm* localTime = std::localtime(&currentTime);

    // Define a buffer for the formatted date and time
    char buffer[80];

    // Format the date and time as "YYYY-MM-DD HH:MM:SS"
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

        // Stream result for display, add Time + Average Price + Years selected
    std::ostringstream result;
    result << buffer << " -- Avg Price: $" << value << std::endl 
        << " S0: " << initialPrice << " mu: " << expectedReturn 
        << " sigma: " << volatility << " years: " << timeHorizon 
        << " sims: " << numSimulations << std::endl;

    ui.editResult->append(result.str().c_str());

    // Clean Up WorkerThread
    delete sim;
    sim = nullptr;

    // Stop the previous simulation and wait for it to finish.
    if (simThread->isRunning()) {
        simThread->quit();
        simThread->wait();
    }

    delete simThread;
    simThread = nullptr;

    delete s;
    s = nullptr;

    ui.buttonExecute->setDisabled(false);
}
