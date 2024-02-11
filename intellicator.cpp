#include "intellicator.h"
#include <QThread>
#include <QtCharts>
#include <sstream>
#include <ctime>

Intellicator::Intellicator(QWidget* parent) : QMainWindow(parent), initialPrice(100.0F), expectedReturn(0.1025), volatility(0.2F), timeHorizon(1.0F), numSimulations(10000)
{
    sim = nullptr;
    simulator = nullptr;
    simThread = nullptr;

    ui.setupUi(this);
    ui.editInitialPrice->setText(QString::number(initialPrice));
    ui.editInitialPrice->setInputMask("$999");
    QString expectedReturnStr = QString::number(expectedReturn * 100.0, 'f', 2);
    ui.editExpectedReturn->setText(expectedReturnStr);
    ui.editExpectedReturn->setInputMask("99.99\\%");
    ui.editVolatility->setText(QString::number(volatility * 100.0));
    ui.editVolatility->setInputMask("99\\%");
    ui.editTimeHorizon->setText(QString::number(timeHorizon));
    ui.editNumSimulations->setText(QString::number(numSimulations));
    ui.progressBar->setRange(0, 100);
    ui.progressBar->setValue(0);
    connect(ui.buttonExecute, &QPushButton::clicked, this, &Intellicator::onButtonClicked);
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
        connect(simThread, &QThread::finished, simThread, &QThread::deleteLater);       // schedule object for deletion
        connect(sim, &Simulation::finished, this, &Intellicator::finishedTask);
        connect(sim, &Simulation::finished, simThread, &QThread::quit);
        connect(sim, &Simulation::finished, sim, &Simulation::deleteLater);             // schedule object for deletion
        connect(sim, &Simulation::progressUpdated, this, &Intellicator::updateProgress);
        connect(sim, &Simulation::priceDataReady, this, &Intellicator::createChart);
    }
}

void Intellicator::onButtonClicked()
{
    ui.progressBar->setValue(0);
    ui.buttonExecute->setDisabled(true);

    // Retrieve user input and update member variables
    QString initialPriceStr = ui.editInitialPrice->text().remove('$');
    initialPrice = initialPriceStr.toDouble();

    QString expectedReturnStr = ui.editExpectedReturn->text().remove('%');
    expectedReturn = expectedReturnStr.toDouble() / 100.0;
    
    QString editVolatilityStr = ui.editVolatility->text().remove('%');
    volatility = editVolatilityStr.toDouble() / 100.0;
    
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
    // Pass the updated values to the simulation object
    simulator->setParameters(initialPrice, expectedReturn, volatility, timeHorizon, numSimulations);
    sim->doWork(simulator);
}

void Intellicator::updateProgress(int value)
{
    ui.progressBar->setValue(value);
}

void Intellicator::finishedTask(double value)
{
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the time to a tm structure for extracting date and time components
    std::tm* localTime = std::localtime(&currentTime);

    // Define a stringstream for the formatted date and time
    std::stringstream ss;

    // Format the date and time as "YYYY-MM-DD HH:MM:SS"
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    // Stream result for display, add Time + Average Price + Years selected
    std::ostringstream result;
    result << ss.str() << " -- Avg Price: $" << value << std::endl 
        << " S0: " << initialPrice << " mu: " << expectedReturn 
        << " sigma: " << volatility << " years: " << timeHorizon 
        << " sims: " << numSimulations << std::endl;

    ui.editResult->append(result.str().c_str());

    // Clean Up WorkerThread
    delete sim;
    sim = nullptr;

    // Stop the previous simulation and wait for it to finish
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
    // Check if a QChartView already exists in the container
    QChartView* chartView = nullptr;
    if (ui.chartContainer->layout() && ui.chartContainer->layout()->count() > 0) {
        chartView = dynamic_cast<QChartView*>(ui.chartContainer->layout()->itemAt(0)->widget());
    }

    // If QChartView doesn't exist, create and set it up
    if (!chartView) {
        chartView = new QChartView();
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(chartView);
        ui.chartContainer->setLayout(layout);
    }

    // Clear the existing chart data
    QChart* chart = chartView->chart();
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());
    chart->createDefaultAxes();

    // Create new series with the new data
    QLineSeries* series = new QLineSeries();
    series->setName("Simulations");

    for (const Point& point : data) {
        series->append(static_cast<qreal>(point.simulation), static_cast<qreal>(point.price));
    }

    // Add the new series to the chart
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setTitleText("Simulation Number");
    chart->axisY()->setTitleText("Simulated Price ($)");
}