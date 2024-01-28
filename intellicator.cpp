#include "intellicator.h"
#include <QThread>
#include <QtCharts>
#include <sstream>
#include <ctime>

Intellicator::Intellicator(QWidget* parent) : QMainWindow(parent)
{
    sim = nullptr;
    simulator = nullptr;
    simThread = nullptr;

    ui.setupUi(this);
    ui.progressBar->setValue(0);
    connect(ui.buttonExecute, &QPushButton::clicked, this, &Intellicator::onButtonClicked);

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

Intellicator::~Intellicator()
{
}

void Intellicator::initializeThread() {

    if (sim == nullptr && simulator == nullptr && simThread == nullptr) {
        sim = new Simulation(this, numSimulations);
        simulator = new Simulator();
        simThread = new QThread(this);
        sim->moveToThread(simThread);
        connect(simThread, &QThread::started, this, &Intellicator::startSimulation);
        connect(sim, &Simulation::finished, this, &Intellicator::finishedTask);
        connect(sim, &Simulation::progressUpdated, this, &Intellicator::updateProgress);
        connect(sim, &Simulation::finished, simThread, &QThread::quit);
        connect(sim, &Simulation::finished, sim, &Simulation::deleteLater);
        connect(simThread, &QThread::finished, simThread, &QThread::deleteLater);
        connect(sim, &Simulation::priceDataReady, this, &Intellicator::createChart);
    } 
}

void Intellicator::onButtonClicked()
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

void Intellicator::startTask()
{
    initializeThread();
    simThread->start();
}

void Intellicator::startSimulation()
{
    // Pass the updated values to the simulation object.
    simulator->setParameters(initialPrice, expectedReturn, volatility, timeHorizon, numSimulations);
    sim->doWork(simulator);
}

void Intellicator::updateProgress(int value)
{
    ui.progressBar->setValue(value);
}

void Intellicator::finishedTask(int value)
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

    delete simulator;
    simulator = nullptr;

    ui.buttonExecute->setDisabled(false);
}

void Intellicator::createChart(const std::vector<Point>& data) {
    QLineSeries* series = new QLineSeries();

    for (const Point& point : data) {
        series->append(static_cast<qreal>(point.simulation), static_cast<qreal>(point.price));
    }

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setTitleText("Simulation Number");
    chart->axisY()->setTitleText("Simulated Price ($)");

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set the chart view size to fill the container
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->resize(ui.chartContainer->size());

    // Set chartView as a child of chartContainer
    QVBoxLayout* layout = new QVBoxLayout(ui.chartContainer);
    layout->addWidget(chartView);
    ui.chartContainer->setLayout(layout);
}

